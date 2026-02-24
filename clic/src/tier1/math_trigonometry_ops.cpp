#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

namespace cle::tier1
{

namespace
{

// Native 1D kernel template.
constexpr const char * kernel_template = R"SRC(
{KW} void math_trigo(
  {ADDR}const {SRC_T}* src,
  {ADDR}{DST_T}*       dst,
  const int            size
) {
  const int idx = {GID};
  if (idx >= size) return;
  const float x = (float)src[idx];
  dst[idx] = ({DST_T})({OP});
}
)SRC";

auto
apply_trigonometric_op(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, const std::string & op_expr)
  -> Array::Pointer
{
  tier0::create_like(src, dst);

  const bool        is_opencl = (device->getType() == Device::Type::OPENCL);
  const std::string src_type = toString(src->dtype());
  const std::string dst_type = toString(dst->dtype());

  const std::string source = renderTemplate(kernel_template,
                                            { { "KW", is_opencl ? "__kernel" : "extern \"C\" __global__" },
                                              { "ADDR", is_opencl ? "__global " : "" },
                                              { "SRC_T", src_type },
                                              { "DST_T", dst_type },
                                              { "GID", is_opencl ? "get_global_id(0)" : "blockDim.x * blockIdx.x + threadIdx.x" },
                                              { "OP", op_expr } });

  const int        total_size = static_cast<int>(src->size());
  const size_t     max_local = device->getMaximumWorkGroupSize();
  const size_t     global_padded = ((total_size + max_local - 1) / max_local) * max_local;
  const RangeArray global_range = { global_padded, 1, 1 };
  const RangeArray local_range = { max_local, 1, 1 };

  const KernelInfo    kernel = { "math_trigo", source };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "size", total_size } };
  native_execute(device, kernel, params, global_range, local_range);
  return dst;
}

} // namespace

auto
sin_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "sin(x)");
}

auto
cos_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "cos(x)");
}

auto
tan_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "tan(x)");
}

auto
asin_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "asin(x)");
}

auto
acos_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "acos(x)");
}

auto
atan_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "atan(x)");
}

auto
sinh_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "sinh(x)");
}

auto
cosh_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "cosh(x)");
}

auto
tanh_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "tanh(x)");
}

} // namespace cle::tier1
