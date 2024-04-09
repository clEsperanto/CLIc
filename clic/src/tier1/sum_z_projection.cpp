#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_sum_z_projection.h"

namespace cle::tier1
{

auto
sum_z_projection_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_xy(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "sum_z_projection", kernel::sum_z_projection };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

// superior_inferior(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
// {
//   tier0::create_like(src, dst, dType::UINT8);
//   if (src->dtype() != dType::UINT8)
//   {
//     throw std::runtime_error("inferior_superior only supports UINT8 images");
//   }
//   const KernelInfo    kernel = { "superior_inferior", kernel::superior_inferior };
//   const ParameterList params = { { "src", src }, { "dst", dst } };
//   const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
//   execute(device, kernel, params, range);
//   return dst;
// }

} // namespace cle::tier1
