#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
opening_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & footprint, Array::Pointer dst)
  -> Array::Pointer
{
  if (src->dimension() != footprint->dimension())
  {
    throw std::runtime_error("Error: input and structuring element in opening operator must have the same dimensionality.");
  }
  auto temp = tier1::erosion_func(device, src, footprint, nullptr);
  return tier1::dilation_func(device, temp, footprint, dst);
}

auto
binary_opening_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z,
                    std::string             connectivity) -> Array::Pointer
{
  auto temp = tier1::binary_erode_func(device, src, nullptr, radius_x, radius_y, radius_z, connectivity);
  return tier1::binary_dilate_func(device, temp, dst, radius_x, radius_y, radius_z, connectivity);
}


auto
grayscale_opening_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       float                   radius_x,
                       float                   radius_y,
                       float                   radius_z,
                       std::string             connectivity) -> Array::Pointer
{
  auto temp = tier1::minimum_filter_func(device, src, nullptr, radius_x, radius_y, radius_z, connectivity);
  return tier1::maximum_filter_func(device, temp, dst, radius_x, radius_y, radius_z, connectivity);
}

auto
opening_sphere_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z) -> Array::Pointer
{
  return grayscale_opening_func(device, src, dst, radius_x, radius_y, radius_z, "sphere");
}

auto
opening_box_func(const Device::Pointer & device,
                 const Array::Pointer &  src,
                 Array::Pointer          dst,
                 float                   radius_x,
                 float                   radius_y,
                 float                   radius_z) -> Array::Pointer
{
  return grayscale_opening_func(device, src, dst, radius_x, radius_y, radius_z, "box");
}

} // namespace cle::tier2
