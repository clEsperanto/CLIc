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

auto
labels_neighbors_statistics_func(const Device::Pointer &  device,
                                 const Array::Pointer     label,
                                 const std::vector<int> & proximal_distances,
                                 const std::vector<int> & nearest_neighbor_ns,
                                 const std::vector<int> & dilation_radii) -> StatisticsMap
{
  auto nei_stats = compute_neighbors_statistics_per_labels(device, label, proximal_distances, nearest_neighbor_ns, dilation_radii);

  // remove background label statistics (index 0) from all exported vectors
  for (auto & entry : nei_stats)
  {
    auto & vec = entry.second;
    if (!vec.empty())
    {
      vec.erase(vec.begin());
    }
  }

  return nei_stats;
}

auto
statistics_of_labelled_neighbors_func(const Device::Pointer &  device,
                                      const Array::Pointer     label,
                                      const std::vector<int> & proximal_distances,
                                      const std::vector<int> & nearest_neighbor_ns,
                                      const std::vector<int> & dilation_radii) -> StatisticsMap
{
  return labels_neighbors_statistics_func(device, label, proximal_distances, nearest_neighbor_ns, dilation_radii);
}

} // namespace cle::tier3
