#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

#include "cle_read_map_values.h"

namespace cle::tier1
{   

auto
read_map_values_func(const Device::Pointer & device, const Array::Pointer & label, const Array::Pointer & map, Array::Pointer & dst) -> Array::Pointer
{
  auto max_label = tier2::maximum_of_all_pixels_func(device, label);
  tier0::create_vector(label, dst, max_label, dType::FLOAT);
  dst->fill(0.0f);

  const KernelInfo    kernel = { "read_map_values", kernel::read_map_values };
  const ParameterList params = { { "src0", label }, { "src1", map }, { "dst", dst } };
  const RangeArray    range = { label->width(), label->height(), label->depth() };
  execute(device, kernel, params, range);    

  return dst;
}

auto
read_intensities_from_map_func(const Device::Pointer & device, const Array::Pointer & map, Array::Pointer & dst) -> Array::Pointer
{
  return read_map_values_func(device, map, map, dst);
}

} // namespace cle::tier1