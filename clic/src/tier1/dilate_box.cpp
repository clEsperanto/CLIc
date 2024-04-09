#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_dilate_box.h"

namespace cle::tier1
{

auto
dilate_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  std::cerr << "Deprecated: Please use dilate() instead\n";
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "dilate_box", kernel::dilate_box };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
