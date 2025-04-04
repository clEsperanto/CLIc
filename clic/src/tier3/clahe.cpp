#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

#include "cle_clahe.h"

namespace cle::tier3
{

  auto
  clahe_func(const Device::Pointer & device,
             const Array::Pointer &  src,
             Array::Pointer          dst,
             int                     tile_size,
             float                     clip_limit,
             float                     minimum_intensity,
             float                     maximum_intensity) -> Array::Pointer
{
  tier0::create_like(src, dst);

  if (std::isnan(minimum_intensity) || std::isnan(maximum_intensity))
  {
    minimum_intensity = tier2::minimum_of_all_pixels_func(device, src);
    maximum_intensity = tier2::maximum_of_all_pixels_func(device, src);
  }
  clip_limit = clip_limit * 255;  // top 1% of the histogram for an 8-bit image

  const KernelInfo    kernel = { "clahe", kernel::clahe };
  const ParameterList params = { 
    { "src", src }, 
    { "dst", dst },
    {"tileSize", tile_size},   
    {"clipLimit", clip_limit},
    {"minIntensity", minimum_intensity},
    {"maxIntensity", maximum_intensity}
  };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
