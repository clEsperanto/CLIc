#include "tier0.hpp"
#include "tier1.hpp"
#include "utils.hpp"

#include "cle_minimum_of_masked_pixels_reduction.h"
#include "cle_sum_reduction_x.h"

namespace cle::tier1
{

auto
sum_reduction_x_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int blocksize) -> Array::Pointer
{
  // WARNING: function is not tested for 2D and 3D data. exepecting 1D only for now.
  if (dst == nullptr)
  {
    size_t dst_width = src->width();
    size_t dst_height = src->height();
    size_t dst_depth = src->depth();
    auto   dim = shape_to_dimension(dst_width, dst_height, dst_depth);
    switch (dim)
    {
      case 1:
        dst_width = static_cast<size_t>(src->width() / blocksize);
        break;
      case 2:
        dst_height = static_cast<size_t>(src->height() / blocksize);
        break;
      case 3:
        dst_depth = static_cast<size_t>(src->depth() / blocksize);
        break;
    }
    dst = Array::create(dst_width, dst_height, dst_depth, 1, src->dtype(), src->mtype(), src->device());
  }
  const KernelInfo    kernel = { "sum_reduction_x", kernel::sum_reduction_x };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "index", blocksize } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

auto
minimum_of_masked_pixels_reduction_func(const Device::Pointer & device,
                                        const Array::Pointer &  src,
                                        const Array::Pointer &  mask,
                                        Array::Pointer          reduced_src,
                                        Array::Pointer          reduced_mask) -> Array::Pointer
{
  tier0::create_xy(src, reduced_src);
  tier0::create_xy(mask, reduced_mask);
  const KernelInfo    kernel = { "minimum_of_masked_pixels_reduction", kernel::minimum_of_masked_pixels_reduction };
  const ParameterList params = { { "src", src }, { "mask", mask }, { "dst_src", reduced_src }, { "dst_mask", reduced_mask } };
  const RangeArray    range = { reduced_src->width(), reduced_src->height(), reduced_src->depth() };
  execute(device, kernel, params, range);
  return reduced_src;
}

} // namespace cle::tier1
