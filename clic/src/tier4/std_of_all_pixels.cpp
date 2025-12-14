#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"

#include "utils.hpp"

namespace cle::tier4
{

namespace
{
  constexpr const char * kernel_source = R"CLC(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;
__kernel void sum_pow(IMAGE_src_TYPE  src, IMAGE_dst_TYPE  dst, const float mean)
{
    int x = get_global_id(0);
    int y = get_global_id(1);
    int z = get_global_id(2);
    const float val = (float) READ_IMAGE(src, sampler, POS_src_INSTANCE(x, y, z, 0)).x;
    float res = pow(val - mean, 2.0f);
    WRITE_IMAGE(dst, POS_dst_INSTANCE(x, y, z, 0), CONVERT_dst_PIXEL_TYPE(res));
}
)CLC";
} // namespace

auto
standard_deviation_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  float mean = tier3::mean_of_all_pixels_func(device, src);
  auto  diff = Array::create(src->width(), src->height(), src->depth(), src->dimension(), dType::FLOAT, src->mtype(), device);
  
  // tier1::add_image_and_scalar_func(device, src, diff, -mean);
  // tier1::power_func(device, diff, diff, 2.0f);
  const KernelInfo    kernel = { "sum_pow", kernel_source };
  const ParameterList params = { { "src", src }, { "dst", diff }, { "mean", mean } };
  const RangeArray    range = { diff->width(), diff->height(), diff->depth() };
  execute(device, kernel, params, range);



  float variance = tier3::mean_of_all_pixels_func(device, diff);
  return std::sqrt(variance);
}

} // namespace cle::tier4
