#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_laplace_diamond.h"

namespace cle::tier1
{

auto
laplace_diamond_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  std::cerr << "Deprecated: Please use laplace() instead\n";
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "laplace_diamond", kernel::laplace_diamond };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
