#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_superior_inferior_2d.h"
#include "cle_superior_inferior_3d.h"

namespace cle::tier1
{

auto
binary_supinf_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  cle::Array::Pointer in = nullptr;
  if (src->dtype() != dType::BINARY)
  {
    std::cerr << "Warning: Source image of binary_supinf expected to be binary, " << src->dtype() << " given." << std::endl;
    tier0::create_like(src, in, dType::BINARY);
    tier1::copy_func(device, src, in);
  }
  else
  {
    in = src;
  }
  tier0::create_like(src, dst, dType::BINARY);
  const KernelInfo    kernel = in->depth() > 1 ? KernelInfo{ "superior_inferior", kernel::superior_inferior_3d }
                                               : KernelInfo{ "superior_inferior", kernel::superior_inferior_2d };
  const ParameterList params = { { "src", in }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
