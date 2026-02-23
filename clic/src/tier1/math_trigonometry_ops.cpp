#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

namespace cle::tier1
{

namespace
{
auto
apply_trigonometric_op(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       const std::string &     op_name,
                       const std::string &     op_expr) -> Array::Pointer
{
  tier0::create_like(src, dst);

  const bool        is_opencl = (device->getType() == Device::Type::OPENCL);
  const std::string src_type = toString(src->dtype());
  const std::string dst_type = toString(dst->dtype());
  const std::string addr = is_opencl ? "__global " : "";
  const std::string kw = is_opencl ? "__kernel" : "extern \"C\" __global__";
  const std::string gid = is_opencl ? "get_global_id(0)" : "blockDim.x * blockIdx.x + threadIdx.x";
  const std::string kernel_name = op_name + "_" + src_type + "_" + dst_type;

  const std::string kernel_source =
    kw + " void " + kernel_name + "(\n"
    "  " + addr + "const " + src_type + "* src,\n"
    "  " + addr + dst_type + "* dst,\n"
    "  const int size\n"
    ") {\n"
    "  const int idx = " + gid + ";\n"
    "  if (idx >= size) return;\n"
    "  const float x = (float)src[idx];\n"
    "  dst[idx] = (" + dst_type + ")(" + op_expr + ");\n"
    "}\n";

  const int        total_size = static_cast<int>(src->size());
  const size_t     max_local = device->getMaximumWorkGroupSize();
  const size_t     global_padded = ((total_size + max_local - 1) / max_local) * max_local;
  const RangeArray global_range = { global_padded, 1, 1 };
  const RangeArray local_range = { max_local, 1, 1 };

  const KernelInfo    kernel = { kernel_name, kernel_source };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "size", total_size } };
  native_execute(device, kernel, params, global_range, local_range);
  return dst;
}
} // namespace

auto
sin_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "sin", "sin(x)");
}

auto
cos_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "cos", "cos(x)");
}

auto
tan_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "tan", "tan(x)");
}

auto
asin_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "asin", "asin(x)");
}

auto
acos_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "acos", "acos(x)");
}

auto
atan_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "atan", "atan(x)");
}

auto
sinh_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "sinh", "sinh(x)");
}

auto
cosh_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "cosh", "cosh(x)");
}

auto
tanh_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return apply_trigonometric_op(device, src, dst, "tanh", "tanh(x)");
}

} // namespace cle::tier1
