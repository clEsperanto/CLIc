#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

#include "cle_labelled_spots_to_point_list.h"

namespace cle::tier3
{

auto
labelled_spots_to_pointlist_func(const Device::Pointer & device, const Array::Pointer & label, Array::Pointer pointlist) -> Array::Pointer
{
  auto max_label = tier2::maximum_of_all_pixels_func(device, label);
  auto dim = shape_to_dimension(label->width(), label->height(), label->depth());
  tier0::create_dst(label, pointlist, max_label, dim, 1, dType::LABEL);
  pointlist->fill(0);

  const KernelInfo    kernel = { "labelled_spots_to_point_list", kernel::labelled_spots_to_point_list };
  const ParameterList params = { { "src", label }, { "dst", pointlist } };
  const RangeArray    range = { label->width(), label->height(), label->depth() };
  execute(device, kernel, params, range);
  return pointlist;
}

} // namespace cle::tier3
