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

auto
copy_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);

  if (src->dtype() == dst->dtype())
  {
    // use built-in copy if data types are the same
    src->copyTo(dst);
    return dst;
  }

  if (src->mtype() == mType::IMAGE || dst->mtype() == mType::IMAGE)
  {
    // use image copy kernel if either src or dst is an image
    tier0::create_like(src, dst);
    const KernelInfo    kernel = { "copy", kernel::copy };
    const ParameterList params = { { "src", src }, { "dst", dst } };
    const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
    execute(device, kernel, params, range);
    return dst;    
  }

  // use basic copy kernel for the rest

  auto input_type = toString(src->dtype());
  auto output_type = toString(dst->dtype());
  auto convert = (dst->dtype() == dType::FLOAT) ? "convert_float" : "convert_" + output_type + "_sat";
  auto kernel_name = "copy_" + input_type + "_to_" + output_type;

  std::string kernel_source = "__kernel void " + kernel_name + "(__global const " + input_type + "* restrict src, __global " + output_type + "* restrict dst, const uint n) { const uint gid = get_global_id(0); if (gid < n) dst[gid] = " + convert + "(src[gid]); }";

  // save kernel as kernel.txt for debugging
  // saveFile("kernel.txt", kernel_source);

  const KernelInfo    kernel_info = { kernel_name, kernel_source };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "n", static_cast<unsigned int>(dst->size()) } };
  
  auto max_work_group_size = device->getMaximumWorkGroupSize();
  auto global_work_size = dst->size();
  auto local_work_size = (global_work_size < max_work_group_size) ? global_work_size : max_work_group_size;

  const RangeArray    range = { dst->size(), 1, 1 };
  const RangeArray    local_range = { local_work_size, 1, 1 };
  native_execute(device, kernel_info, params, range, local_range);
  return dst;
}




} // namespace cle::tier1
