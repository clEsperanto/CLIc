#include "tier0.hpp"
#include "tier1.hpp"
#include "tier3.hpp"

namespace cle::tier3
{

auto
statistics_of_labelled_pixels_func(const Device::Pointer & device,
                                   const Array::Pointer &  src,
                                   Array::Pointer          intensity,
                                   bool withBG) -> StatisticsMap
{
  // create intensity if not set
  if (intensity == nullptr)
  {
    // std::cerr << "Warning: no intensity was provided. Pixels intensity will be set to their label value." <<
    // std::endl;
    tier0::create_like(src, intensity, dType::FLOAT);
    intensity->fill(0);
    // tier1::copy_func(device, label, intensity);
  }

  size_t offset = (withBG) ? 0 : 1;
  return compute_statistics_per_labels(device, src, intensity, offset);
}


} // namespace cle::tier3
