
#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"

#include "utils.hpp"
#include <numeric>

namespace cle::tier4
{

auto
generate_partial_touching_area_matrix_func(const Device::Pointer & device, const Array::Pointer & src_label, Array::Pointer dst_matrix) -> Array::Pointer
{
    auto area_touching_matrix = tier3::generate_touching_area_matrix_func(device, src_label, nullptr);
    tier0::create_dst(area_touching_matrix,
                      dst_matrix,
                      area_touching_matrix->width(),
                      area_touching_matrix->height(),
                      area_touching_matrix->depth(),
                      dType::FLOAT);
    auto vector = tier1::sum_y_projection_func(device, area_touching_matrix, nullptr);
    tier1::divide_images_func(device, area_touching_matrix, vector, dst_matrix);
    return dst_matrix;
}

auto 
generate_touch_portion_matrix_func(const Device::Pointer & device, const Array::Pointer & src_label, Array::Pointer dst_matrix) -> Array::Pointer
{
    return generate_partial_touching_area_matrix_func(device, src_label, dst_matrix);
}


} // namespace cle::tier4