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

  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "copy", kernel::copy };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}


} // namespace cle::tier1
