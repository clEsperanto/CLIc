
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
                         Array::Pointer          coorindate_list_destination,
                         bool                    include_background) -> Array::Pointer
{
  cle::StatisticsMap props;
  if (include_background)
  {
    props = tier3::statistics_of_background_and_labelled_pixels_func(device, label_image, nullptr);
  }
  else
  {
    props = tier3::statistics_of_labelled_pixels_func(device, label_image, nullptr);
  }
  auto centroid_x = props["centroid_x"];
  auto centroid_y = props["centroid_y"];
  auto centroid_z = props["centroid_z"];
  auto nb_labels = centroid_x.size();

  auto res = Array::create(nb_labels, 3, 1, 1, dType::FLOAT, mType::BUFFER, device);
  res->writeFrom(centroid_x.data(), { nb_labels, 1, 1 }, { 0, 0, 0 });
  res->writeFrom(centroid_y.data(), { nb_labels, 1, 1 }, { 0, 1, 0 });
  res->writeFrom(centroid_z.data(), { nb_labels, 1, 1 }, { 0, 2, 0 });
  return res;
}


} // namespace cle::tier4
