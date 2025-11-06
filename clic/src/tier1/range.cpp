#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_range.h"

namespace cle::tier1
{

auto
range_func(const Device::Pointer & device,
           const Array::Pointer &  src,
           Array::Pointer          dst,
           int                     start_x,
           int                     stop_x,
           int                     step_x,
           int                     start_y,
           int                     stop_y,
           int                     step_y,
           int                     start_z,
           int                     stop_z,
           int                     step_z) -> Array::Pointer
{
  tier0::create_dst(src,
                    dst,
                    abs(stop_x - start_x) / std::max(std::abs(step_x), 1),
                    abs(stop_y - start_y) / std::max(std::abs(step_y), 1),
                    abs(stop_z - start_z) / std::max(std::abs(step_z), 1),
                    src->dtype());

  correct_range(&start_x, &stop_x, &step_x, static_cast<int>(src->width()));
  correct_range(&start_y, &stop_y, &step_y, static_cast<int>(src->height()));
  correct_range(&start_z, &stop_z, &step_z, static_cast<int>(src->depth()));

  const KernelInfo    kernel = { "range", kernel::range };
  const ParameterList params = { { "src", src },         { "dst", dst },       { "start_x", start_x }, { "step_x", step_x },
                                 { "start_y", start_y }, { "step_y", step_y }, { "start_z", start_z }, { "step_z", step_z } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

// read_values_from_map_func

} // namespace cle::tier1
