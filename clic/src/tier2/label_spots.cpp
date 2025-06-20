#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

#include "cle_label_spots_in_x.h"

namespace cle::tier2
{

auto
label_spots_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  dst->fill(0);

  auto spot_count_in_x = tier1::sum_x_projection_func(device, src, nullptr);
  auto spot_count_in_xy = tier1::sum_y_projection_func(device, spot_count_in_x, nullptr);

  const KernelInfo    kernel_code = { "label_spots_in_x", kernel::label_spots_in_x };
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "countX", spot_count_in_x }, { "countXY", spot_count_in_xy }
  };
  const RangeArray range = { 1, dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

} // namespace cle::tier2
