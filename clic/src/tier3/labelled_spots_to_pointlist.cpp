#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

#include "cle_labelled_spots_to_point_list.h"

namespace cle::tier3
{

auto
labelled_spots_to_pointlist_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  auto max_label = tier2::maximum_of_all_pixels_func(device, src);
  auto dim = shape_to_dimension(src->width(), src->height(), src->depth());
  tier0::create_dst(src, dst, max_label, dim, 1, dType::LABEL);
  dst->fill(0);

  const KernelInfo    kernel = { "labelled_spots_to_point_list", kernel::labelled_spots_to_point_list };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier3
