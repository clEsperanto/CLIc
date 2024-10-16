#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_maximum_image_and_scalar.h"
#include "cle_maximum_images.h"
#include "cle_maximum_separable.h"
#include "cle_maximum_sphere.h"
#include "cle_maximum_x_projection.h"
#include "cle_maximum_y_projection.h"
#include "cle_maximum_z_projection.h"
#include "cle_nonzero_maximum_box.h"
#include "cle_nonzero_maximum_diamond.h"
#include "cle_onlyzero_overwrite_maximum_box.h"
#include "cle_onlyzero_overwrite_maximum_diamond.h"
#include "cle_x_position_of_maximum_x_projection.h"
#include "cle_y_position_of_maximum_y_projection.h"
#include "cle_z_position_of_maximum_z_projection.h"

namespace cle::tier1
{

auto
maximum_filter_func(const Device::Pointer & device,
                    const Array::Pointer &  src,
                    Array::Pointer          dst,
                    float                   radius_x,
                    float                   radius_y,
                    float                   radius_z,
                    std::string             connectivity) -> Array::Pointer
{
  tier0::create_like(src, dst);
  auto r_x = radius2kernelsize(radius_x);
  auto r_y = radius2kernelsize(radius_y);
  auto r_z = radius2kernelsize(radius_z);
  if (connectivity == "sphere")
  {
    const KernelInfo    kernel = { "maximum_sphere", kernel::maximum_sphere };
    const ParameterList params = {
      { "src", src }, { "dst", dst }, { "scalar0", r_x }, { "scalar1", r_y }, { "scalar2", r_z }
    };
    const RangeArray range = { dst->width(), dst->height(), dst->depth() };
    execute(device, kernel, params, range);
  }
  else
  {
    const KernelInfo kernel = { "maximum_separable", kernel::maximum_separable };
    execute_separable(device,
                      kernel,
                      src,
                      dst,
                      { static_cast<float>(radius_x), static_cast<float>(radius_y), static_cast<float>(radius_z) },
                      { r_x, r_y, r_z });
  }
  return dst;
}

} // namespace cle::tier1
