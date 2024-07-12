#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_dilate_box.h"
#include "cle_dilate_sphere.h"

namespace cle::tier1
{

auto
dilate_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, std::string connectivity)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  KernelInfo kernel = { "dilate_box", kernel::dilate_box };
  if (connectivity == "sphere")
  {
    kernel = { "dilate_sphere", kernel::dilate_sphere };
  }
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
