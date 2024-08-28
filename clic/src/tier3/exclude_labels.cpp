#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

namespace cle::tier3
{

auto
remove_labels_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    const Array::Pointer &  list,
                    Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  if (list->dtype() != dType::LABEL)
  {
    throw std::runtime_error("remove_labels: label list must be of type uint32");
  }
  std::vector<uint32_t> labels_list(list->size());
  list->readTo(labels_list.data());

  labels_list.front() = 0;
  uint32_t count = 1;
  for (int32_t i = 1; i < labels_list.size(); i++)
  {
    if (labels_list[i] == 0)
    {
      labels_list[i] = count;
      count++;
    }
    else
    {
      labels_list[i] = 0;
    }
  }

  auto index_list = Array::create(list->size(), 1, 1, 1, dType::LABEL, mType::BUFFER, src->device());
  index_list->writeFrom(labels_list.data());
  tier1::replace_values_func(device, src, index_list, dst);
  return dst;
}

} // namespace cle::tier3
