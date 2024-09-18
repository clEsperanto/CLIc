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
remove_small_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float min_size)
  -> Array::Pointer
{
  return tier5::filter_label_by_size_func(device, src, dst, min_size, std::numeric_limits<float>::max());
}

auto
remove_large_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float max_size)
  -> Array::Pointer
{
  return tier5::filter_label_by_size_func(device, src, dst, 0, max_size);
}

auto
exclude_small_labels_func(const Device::Pointer & device,
                          const Array::Pointer &  src,
                          Array::Pointer          dst,
                          float                   max_size) -> Array::Pointer
{
  return remove_small_labels_func(device, src, dst, max_size);
}

auto
exclude_large_labels_func(const Device::Pointer & device,
                          const Array::Pointer &  src,
                          Array::Pointer          dst,
                          float                   min_size) -> Array::Pointer
{
  return remove_large_labels_func(device, src, dst, min_size);
}

} // namespace cle::tier6
