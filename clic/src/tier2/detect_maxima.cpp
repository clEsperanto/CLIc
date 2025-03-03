#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

#include "cle_detect_maxima.h"

namespace cle::tier2
{

auto
detect_maxima_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   float                   radius_x,
                   float                   radius_y,
                   float                   radius_z,
                   std::string             connectivity) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::BINARY);
  auto                temp = tier1::mean_filter_func(device, src, nullptr, radius_x, radius_y, radius_z, connectivity);
  const KernelInfo    kernel_code = { "detect_maxima", kernel::detect_maxima };
  const ParameterList params = { { "src", temp }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

auto
detect_maxima_box_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       float                   radius_x,
                       float                   radius_y,
                       float                   radius_z) -> Array::Pointer
{
  return detect_maxima_func(device, src, dst, radius_x, radius_y, radius_z, "box");
}

} // namespace cle::tier2
