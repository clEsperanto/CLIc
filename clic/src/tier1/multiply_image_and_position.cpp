#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_multiply_image_and_position.h"

namespace cle::tier1
{

auto
multiply_image_and_position_func(const Device::Pointer & device,
                                 const Array::Pointer &  src,
                                 Array::Pointer          dst,
                                 int                     dimension) -> Array::Pointer
{
  auto type = src->dtype();
  switch (src->dtype())
  {
    case dType::INT8:
    case dType::INT16:
      type = dType::INT32;
      break;
    case dType::UINT8:
    case dType::UINT16:
      type = dType::UINT32;
      break;
    default:
      break;
  }
  tier0::create_like(src, dst, type);
  const KernelInfo    kernel = { "multiply_image_and_position", kernel::multiply_image_and_position };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", dimension } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
