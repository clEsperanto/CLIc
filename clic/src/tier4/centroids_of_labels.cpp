
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"

#include "utils.hpp"
#include <numeric>

namespace cle::tier4
{

auto
centroids_of_labels_func(const Device::Pointer & device,
                         const Array::Pointer &  label_image,
                         Array::Pointer          centroids_coordinates,
                         bool                    include_background) -> Array::Pointer
{
  cle::StatisticsMap props;
  if (include_background)
  {
    props = tier3::statistics_of_background_and_labelled_pixels_func(device, label_image, label_image);
  }
  else
  {
    props = tier3::statistics_of_labelled_pixels_func(device, label_image, label_image);
  }
  auto centroid_x = props["centroid_x"];
  auto centroid_y = props["centroid_y"];
  auto centroid_z = props["centroid_z"];
  auto nb_labels = centroid_x.size();

  if (centroids_coordinates == nullptr)
  {
    centroids_coordinates = Array::create(nb_labels, 3, 1, 1, dType::FLOAT, mType::BUFFER, device);
  }

  if (centroids_coordinates->width() != nb_labels || centroids_coordinates->height() != 3)
  {
    throw std::runtime_error("centroids_of_labels: Provided output array has wrong dimensions." +
                             std::to_string(centroids_coordinates->width()) + "x" +
                             std::to_string(centroids_coordinates->height()) + "x1 instead of " +
                             std::to_string(nb_labels) + "x3x1");
  }

  if (centroids_coordinates->dtype() != dType::FLOAT)
  {
    throw std::runtime_error("centroids_of_labels: Provided output array has wrong data type. Expected dtype==FLOAT.");
  }

  centroids_coordinates->writeFrom(centroid_x.data(), { nb_labels, 1, 1 }, { 0, 0, 0 });
  centroids_coordinates->writeFrom(centroid_y.data(), { nb_labels, 1, 1 }, { 0, 1, 0 });
  centroids_coordinates->writeFrom(centroid_z.data(), { nb_labels, 1, 1 }, { 0, 2, 0 });
  return centroids_coordinates;
}


} // namespace cle::tier4
