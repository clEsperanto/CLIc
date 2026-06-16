#include "tier0.hpp"
#include "tier1.hpp"
#include "tier3.hpp"

namespace cle::tier3
{

auto
labels_statistics_func(const Device::Pointer & device, Array::Pointer label, Array::Pointer intensity, bool include_background)
  -> StatisticsMap
{
  if (intensity == nullptr && label == nullptr)
  {
    throw std::runtime_error("Error: no label map nor intensity image was provided. Please provide at least one of them.");
  }
  if (label == nullptr)
  {
    tier0::create_like(intensity, label, dType::LABEL);
    label->fill(1);
  }
  if (intensity == nullptr)
  {
    tier0::create_like(label, intensity, dType::FLOAT);
    tier1::copy_func(device, label, intensity);
  }

  if (!include_background)
  {
    return compute_statistics_per_labels(device, label, intensity);
  }

  auto               temp = tier1::add_image_and_scalar_func(device, label, nullptr, 1);
  auto               props = compute_statistics_per_labels(device, temp, intensity);
  std::vector<float> labels = props["label"];
  std::transform(labels.begin(), labels.end(), labels.begin(), [](float label) { return label - 1; });
  props["label"] = labels;
  return props;
}

auto
statistics_of_labelled_pixels_func(const Device::Pointer & device, Array::Pointer intensity, Array::Pointer label) -> StatisticsMap
{
  return labels_statistics_func(device, label, intensity, false);
}

auto
statistics_of_background_and_labelled_pixels_func(const Device::Pointer & device, Array::Pointer intensity, Array::Pointer label)
  -> StatisticsMap
{
  return labels_statistics_func(device, label, intensity, true);
}


} // namespace cle::tier3
