#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_laplace_box.h"
#include "cle_laplace_diamond.h"

namespace cle::tier1
{

auto
laplace_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, std::string connectivity)
  -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  KernelInfo kernel_code = { "laplace_box", kernel::laplace_box };
  if (connectivity == "sphere")
  {
    kernel_code = { "laplace_diamond", kernel::laplace_diamond };
  }
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
  return dst;
}

auto
laplace_box_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return laplace_func(device, src, dst, "box");
}

auto
laplace_diamond_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  return laplace_func(device, src, dst, "sphere");
}

} // namespace cle::tier1
