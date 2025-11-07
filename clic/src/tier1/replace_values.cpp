#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_replace_value.h"
#include "cle_replace_values.h"

namespace cle::tier1
{

auto
replace_values_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src0, dst);
  const KernelInfo    kernel = { "replace_values", kernel::replace_values };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
replace_value_func(const Device::Pointer & device,
                   const Array::Pointer &  src,
                   Array::Pointer          dst,
                   float                   value_to_replace,
                   float                   value_replacement) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "replace_value", kernel::replace_value };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar0", value_to_replace }, { "scalar1", value_replacement } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
replace_intensity_func(const Device::Pointer & device,
                       const Array::Pointer &  src,
                       Array::Pointer          dst,
                       float                   value_to_replace,
                       float                   value_replacement) -> Array::Pointer
{
  return replace_value_func(device, src, dst, value_to_replace, value_replacement);
}

auto
replace_intensities_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1, Array::Pointer dst)
  -> Array::Pointer
{
  return replace_values_func(device, src0, src1, dst);
}

} // namespace cle::tier1
