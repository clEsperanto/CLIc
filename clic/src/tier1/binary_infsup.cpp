#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_inferior_superior_2d.h"
#include "cle_inferior_superior_3d.h"

namespace cle::tier1
{

auto
binary_infsup_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  cle::Array::Pointer in = nullptr;
  if (src->dtype() != dType::BINARY)
  {
    std::cerr << "Warning: Source image of binary_infsup expected to be binary, " << src->dtype() << " given."
              << std::endl;
    tier0::create_like(src, in, dType::BINARY);
    tier1::copy_func(device, src, in);
  }
  else
  {
    in = src;
  }
  tier0::create_like(src, dst, dType::BINARY);
  const KernelInfo    kernel = in->depth() > 1 ? KernelInfo{ "inferior_superior", kernel::inferior_superior_3d }
                                               : KernelInfo{ "inferior_superior", kernel::inferior_superior_2d };
  const ParameterList params = { { "src", in }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
