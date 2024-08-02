#include "tier0.hpp"
#include "tier1.hpp"
#include "tier3.hpp"

namespace cle::tier3
{

auto
statistics_of_labelled_pixels_func(const Device::Pointer & device,
                                   Array::Pointer          label,
                                   Array::Pointer          intensity) -> StatisticsMap
{
  if (!intensity && !label)
  {
    throw std::runtime_error("Error: either intensity or label must be set.");
  }

  // create label if not set
  if (!label)
  {
    std::cerr << "Warning: no label was provided. The image will be treated as one object." << std::endl;
    tier0::create_like(intensity, label, dType::LABEL);
    label->fill(1);
  }

  // create intensity if not set
  if (intensity == nullptr)
  {
    std::cerr << "Warning: no intensity was provided. Pixels intensity will be set to their label value." << std::endl;
    tier0::create_like(label, intensity, dType::FLOAT);
    tier1::copy_func(device, label, intensity);
  }

  return compute_statistics_per_labels(device, label, intensity);
}


} // namespace cle::tier3
