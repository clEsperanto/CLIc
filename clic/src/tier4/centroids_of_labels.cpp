
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"

#include "execution.hpp"
#include "utils.hpp"
#include <algorithm>
#include <numeric>

namespace cle::tier4
{

namespace
{

constexpr const char * centroid_kernel_source = R"CLC(
  __constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
                                 CLK_ADDRESS_CLAMP_TO_EDGE |
                                 CLK_FILTER_NEAREST;

  __kernel void centroid_accumulate(
      IMAGE_src_label_TYPE src_label,
      IMAGE_dst_TYPE       dst,
      int                  nb_labels
  )
  {
    const int x = get_global_id(0);
    const int y = get_global_id(1);
    const int z = get_global_id(2);

    const int label = (int) READ_IMAGE(src_label, sampler, POS_src_label_INSTANCE(x, y, z, 0)).x;
    if (label < 0 || label >= nb_labels)
      return;

    atomic_add(&dst[label + 0 * nb_labels], x);
    atomic_add(&dst[label + 1 * nb_labels], y);
    atomic_add(&dst[label + 2 * nb_labels], z);
    atomic_add(&dst[label + 3 * nb_labels], 1);
  }
)CLC";

} // namespace

auto
centroids_of_labels_func(const Device::Pointer & device,
                         const Array::Pointer &  label_image,
                         Array::Pointer          centroids_coordinates,
                         bool                    include_background) -> Array::Pointer
{
  const auto max_label = tier2::maximum_of_all_pixels_func(device, label_image);
  const auto nb_labels = static_cast<size_t>(std::max(max_label, 0.0f)) + 1;

  if (max_label < 0)
  {
    throw std::runtime_error("centroids_of_labels: input label image contains no valid (non-negative) labels.");
  }

  auto accum = Array::create(nb_labels, 4, 1, 2, dType::INT32, mType::BUFFER, device);
  accum->fill(0);

  const KernelInfo    kernel = { "centroid_accumulate", centroid_kernel_source };
  const ParameterList params = {
    { "src_label", label_image }, { "dst", accum }, { "nb_labels", static_cast<int>(nb_labels) }
  };
  const RangeArray range = { label_image->width(), label_image->height(), label_image->depth() };
  execute(device, kernel, params, range);

  std::vector<int> host_accum(nb_labels * 4);
  accum->readTo(host_accum.data());

  const size_t offset = include_background ? 0 : 1;
  const size_t real_labels = (nb_labels > offset) ? (nb_labels - offset) : 0;
  const size_t out_labels = std::max<size_t>(real_labels, 1);

  std::vector<float> centroids(out_labels * 3, 0.0f);
  for (size_t i = 0; i < real_labels; ++i)
  {
    const size_t idx = i + offset;
    const int    count = host_accum[idx + 3 * nb_labels];
    if (count > 0)
    {
      const float inv = 1.0f / static_cast<float>(count);
      centroids[i + 0 * out_labels] = static_cast<float>(host_accum[idx + 0 * nb_labels]) * inv;
      centroids[i + 1 * out_labels] = static_cast<float>(host_accum[idx + 1 * nb_labels]) * inv;
      centroids[i + 2 * out_labels] = static_cast<float>(host_accum[idx + 2 * nb_labels]) * inv;
    }
  }

  if (centroids_coordinates == nullptr)
  {
    centroids_coordinates = Array::create(out_labels, 3, 1, 1, dType::FLOAT, mType::BUFFER, device);
  }

  if (centroids_coordinates->width() != out_labels || centroids_coordinates->height() != 3 ||
      centroids_coordinates->depth() != 1)
  {
    throw std::runtime_error("centroids_of_labels: Provided output array has wrong dimensions." +
                             std::to_string(centroids_coordinates->width()) + "x" + std::to_string(centroids_coordinates->height()) +
                             "x" + std::to_string(centroids_coordinates->depth()) + " instead of " +
                             std::to_string(out_labels) + "x3x1");
  }

  if (centroids_coordinates->dtype() != dType::FLOAT)
  {
    throw std::runtime_error("centroids_of_labels: Provided output array has wrong data type. Expected dtype==FLOAT.");
  }

  centroids_coordinates->writeFrom(centroids.data());
  return centroids_coordinates;
}


} // namespace cle::tier4
