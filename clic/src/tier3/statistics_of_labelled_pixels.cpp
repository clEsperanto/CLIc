#include "tier0.hpp"
#include "tier1.hpp"
#include "tier3.hpp"

namespace cle::tier3
{

auto
statistics_of_labelled_pixels_func(const Device::Pointer & device, Array::Pointer intensity, Array::Pointer input_labels)
  -> StatisticsMap
{
  if (intensity == nullptr && input_labels == nullptr)
  {
    throw std::runtime_error(
      "Error: no intensity nor label was provided to the 'statistics_of_labelled_pixels' function.");
  }
  if (input_labels == nullptr)
  {
    std::cerr << "Warning: no label was provided. Label will be the entire image." << std::endl;
    tier0::create_like(intensity, input_labels, dType::LABEL);
    input_labels->fill(1);
  }
  if (intensity == nullptr)
  {
    std::cerr << "Warning: no intensity was provided. Pixels intensity will be set to their label value." << std::endl;
    tier0::create_like(input_labels, intensity, dType::FLOAT);
    tier1::copy_func(device, input_labels, intensity);
  }

  return compute_statistics_per_labels(device, input_labels, intensity);
}


auto
statistics_of_background_and_labelled_pixels_func(const Device::Pointer & device,
                                                  Array::Pointer          intensity,
                                                  Array::Pointer          input_labels) -> StatisticsMap
{
  if (intensity == nullptr && input_labels == nullptr)
  {
    throw std::runtime_error(
      "Error: no intensity nor label was provided to the 'statistics_of_labelled_pixels' function.");
  }
  if (input_labels == nullptr)
  {
    std::cerr << "Warning: no label was provided. Label will be the entire image." << std::endl;
    tier0::create_like(intensity, input_labels, dType::LABEL);
    input_labels->fill(1);
  }
  if (intensity == nullptr)
  {
    std::cerr << "Warning: no intensity was provided. Pixels intensity will be set to their label value." << std::endl;
    tier0::create_like(input_labels, intensity, dType::FLOAT);
    tier1::copy_func(device, input_labels, intensity);
  }
  auto               temp = tier1::add_image_and_scalar_func(device, input_labels, nullptr, 1);
  auto               props = tier3::statistics_of_labelled_pixels_func(device, intensity, temp);
  std::vector<float> labels = props["label"];
  std::transform(labels.begin(), labels.end(), labels.begin(), [](float label) { return label - 1; });
  props["label"] = labels;
  return props;
}


} // namespace cle::tier3
