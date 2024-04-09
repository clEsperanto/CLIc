#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_erode_box.h"
#include "cle_erode_sphere.h"

namespace cle::tier1
{

auto
erode_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, std::string connectivity)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  KernelInfo kernel = { "erode_box", kernel::erode_box };
  if (connectivity == "sphere")
  {
    kernel = { "erode_sphere", kernel::erode_sphere };
  }
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

// erode_sphere_slice_by_slice_func

} // namespace cle::tier1
