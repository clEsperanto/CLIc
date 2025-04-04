#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_replace_value.h"
#include "cle_replace_values.h"

namespace cle::tier1
{

auto
replace_values_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    const Array::Pointer &  values,
                    Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel_code = { "replace_values", kernel::replace_values };
  const ParameterList params = { { "src0", src }, { "src1", values }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
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
  const KernelInfo    kernel_code = { "replace_value", kernel::replace_value };
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "scalar0", value_to_replace }, { "scalar1", value_replacement }
  };
  const RangeArray range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel_code, params, range);
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
replace_intensities_func(const Device::Pointer & device,
                         const Array::Pointer &  src,
                         const Array::Pointer &  intensities,
                         Array::Pointer          dst) -> Array::Pointer
{
  return replace_values_func(device, src, intensities, dst);
}

} // namespace cle::tier1
