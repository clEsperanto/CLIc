#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

namespace cle::tier2
{

auto
large_hessian_eigenvalue_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  tier1::hessian_eigenvalues_func(device, src, nullptr, nullptr, dst);
  return dst;
}

} // namespace cle::tier2
