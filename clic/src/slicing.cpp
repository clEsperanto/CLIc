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

// Note: DTYPE_PLACEHOLDER is used instead of bare DTYPE to avoid accidental
// partial-match issues during string replacement.
const char * slice_strided_kernel = R"CLC(
__kernel void slice_strided_kernel(
    __global const DTYPE_PLACEHOLDER * src,
    __global       DTYPE_PLACEHOLDER * dst,
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
    const int ox = get_global_id(0);
    const int oy = get_global_id(1);
    const int oz = get_global_id(2);

    if (ox >= dst_w || oy >= dst_h || oz >= dst_d)
        return;

    const int sx = x_start + ox * x_step;
    const int sy = y_start + oy * y_step;
    const int sz = z_start + oz * z_step;

    if (sx < 0 || sx >= src_w || sy < 0 || sy >= src_h || sz < 0 || sz >= src_d)
        return;

    const long src_offset = ((long)sz * src_h * src_w + (long)sy * src_w + (long)sx);
    const long dst_offset = ((long)oz * dst_h * dst_w + (long)oy * dst_w + (long)ox);

    dst[dst_offset] = src[src_offset];
}
)CLC";

const char * paste_strided_kernel = R"CLC(
__kernel void paste_strided_kernel(
    __global const DTYPE_PLACEHOLDER * src,
    __global       DTYPE_PLACEHOLDER * dst,
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


// ── internal helpers ────────────────────────────────────────────────────────

static const std::string DTYPE_TOKEN = "DTYPE_PLACEHOLDER";

static auto
replace_dtype(const char * kernel_src, const std::string & dtype_str) -> std::string
{
  std::string code(kernel_src);
  size_t      pos = 0;
  while ((pos = code.find(DTYPE_TOKEN, pos)) != std::string::npos)
  {
    code.replace(pos, DTYPE_TOKEN.length(), dtype_str);
    pos += dtype_str.length();
  }
  return code;
}

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
compute_output_dim(const std::array<ResolvedSlice, 3> & resolved) -> size_t
{
  size_t dim = 3;
  for (auto & rs : resolved)
  {
    if (rs.is_index)
      --dim;
  }
  return std::max(dim, size_t(1));
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

/** @brief Compute the 3-D shape of the output array (before dimension collapsing).
 *  Collapsed axes get size 1. */
static auto
output_shape(const std::array<ResolvedSlice, 3> & resolved) -> std::array<size_t, 3>
{
  return {
    resolved[0].is_index ? size_t(1) : resolved[0].length,
    resolved[1].is_index ? size_t(1) : resolved[1].length,
    resolved[2].is_index ? size_t(1) : resolved[2].length,
  };
}


// ── slice: contiguous fast path ─────────────────────────────────────────────

static auto
slice_contiguous(const Array::Pointer & src, const std::array<ResolvedSlice, 3> & resolved) -> Array::Pointer
{
  const std::array<size_t, 3> src_origin = {
    static_cast<size_t>(resolved[0].start),
    static_cast<size_t>(resolved[1].start),
    static_cast<size_t>(resolved[2].start)
  };
  const std::array<size_t, 3> region = { resolved[0].length, resolved[1].length, resolved[2].length };

  const auto [out_w, out_h, out_d] = output_shape(resolved);
  const size_t out_dim = compute_output_dim(resolved);

  auto dst = Array::create(out_w, out_h, out_d, out_dim, src->dtype(), src->mtype(), src->device());
  dst->allocate();

  const std::array<size_t, 3> dst_origin = { 0, 0, 0 };
  src->copyTo(dst, region, src_origin, dst_origin);
  return dst;
}


// ── slice: strided GPU kernel path ──────────────────────────────────────────

static auto
slice_strided(const Array::Pointer & src, const std::array<ResolvedSlice, 3> & resolved) -> Array::Pointer
{
  const auto [out_w, out_h, out_d] = output_shape(resolved);
  const size_t out_dim = compute_output_dim(resolved);

  auto dst = Array::create(out_w, out_h, out_d, out_dim, src->dtype(), src->mtype(), src->device());
  dst->allocate();

  std::string kernel_code = replace_dtype(kernel::slice_strided_kernel, toString(src->dtype()));

  ParameterList parameters;
  parameters.push_back({ "src", src });
  parameters.push_back({ "dst", dst });
  parameters.push_back({ "src_w", static_cast<int>(src->width()) });
  parameters.push_back({ "src_h", static_cast<int>(src->height()) });
  parameters.push_back({ "src_d", static_cast<int>(src->depth()) });
  parameters.push_back({ "dst_w", static_cast<int>(out_w) });
  parameters.push_back({ "dst_h", static_cast<int>(out_h) });
  parameters.push_back({ "dst_d", static_cast<int>(out_d) });
  parameters.push_back({ "x_start", resolved[0].start });
  parameters.push_back({ "x_step",  resolved[0].step });
  parameters.push_back({ "y_start", resolved[1].start });
  parameters.push_back({ "y_step",  resolved[1].step });
  parameters.push_back({ "z_start", resolved[2].start });
  parameters.push_back({ "z_step",  resolved[2].step });

  KernelInfo kernel_info = { "slice_strided_kernel", kernel_code.c_str() };
  RangeArray global_range = { out_w, out_h, out_d };
  native_execute(src->device(), kernel_info, parameters, global_range);

  return dst;
}


// ── slice: public API ───────────────────────────────────────────────────────

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

  for (int axis = 0; axis < 3; ++axis)
  {
    if (resolved[axis].length == 0)
    {
      throw std::out_of_range("slice(): axis " + std::to_string(axis) + " produces an empty selection.");
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
slice(const Array::Pointer & src, const Slice & x_slice, const Slice & y_slice, const Slice & z_slice) -> Array::Pointer
{
  return slice(src, std::vector<Slice>{ x_slice, y_slice, z_slice });
}


// ── paste: validation ───────────────────────────────────────────────────────

static void
validate_paste_shape(const Array::Pointer & src, const std::array<ResolvedSlice, 3> & resolved)
{
  const auto [exp_w, exp_h, exp_d] = output_shape(resolved);

  if (src->width() != exp_w || src->height() != exp_h || src->depth() != exp_d)
  {
    throw std::invalid_argument(
      "paste(): source shape (" + std::to_string(src->width()) + ", " +
      std::to_string(src->height()) + ", " + std::to_string(src->depth()) +
      ") does not match the target region (" + std::to_string(exp_w) + ", " +
      std::to_string(exp_h) + ", " + std::to_string(exp_d) + ").");
  }
}


// ── paste: contiguous fast path ─────────────────────────────────────────────

static void
paste_contiguous(const Array::Pointer & src, const Array::Pointer & dst, const std::array<ResolvedSlice, 3> & resolved)
{
  const std::array<size_t, 3> region = { resolved[0].length, resolved[1].length, resolved[2].length };
  const std::array<size_t, 3> src_origin = { 0, 0, 0 };
  const std::array<size_t, 3> dst_origin = {
    static_cast<size_t>(resolved[0].start),
    static_cast<size_t>(resolved[1].start),
    static_cast<size_t>(resolved[2].start)
  };

  src->copyTo(dst, region, src_origin, dst_origin);
}


// ── paste: strided GPU kernel path ──────────────────────────────────────────

static void
paste_strided(const Array::Pointer & src, const Array::Pointer & dst, const std::array<ResolvedSlice, 3> & resolved)
{
  std::string kernel_code = replace_dtype(kernel::paste_strided_kernel, toString(dst->dtype()));

  const size_t src_w = src->width();
  const size_t src_h = src->height();
  const size_t src_d = src->depth();

  ParameterList parameters;
  parameters.push_back({ "src", src });
  parameters.push_back({ "dst", dst });
  parameters.push_back({ "src_w", static_cast<int>(src_w) });
  parameters.push_back({ "src_h", static_cast<int>(src_h) });
  parameters.push_back({ "src_d", static_cast<int>(src_d) });
  parameters.push_back({ "dst_w", static_cast<int>(dst->width()) });
  parameters.push_back({ "dst_h", static_cast<int>(dst->height()) });
  parameters.push_back({ "dst_d", static_cast<int>(dst->depth()) });
  parameters.push_back({ "x_start", resolved[0].start });
  parameters.push_back({ "x_step",  resolved[0].step });
  parameters.push_back({ "y_start", resolved[1].start });
  parameters.push_back({ "y_step",  resolved[1].step });
  parameters.push_back({ "z_start", resolved[2].start });
  parameters.push_back({ "z_step",  resolved[2].step });

  KernelInfo kernel_info = { "paste_strided_kernel", kernel_code.c_str() };
  RangeArray global_range = { src_w, src_h, src_d };
  native_execute(dst->device(), kernel_info, parameters, global_range);
}


// ── paste: public API ───────────────────────────────────────────────────────

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
      "paste(): source and destination must have the same dtype (" +
      toString(src->dtype()) + " vs " + toString(dst->dtype()) + ").");
  }

  auto resolved = resolve_slices(dst, slices);

  for (int axis = 0; axis < 3; ++axis)
  {
    if (resolved[axis].length == 0)
    {
      throw std::out_of_range("paste(): axis " + std::to_string(axis) + " produces an empty selection.");
    }
  }

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
paste(const Array::Pointer & src, const Array::Pointer & dst,
      const Slice & x_slice, const Slice & y_slice, const Slice & z_slice) -> void
{
  paste(src, dst, std::vector<Slice>{ x_slice, y_slice, z_slice });
}

} // namespace cle