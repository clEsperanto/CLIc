#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_onlyzero_overwrite_maximum_diamond.h"

namespace cle::tier1
{

auto
onlyzero_overwrite_maximum_diamond_func(const Device::Pointer & device,
                                        const Array::Pointer &  src,
                                        Array::Pointer          dst0,
                                        Array::Pointer          dst1) -> Array::Pointer
{
  std::cerr << "Deprecated: Please use onlyzero_overwrite_maximum() instead\n";
  tier0::create_like(src, dst1);
  const KernelInfo    kernel = { "onlyzero_overwrite_maximum_diamond", kernel::onlyzero_overwrite_maximum_diamond };
  const ParameterList params = { { "src", src }, { "dst0", dst0 }, { "dst1", dst1 } };
  const RangeArray    range = { dst1->width(), dst1->height(), dst1->depth() };
  execute(device, kernel, params, range);
  return dst1;
}

} // namespace cle::tier1
