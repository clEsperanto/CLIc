#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

namespace cle::tier3
{

auto
remove_labels_func(const Device::Pointer & device,
                   const Array::Pointer &  input_labels,
                   const Array::Pointer &  label_list,
                   Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(input_labels, dst);
  if (label_list->dtype() != dType::LABEL)
  {
    throw std::runtime_error("remove_labels: label list must be of type uint32");
  }
  std::vector<uint32_t> tmp_list(label_list->size());
  label_list->readTo(tmp_list.data());

  tmp_list.front() = 0;
  uint32_t count = 1;
  for (int32_t i = 1; i < tmp_list.size(); i++)
  {
    if (tmp_list[i] == 0)
    {
      tmp_list[i] = count;
      count++;
    }
    else
    {
      tmp_list[i] = 0;
    }
  }

  auto index_list = Array::create(label_list->size(), 1, 1, 1, dType::LABEL, mType::BUFFER, input_labels->device());
  index_list->writeFrom(tmp_list.data());

  tier1::replace_values_func(device, input_labels, index_list, dst);
  return dst;
}

auto
exclude_labels_func(const Device::Pointer & device,
                    const Array::Pointer &  input_labels,
                    const Array::Pointer &  list,
                    Array::Pointer          dst) -> Array::Pointer
{
  return remove_labels_func(device, input_labels, list, dst);
}

} // namespace cle::tier3
