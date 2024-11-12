#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_onlyzero_overwrite_maximum_box.h"
#include "cle_onlyzero_overwrite_maximum_diamond.h"

namespace cle::tier1
{

auto
onlyzero_overwrite_maximum_func(const Device::Pointer & device,
                                const Array::Pointer &  src,
                                Array::Pointer          flag,
                                Array::Pointer          dst,
                                std::string             connectivity) -> Array::Pointer
{
  tier0::create_like(src, dst);
  KernelInfo kernel = { "onlyzero_overwrite_maximum_box", kernel::onlyzero_overwrite_maximum_box };
  if (connectivity == "sphere")
  {
    kernel = { "onlyzero_overwrite_maximum_diamond", kernel::onlyzero_overwrite_maximum_diamond };
  }
  const ParameterList params = { { "src", src }, { "dst0", flag }, { "dst1", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
onlyzero_overwrite_maximum_diamond_func(const Device::Pointer & device,
                                        const Array::Pointer &  src,
                                        Array::Pointer          flag,
                                        Array::Pointer          dst) -> Array::Pointer
{
  return onlyzero_overwrite_maximum_func(device, src, flag, dst, "sphere");
}

auto
onlyzero_overwrite_maximum_box_func(const Device::Pointer & device,
                                    const Array::Pointer &  src,
                                    Array::Pointer          flag,
                                    Array::Pointer          dst) -> Array::Pointer
{
  return onlyzero_overwrite_maximum_func(device, src, flag, dst, "box");
}

// point_index_list_to_mesh_func
// point_index_list_to_touch_matrix_func

} // namespace cle::tier1
