#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

#include "cle_labelled_spots_to_point_list.h"

namespace cle::tier3
{

auto
labelled_spots_to_pointlist_func(const Device::Pointer & device,
                                 const Array::Pointer &  input_labels,
                                 Array::Pointer          pointlist) -> Array::Pointer
{
  auto max_label = tier2::maximum_of_all_pixels_func(device, input_labels);
  auto dim = shape_to_dimension(input_labels->width(), input_labels->height(), input_labels->depth());
  tier0::create_dst(input_labels, pointlist, max_label, dim, 1, dType::LABEL);
  pointlist->fill(0);

  const KernelInfo    kernel_code = { "labelled_spots_to_point_list", kernel::labelled_spots_to_point_list };
  const ParameterList params = { { "src", input_labels }, { "dst", pointlist } };
  const RangeArray    range = { input_labels->width(), input_labels->height(), input_labels->depth() };
  execute(device, kernel_code, params, range);
  return pointlist;
}

} // namespace cle::tier3
