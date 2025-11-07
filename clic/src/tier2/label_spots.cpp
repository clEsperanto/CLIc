#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"

#include "utils.hpp"

#include "cle_label_spots_in_x.h"

namespace cle::tier2
{

auto
label_spots_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::LABEL);
  dst->fill(0);

  auto spot_count_in_x = tier1::sum_x_projection_func(device, src, nullptr);
  auto spot_count_in_xy = tier1::sum_y_projection_func(device, spot_count_in_x, nullptr);

  const KernelInfo    kernel = { "label_spots_in_x", kernel::label_spots_in_x };
  const ParameterList params = {
    { "src", src }, { "dst", dst }, { "countX", spot_count_in_x }, { "countXY", spot_count_in_xy }
  };
  const RangeArray range = { 1, dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
pointlist_to_labelled_spots_func(const Device::Pointer & device,
                                 const Array::Pointer &  src,
                                 Array::Pointer          dst) -> Array::Pointer
{
    if (dst == nullptr)
    {
      // Determine the number of dimensions of the pointlist n x ndims x 1
      auto ndims = src->height();
      std::vector<float> max_value(ndims, 0.0f);

      // Determine the maximum in each dimension, it should return a 1 x ndims x 1 array
      auto max = tier1::maximum_x_projection_func(device, src, nullptr);
      max->readTo(max_value.data(), {1, ndims, 1}, {0,0,0});

      int width  = (ndims > 0) ? static_cast<int>(max_value[0]) + 1 : 1;
      int height = (ndims > 1) ? static_cast<int>(max_value[1]) + 1 : 1;
      int depth  = (ndims > 2) ? static_cast<int>(max_value[2]) + 1 : 1;

      // Create destination with given dimensions
      dst = Array::create(  static_cast<size_t>(width),
                            static_cast<size_t>(height),
                            static_cast<size_t>(depth), 
                            src->height(), dType::LABEL, mType::BUFFER, device);
     
    }
    dst->fill(0);

    auto temp1 = Array::create(src->width(), src->height(), 1, 2, dType::FLOAT, mType::BUFFER, device);
    auto temp2 = Array::create(src->width(), src->height(), 1, 2, dType::FLOAT, mType::BUFFER, device);

    tier1::set_ramp_x_func(device, temp1);
    tier1::add_image_and_scalar_func(device, temp1, temp2, 1);
    tier1::write_values_to_positions_func(device, temp2, dst);

    return dst;
}

} // namespace cle::tier2
