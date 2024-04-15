#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"

#include "utils.hpp"

namespace cle::tier5
{

auto
connected_components_labeling_func(const Device::Pointer & device,
                                   const Array::Pointer &  src,
                                   Array::Pointer          dst,
                                   const std::string &     connectivity) -> Array::Pointer
{
  // type definition of connectivity processing function
  using nonzero_minimum_type = cle::Array::Pointer (*)(
    const cle::Device::Pointer &, const cle::Array::Pointer &, cle::Array::Pointer, cle::Array::Pointer);
  nonzero_minimum_type nonzero_minimum_func = nullptr;
  if (connectivity == "sphere")
  {
    nonzero_minimum_func = tier1::nonzero_minimum_diamond_func;
  }
  else
  {
    nonzero_minimum_func = tier1::nonzero_minimum_box_func;
  }

  tier0::create_like(src, dst, dType::LABEL);

  auto temp1 = tier1::set_nonzero_pixels_to_pixelindex_func(device, src, nullptr, 1);
  auto temp2 = Array::create(temp1);
  temp2->fill(0);

  auto flag = Array::create(1, 1, 1, 1, dType::INT32, mType::BUFFER, device);
  flag->fill(0);

  int flag_value = 1;
  int iteration_count = 0;
  while (flag_value > 0)
  {
    if (iteration_count % 2 == 0)
    {
      nonzero_minimum_func(device, temp1, flag, temp2);
    }
    else
    {
      nonzero_minimum_func(device, temp2, flag, temp1);
    }
    flag->read(&flag_value);
    flag->fill(0);
    iteration_count++;
  }
  if (iteration_count % 2 == 0)
  {
    tier4::relabel_sequential_func(device, temp1, dst, 4096);
  }
  else
  {
    tier4::relabel_sequential_func(device, temp2, dst, 4096);
  }
  return dst;
}

} // namespace cle::tier5
