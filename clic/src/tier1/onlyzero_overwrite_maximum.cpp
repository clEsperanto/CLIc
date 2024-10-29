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
                                Array::Pointer          dst0,
                                Array::Pointer          dst1,
                                std::string             connectivity) -> Array::Pointer
{
  tier0::create_like(src, dst1);
  KernelInfo kernel = { "onlyzero_overwrite_maximum_box", kernel::onlyzero_overwrite_maximum_box };
  if (connectivity == "sphere")
  {
    kernel = { "onlyzero_overwrite_maximum_diamond", kernel::onlyzero_overwrite_maximum_diamond };
  }
  const ParameterList params = { { "src", src }, { "dst0", dst0 }, { "dst1", dst1 } };
  const RangeArray    range = { dst1->width(), dst1->height(), dst1->depth() };
  execute(device, kernel, params, range);
  return dst1;
}

auto
onlyzero_overwrite_maximum_diamond_func(const Device::Pointer & device,
                                        const Array::Pointer &  src,
                                        Array::Pointer          dst0,
                                        Array::Pointer          dst1) -> Array::Pointer
{
  return onlyzero_overwrite_maximum_func(device, src, dst0, dst1, "sphere");
}

auto
onlyzero_overwrite_maximum_box_func(const Device::Pointer & device,
                                    const Array::Pointer &  src,
                                    Array::Pointer          dst0,
                                    Array::Pointer          dst1) -> Array::Pointer
{
  return onlyzero_overwrite_maximum_func(device, src, dst0, dst1, "box");
}

// point_index_list_to_mesh_func
// point_index_list_to_touch_matrix_func

} // namespace cle::tier1
