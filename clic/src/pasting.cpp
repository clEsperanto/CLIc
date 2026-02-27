#include "slicing.hpp"
#include "execution.hpp"
#include "utils.hpp"

#include <cstring>
#include <numeric>
#include <stdexcept>
#include <string>

namespace cle
{

namespace kernel
{

const char * paste_strided_kernel = R"CLC(
__kernel void paste_strided_kernel(
    __global const DTYPE * src,
    __global       DTYPE * dst,
    const int src_w,
    const int src_h,
    const int src_d,
    const int dst_w,
    const int dst_h,
    const int dst_d,
    const int x_start, const int x_step,
    const int y_start, const int y_step,
    const int z_start, const int z_step)
{
    const int ix = get_global_id(0);
    const int iy = get_global_id(1);
    const int iz = get_global_id(2);

    if (ix >= src_w || iy >= src_h || iz >= src_d)
        return;

    // Map input (small array) coords → destination (large array) coords
    const int dx = x_start + ix * x_step;
    const int dy = y_start + iy * y_step;
    const int dz = z_start + iz * z_step;

    if (dx < 0 || dx >= dst_w || dy < 0 || dy >= dst_h || dz < 0 || dz >= dst_d)
        return;

    const long src_offset = ((long)iz * src_h * src_w + (long)iy * src_w + (long)ix);
    const long dst_offset = ((long)dz * dst_h * dst_w + (long)dy * dst_w + (long)dx);

    dst[dst_offset] = src[src_offset];
}
)CLC";

} // namespace kernel


// ── internal helpers (shared with slice) ────────────────────────────────────

static auto
resolve_slices(const Array::Pointer & arr, const std::vector<Slice> & slices) -> std::array<ResolvedSlice, 3>
{
  const std::array<size_t, 3> shape = { arr->width(), arr->height(), arr->depth() };

  std::array<ResolvedSlice, 3> resolved;
  for (size_t axis = 0; axis < 3; ++axis)
  {
    const Slice & s = (axis < slices.size()) ? slices[axis] : Slice();
    auto [r_start, r_stop, r_step] = s.resolve(shape[axis]);
    size_t out_len = s.output_length(shape[axis]);

    resolved[axis] = { r_start, r_stop, r_step, out_len, s.is_index };
  }
  return resolved;
}


static auto
is_contiguous(const std::array<ResolvedSlice, 3> & resolved) -> bool
{
  for (auto & rs : resolved)
  {
    if (rs.step != 1)
      return false;
  }
  return true;
}


// ── validate that src shape matches the resolved region ─────────────────────

static void
validate_paste_shape(const Array::Pointer & src, const std::array<ResolvedSlice, 3> & resolved)
{
  // Expected shape of the source: for collapsed axes → 1, otherwise → resolved length
  const std::array<size_t, 3> expected = {
    resolved[0].is_index ? 1 : resolved[0].length,
    resolved[1].is_index ? 1 : resolved[1].length,
    resolved[2].is_index ? 1 : resolved[2].length,
  };

  if (src->width() != expected[0] || src->height() != expected[1] || src->depth() != expected[2])
  {
    throw std::invalid_argument(
      "paste(): source shape (" + std::to_string(src->width()) + ", " + std::to_string(src->height()) + ", " +
      std::to_string(src->depth()) + ") does not match the target region (" + std::to_string(expected[0]) + ", " +
      std::to_string(expected[1]) + ", " + std::to_string(expected[2]) + ").");
  }
}


// ── fast path: contiguous region ────────────────────────────────────────────

static void
paste_contiguous(const Array::Pointer & src, const Array::Pointer & dst, const std::array<ResolvedSlice, 3> & resolved)
{
  const std::array<size_t, 3> region = { resolved[0].length, resolved[1].length, resolved[2].length };

  const std::array<size_t, 3> src_origin = { 0, 0, 0 };
  const std::array<size_t, 3> dst_origin = { resolved[0].start, resolved[1].start, resolved[2].start };

  src->copyTo(dst, region, src_origin, dst_origin);
}


// ── GPU kernel path: arbitrary strides ──────────────────────────────────────

static void
paste_strided(const Array::Pointer & src, const Array::Pointer & dst, const std::array<ResolvedSlice, 3> & resolved)
{
  const size_t src_w = src->width();
  const size_t src_h = src->height();
  const size_t src_d = src->depth();
  const size_t dst_w = dst->width();
  const size_t dst_h = dst->height();
  const size_t dst_d = dst->depth();

  // Replace DTYPE placeholder with actual type
  std::string dtype_str = toString(dst->dtype());
  std::string kernel_code(kernel::paste_strided_kernel);
  size_t      pos = 0;
  while ((pos = kernel_code.find("DTYPE", pos)) != std::string::npos)
  {
    kernel_code.replace(pos, 5, dtype_str);
    pos += dtype_str.length();
  }

  ParameterList parameters;
  parameters.push_back({ "src", src });
  parameters.push_back({ "dst", dst });
  parameters.push_back({ "src_w", static_cast<int>(src_w) });
  parameters.push_back({ "src_h", static_cast<int>(src_h) });
  parameters.push_back({ "src_d", static_cast<int>(src_d) });
  parameters.push_back({ "dst_w", static_cast<int>(dst_w) });
  parameters.push_back({ "dst_h", static_cast<int>(dst_h) });
  parameters.push_back({ "dst_d", static_cast<int>(dst_d) });
  parameters.push_back({ "x_start", static_cast<int>(resolved[0].start) });
  parameters.push_back({ "x_step", static_cast<int>(resolved[0].step) });
  parameters.push_back({ "y_start", static_cast<int>(resolved[1].start) });
  parameters.push_back({ "y_step", static_cast<int>(resolved[1].step) });
  parameters.push_back({ "z_start", static_cast<int>(resolved[2].start) });
  parameters.push_back({ "z_step", static_cast<int>(resolved[2].step) });

  KernelInfo kernel_info = { "paste_strided_kernel", kernel_code.c_str() };
  RangeArray global_range = { src_w, src_h, src_d };
  native_execute(dst->device(), kernel_info, parameters, global_range);
}


// ── public API ─────────────────────────────────────────────────────────────

auto
paste(const Array::Pointer & src, const Array::Pointer & dst, const std::vector<Slice> & slices) -> void
{
  if (!src)
  {
    throw std::invalid_argument("paste(): source array is null.");
  }
  if (!dst)
  {
    throw std::invalid_argument("paste(): destination array is null.");
  }
  if (slices.size() > 3)
  {
    throw std::invalid_argument("paste(): at most 3 slice specs (x, y, z) are supported.");
  }
  if (src->dtype() != dst->dtype())
  {
    throw std::invalid_argument(
      "paste(): source and destination must have the same dtype ("
      + toString(src->dtype()) + " vs " + toString(dst->dtype()) + ").");
  }

  // Resolve slices against the DESTINATION shape
  auto resolved = resolve_slices(dst, slices);

  // Validate: no axis produces zero-length output
  for (int axis = 0; axis < 3; ++axis)
  {
    if (resolved[axis].length == 0)
    {
      throw std::out_of_range("paste(): axis " + std::to_string(axis) + " produces an empty selection.");
    }
  }

  // Validate source shape matches the region
  validate_paste_shape(src, resolved);

  if (is_contiguous(resolved))
  {
    paste_contiguous(src, dst, resolved);
  }
  else
  {
    paste_strided(src, dst, resolved);
  }
}


auto
paste(const Array::Pointer & src, const Array::Pointer & dst, const Slice & x_slice, const Slice & y_slice, const Slice & z_slice)
  -> void
{
  paste(src, dst, { x_slice, y_slice, z_slice });
}


} // namespace cle