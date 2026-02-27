#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

// #include "cle_copy_func.h"
// #include "cle_copy_horizontal_slice_from.h"
// #include "cle_copy_horizontal_slice_to.h"
// #include "cle_copy_slice_from.h"
// #include "cle_copy_slice_to.h"
// #include "cle_copy_vertical_slice_from.h"
// #include "cle_copy_vertical_slice_to.h"


namespace cle::tier1
{

namespace kernel
{
  const char * copy = R"CLC(
    __constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void copy(
    IMAGE_src_TYPE  src,
    IMAGE_dst_TYPE  dst
) 
{
  const int x = get_global_id(0);
  const int y = get_global_id(1);
  const int z = get_global_id(2);

  const IMAGE_src_PIXEL_TYPE value = READ_IMAGE(src, sampler, POS_src_INSTANCE(x,y,z,0)).x;
  WRITE_IMAGE(dst, POS_dst_INSTANCE(x,y,z,0), CONVERT_dst_PIXEL_TYPE(value));
})CLC";
} // namespace kernel


// auto
// copy_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
// {
//   tier0::create_like(src, dst);
//   if (src->dtype() == dst->dtype())
//   {
//     src->copyTo(dst);
//   }
//   else
//   {
//     const std::string src_type = toString(src->dtype());
//     const std::string dst_type = toString(dst->dtype());

//     const std::string convert_suffix = (dst->dtype() == dType::FLOAT) ? "" : "_sat";
//     const std::string kernel_name = "copy_cast_" + src_type + "_to_" + dst_type;

//     const std::string kernel_source =
//       "__kernel void " + kernel_name + "("
//       "  __global const " + src_type + "* src,"
//       "  __global "       + dst_type + "* dst,"
//       "  const unsigned long size"
//       ") {\n"
//       "  const size_t idx = get_global_id(0);\n"
//       "  if (idx < size) { dst[idx] = convert_" + dst_type + convert_suffix + "(src[idx]); }\n"
//       "}\n";

//     const size_t     total_size = src->size();
//     const size_t     max_local = device->getMaximumWorkGroupSize();
//     const size_t     global_padded = ((total_size + max_local - 1) / max_local) * max_local;
//     const RangeArray global_range = { global_padded, 1, 1 };
//     const RangeArray local_range = { max_local, 1, 1 };

//     const KernelInfo    kernel = { kernel_name, kernel_source };
//     const ParameterList params = { { "src", src }, { "dst", dst }, { "size", total_size } };
//     native_execute(device, kernel, params, global_range, local_range);
//   }
//   return dst;
// }

auto
copy_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "copy", kernel::copy };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
