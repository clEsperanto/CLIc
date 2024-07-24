#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"

#include "utils.hpp"

namespace cle::tier5
{

auto
array_equal_func(const Device::Pointer & device, const Array::Pointer & src0, const Array::Pointer & src1) -> bool
{
  // check if size is equal
  if (src0->size() != src1->size())
  {
    return false;
  }
  // check if width, height, depth is equal
  if (src0->width() != src1->width() || src0->height() != src1->height() || src0->depth() != src1->depth())
  {
    return false;
  }
  // check if empty
  if (src0->size() == 0 && src1->size() == 0)
  {
    return true;
  }
  auto mse = tier4::mean_squared_error_func(device, src0, src1);
  return mse == 0;
}

} // namespace cle::tier5
