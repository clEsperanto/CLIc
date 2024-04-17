#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

#include "cle_flag_existing_labels.h"

namespace cle::tier3
{

auto
flag_existing_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  auto max = tier2::maximum_of_all_pixels_func(device, src);
  tier0::create_vector(src, dst, max + 1, dType::LABEL);
  dst->fill(0);
  const KernelInfo    kernel = { "flag_existing_labels", kernel::flag_existing_labels };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier3
