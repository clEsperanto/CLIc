#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_absolute.h"

namespace cle::tier1
{

auto
absolute_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst);

  std::string      name = "absolute";
  std::string      source = kernel::absolute;
  const KernelInfo kernel = { name, source };

  std::pair<std::string, Array::Pointer> src_arg = { "src", src };
  std::pair<std::string, Array::Pointer> dst_arg = { "dst", dst };
  const ParameterList                    params = { src_arg, dst_arg };

  auto             width = src->width();
  auto             height = src->height();
  auto             depth = src->depth();
  const RangeArray range = { width, height, depth };

  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
