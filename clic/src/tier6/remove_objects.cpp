#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"

#include "utils.hpp"
#include <numeric>

namespace cle::tier6
{

auto
remove_small_labels_func(const Device::Pointer & device,
                         const Array::Pointer &  input_labels,
                         Array::Pointer          dst,
                         float                   minimum_size) -> Array::Pointer
{
  return tier5::filter_label_by_size_func(device, input_labels, dst, minimum_size, std::numeric_limits<float>::max());
}

auto
remove_large_labels_func(const Device::Pointer & device,
                         const Array::Pointer &  input_labels,
                         Array::Pointer          dst,
                         float                   maximum_size) -> Array::Pointer
{
  return tier5::filter_label_by_size_func(device, input_labels, dst, 0, maximum_size);
}

auto
exclude_small_labels_func(const Device::Pointer & device,
                          const Array::Pointer &  input_labels,
                          Array::Pointer          dst,
                          float                   maximum_size) -> Array::Pointer
{
  return remove_small_labels_func(device, input_labels, dst, maximum_size);
}

auto
exclude_large_labels_func(const Device::Pointer & device,
                          const Array::Pointer &  input_labels,
                          Array::Pointer          dst,
                          float                   minimum_size) -> Array::Pointer
{
  return remove_large_labels_func(device, input_labels, dst, minimum_size);
}

} // namespace cle::tier6
