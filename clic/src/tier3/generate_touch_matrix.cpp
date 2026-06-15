#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

#include "cle_generate_touch_matrix.h"
#include "cle_generate_touching_area_matrix.h"

namespace cle::tier3
{

auto
generate_touch_matrix_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst_matrix) -> Array::Pointer
{
  if (dst_matrix == nullptr)
  {
    auto max_label = tier2::maximum_of_all_pixels_func(device, src) + 1;
    tier0::create_dst(src, dst_matrix, max_label, max_label, 1, dType::INDEX);
  }
  dst_matrix->fill(0);
  const KernelInfo    kernel = { "generate_touch_matrix", kernel::generate_touch_matrix };
  const ParameterList params = { { "src", src }, { "dst", dst_matrix } };
  const RangeArray    range = { src->width(), src->height(), src->depth() };
  execute(device, kernel, params, range);
  return dst_matrix;
}

auto
generate_touching_area_matrix_func(const Device::Pointer & device, const Array::Pointer & src_label, Array::Pointer dst_matrix) -> Array::Pointer
{
  if (dst_matrix == nullptr)
  {
    auto max_label = tier2::maximum_of_all_pixels_func(device, src_label) + 1;
    tier0::create_dst(src_label, dst_matrix, max_label, max_label, 1, dType::UINT32);
  }
  dst_matrix->fill(0);

  auto temp1 = tier1::copy_func(device,dst_matrix, nullptr);
  auto temp2 = tier1::copy_func(device,dst_matrix, nullptr);

  const KernelInfo    kernel = { "generate_touching_area_matrix", kernel::generate_touching_area_matrix };
  const ParameterList params = { { "src_label", src_label }, { "dst_matrix", temp1 } };
  const RangeArray    range = { src_label->width(), src_label->height(), src_label->depth() };
  execute(device, kernel, params, range);

  tier1::transpose_xy_func(device, temp1, temp2);
  tier2::add_images_func(device, temp1, temp2, dst_matrix);
  return dst_matrix;
}

auto
generate_touch_count_matrix_func(const Device::Pointer & device, const Array::Pointer & src) -> Array::Pointer
{
  return generate_touching_area_matrix_func(device, src, nullptr);
}

} // namespace cle::tier3
