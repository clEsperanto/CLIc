#include "tier0.hpp"
#include "tier1.hpp"
#include "tier3.hpp"

namespace cle::tier3
{

auto
statistics_of_labelled_pixels_func(const Device::Pointer & device,
                                   const Array::Pointer &  label,
                                   Array::Pointer          intensity) -> StatisticsMap
{
  if (intensity == nullptr)
  {
    // std::cerr << "Warning: no intensity was provided. Pixels intensity will be set to their label value." <<
    // std::endl;
    tier0::create_like(label, intensity, dType::FLOAT);
    intensity->fill(0);
  }

  return compute_statistics_per_labels(device, label, intensity);
}


auto
statistics_of_background_and_labelled_pixels_func(const Device::Pointer & device,
                                                  const Array::Pointer &  label,
                                                  const Array::Pointer &  intensity) -> StatisticsMap
{
  auto               temp = tier1::add_image_and_scalar_func(device, label, nullptr, 1);
  auto               props = tier3::statistics_of_labelled_pixels_func(device, temp, intensity);
  std::vector<float> labels = props["label"];
  std::transform(labels.begin(), labels.end(), labels.begin(), [](float label) { return label - 1; });
  props["label"] = labels;
  return props;
}


} // namespace cle::tier3
