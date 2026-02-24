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

// OpenCL kernel for strided slicing (non-contiguous) with type-generic data access (using DTYPE and item_size)
const char* slice_strided_kernel = R"CLC(
__kernel void slice_strided_kernel(
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
    const int z_start, const int z_step,
    const int item_size)
{
    const int ox = get_global_id(0);
    const int oy = get_global_id(1);
    const int oz = get_global_id(2);

    if (ox >= dst_w || oy >= dst_h || oz >= dst_d)
        return;

    // Map output coords → source coords
    const int sx = x_start + ox * x_step;
    const int sy = y_start + oy * y_step;
    const int sz = z_start + oz * z_step;

    // Bounds check (should not happen if slicing is correct)
    if (sx < 0 || sx >= src_w || sy < 0 || sy >= src_h || sz < 0 || sz >= src_d)
        return;

    const long src_offset = ((long)sz * src_h * src_w + (long)sy * src_w + (long)sx);
    const long dst_offset = ((long)oz * dst_h * dst_w + (long)oy * dst_w + (long)ox);

    dst[dst_offset] = src[src_offset];
}
)CLC";

} // namespace kernel

// ── internal: resolve all 3 axes ────────────────────────────────────────────

static auto
resolve_slices(const Array::Pointer &     src,
               const std::vector<Slice> & slices) -> std::array<ResolvedSlice, 3>
{
  const std::array<size_t, 3> src_shape = { src->width(), src->height(), src->depth() };

  std::array<ResolvedSlice, 3> resolved;
  for (size_t axis = 0; axis < 3; ++axis)
  {
    const Slice & s = (axis < slices.size()) ? slices[axis] : Slice(); // default = [:]
    auto [r_start, r_stop, r_step] = s.resolve(src_shape[axis]);
    size_t out_len = s.output_length(src_shape[axis]);

    resolved[axis] = { r_start, r_stop, r_step, out_len, s.is_index };
  }
  return resolved;
}


// ── helper: compute output dimension ────────────────────────────────────────

static auto
compute_output_dim(const std::array<ResolvedSlice, 3> & resolved) -> size_t
{
  size_t dim = 3;
  for (auto & rs : resolved)
  {
    if (rs.is_index)
      --dim;
  }
  return std::max(dim, size_t(1)); // at least 1-D
}


// ── fast path: all steps == 1  →  use region copy ──────────────────────────

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

static auto
slice_contiguous(const Array::Pointer &               src,
                 const std::array<ResolvedSlice, 3> & resolved) -> Array::Pointer
{
  // Source origin
  const std::array<size_t, 3> src_origin = {
    resolved[0].start,
    resolved[1].start,
    resolved[2].start
  };

  // Region to copy
  const std::array<size_t, 3> region = {
    resolved[0].length,
    resolved[1].length,
    resolved[2].length
  };

  // Destination dimensions
  const size_t out_dim = compute_output_dim(resolved);

  // For collapsed dimensions, use size 1; for others, use resolved length
  const size_t out_w = resolved[0].is_index ? 1 : resolved[0].length;
  const size_t out_h = resolved[1].is_index ? 1 : resolved[1].length;
  const size_t out_d = resolved[2].is_index ? 1 : resolved[2].length;

  auto dst = Array::create(out_w, out_h, out_d, out_dim,
                           src->dtype(), src->mtype(), src->device());
  dst->allocate();

  const std::array<size_t, 3> dst_origin = { 0, 0, 0 };

  src->copyTo(dst, region, src_origin, dst_origin);
  return dst;
}


// ── GPU kernel path: arbitrary strides using OpenCL kernel ────────────────────

static auto
slice_strided(const Array::Pointer &               src,
              const std::array<ResolvedSlice, 3> & resolved) -> Array::Pointer
{
  const size_t item = src->itemSize();
  const size_t src_w = src->width();
  const size_t src_h = src->height();
  const size_t src_d = src->depth();

  // Compute output shape
  // For collapsed dimensions, use size 1; for others, use resolved length
  const size_t out_w = resolved[0].is_index ? 1 : resolved[0].length;
  const size_t out_h = resolved[1].is_index ? 1 : resolved[1].length;
  const size_t out_d = resolved[2].is_index ? 1 : resolved[2].length;
  const size_t out_dim = compute_output_dim(resolved);

  // Create and allocate destination array
  auto dst = Array::create(out_w, out_h, out_d, out_dim,
                           src->dtype(), src->mtype(), src->device());
  dst->allocate();

  // Get the source dtype string and replace placeholder in kernel
  std::string dtype_str = toString(src->dtype());
  std::string kernel_code(kernel::slice_strided_kernel);
  size_t pos = 0;
  while ((pos = kernel_code.find("DTYPE", pos)) != std::string::npos)
  {
    kernel_code.replace(pos, 5, dtype_str);
    pos += dtype_str.length();
  }

  // Build parameter list for the kernel
  ParameterList parameters;
  parameters.push_back({ "src", src });
  parameters.push_back({ "dst", dst });
  parameters.push_back({ "src_w", static_cast<int>(src_w) });
  parameters.push_back({ "src_h", static_cast<int>(src_h) });
  parameters.push_back({ "src_d", static_cast<int>(src_d) });
  parameters.push_back({ "dst_w", static_cast<int>(out_w) });
  parameters.push_back({ "dst_h", static_cast<int>(out_h) });
  parameters.push_back({ "dst_d", static_cast<int>(out_d) });
  parameters.push_back({ "x_start", static_cast<int>(resolved[0].start) });
  parameters.push_back({ "x_step", static_cast<int>(resolved[0].step) });
  parameters.push_back({ "y_start", static_cast<int>(resolved[1].start) });
  parameters.push_back({ "y_step", static_cast<int>(resolved[1].step) });
  parameters.push_back({ "z_start", static_cast<int>(resolved[2].start) });
  parameters.push_back({ "z_step", static_cast<int>(resolved[2].step) });
  parameters.push_back({ "item_size", static_cast<int>(item) });

  // Execute kernel on GPU with replaced dtype
  KernelInfo kernel_info = { "slice_strided_kernel", kernel_code.c_str() };
  RangeArray global_range = { out_w, out_h, out_d };
  native_execute(src->device(), kernel_info, parameters, global_range);

  return dst;
}


// ── public API ─────────────────────────────────────────────────────────────

auto
slice(const Array::Pointer & src, const std::vector<Slice> & slices) -> Array::Pointer
{
  if (!src)
  {
    throw std::invalid_argument("slice(): source array is null.");
  }
  if (slices.size() > 3)
  {
    throw std::invalid_argument("slice(): at most 3 slice specs (x, y, z) are supported.");
  }

  auto resolved = resolve_slices(src, slices);

  // Validate: no axis produces zero-length output (unless that's intentional)
  for (int axis = 0; axis < 3; ++axis)
  {
    if (resolved[axis].length == 0)
    {
      throw std::out_of_range("slice(): axis " + std::to_string(axis) +
                              " produces an empty selection.");
    }
  }

  if (is_contiguous(resolved))
  {
    return slice_contiguous(src, resolved);
  }
  else
  {
    return slice_strided(src, resolved);
  }
}


auto
slice(const Array::Pointer & src,
      const Slice &          x_slice,
      const Slice &          y_slice,
      const Slice &          z_slice) -> Array::Pointer
{
  return slice(src, { x_slice, y_slice, z_slice });
}


} // namespace cle