#include "tier1.hpp"
#include "utils.hpp"

#include "cle_sum_reduction_x.h"

#include <algorithm>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace kernel
{

// Reduces the flattened image into one partial value per work-item: work-item g combines the pixels g, g + stride,
// g + 2 * stride, ... so that neighbouring work-items read neighbouring pixels. The operation is selected with the
// REDUCTION_OPERATION constant: 0 = sum, 1 = product, 2 = minimum, 3 = maximum. Partials are float, as the result.
constexpr const char * all_pixels_reduction = R"CLC(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void all_pixels_reduction(
    IMAGE_src_TYPE src,
    IMAGE_dst_TYPE dst,
    const int      stride
)
{
  const int index = get_global_id(0);
  const int width = GET_IMAGE_WIDTH(src);
  const int plane = width * GET_IMAGE_HEIGHT(src);
  const int total = plane * GET_IMAGE_DEPTH(src);

  float result = 0;
  for (int i = index; i < total; i += stride)
  {
    const int z = i / plane;
    const int y = (i - z * plane) / width;
    const int x = i - z * plane - y * width;
    const float value = (float) READ_IMAGE(src, sampler, POS_src_INSTANCE(x, y, z, 0)).x;
#if REDUCTION_OPERATION == 0
    result = (i == index) ? value : result + value;
#elif REDUCTION_OPERATION == 1
    result = (i == index) ? value : result * value;
#elif REDUCTION_OPERATION == 2
    result = (i == index || value < result) ? value : result;
#else
    result = (i == index || value > result) ? value : result;
#endif
  }
  WRITE_IMAGE(dst, POS_dst_INSTANCE(index, 0, 0, 0), result);
}
)CLC";

// Same partial scheme, minimum of the pixels where the mask is non-zero (FLT_MAX where there is none)
constexpr const char * masked_minimum_reduction = R"CLC(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void masked_minimum_reduction(
    IMAGE_src_TYPE  src,
    IMAGE_mask_TYPE mask,
    IMAGE_dst_TYPE  dst,
    const int       stride
)
{
  const int index = get_global_id(0);
  const int width = GET_IMAGE_WIDTH(src);
  const int plane = width * GET_IMAGE_HEIGHT(src);
  const int total = plane * GET_IMAGE_DEPTH(src);

  float result = FLT_MAX;
  for (int i = index; i < total; i += stride)
  {
    const int z = i / plane;
    const int y = (i - z * plane) / width;
    const int x = i - z * plane - y * width;
    if ((float) READ_IMAGE(mask, sampler, POS_mask_INSTANCE(x, y, z, 0)).x != 0)
    {
      const float value = (float) READ_IMAGE(src, sampler, POS_src_INSTANCE(x, y, z, 0)).x;
      result = (value < result) ? value : result;
    }
  }
  WRITE_IMAGE(dst, POS_dst_INSTANCE(index, 0, 0, 0), result);
}
)CLC";

} // namespace kernel

namespace cle::tier1
{

namespace
{

// Upper bound on the number of partials: enough work-items to fill the device, and at most 256 KB to read back
constexpr size_t max_partials = 65536;

enum class Reduction : int
{
  SUM = 0,
  PRODUCT = 1,
  MINIMUM = 2,
  MAXIMUM = 3
};

// Runs a partial reduction kernel (arguments: the inputs, then dst and stride) and returns the partials.
// Every partial combines at least one pixel: there are at most as many partials as pixels.
auto
partial_reduction(const Device::Pointer & device,
                  const KernelInfo &      kernel,
                  ParameterList           parameters,
                  const ConstantList &    constants,
                  const Array::Pointer &  src) -> std::vector<float>
{
  if (src->size() > static_cast<size_t>(std::numeric_limits<int>::max()))
  {
    throw std::invalid_argument("Error: reductions are limited to images of less than 2^31 pixels.");
  }
  const size_t nb_partials = std::min(src->size(), max_partials);
  auto         partials = Array::create(nb_partials, 1, 1, 1, dType::FLOAT, mType::BUFFER, device);
  parameters.push_back({ "dst", partials });
  parameters.push_back({ "stride", static_cast<int>(nb_partials) });
  execute(device, kernel, parameters, { nb_partials, 1, 1 }, { 0, 0, 0 }, constants);

  std::vector<float> values(nb_partials);
  partials->readTo(values.data());
  return values;
}

auto
all_pixels_reduction(const Device::Pointer & device, const Array::Pointer & src, Reduction operation) -> std::vector<float>
{
  Array::check_ptr(src, "Error: 'src' is null. Please ensure the provided parameters before calling this function.");
  return partial_reduction(device,
                           { "all_pixels_reduction", kernel::all_pixels_reduction },
                           { { "src", src } },
                           { { "REDUCTION_OPERATION", static_cast<int>(operation) } },
                           src);
}

} // namespace

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
sum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  const auto partials = all_pixels_reduction(device, src, Reduction::SUM);
  return static_cast<float>(std::accumulate(partials.begin(), partials.end(), 0.0));
}

auto
product_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  const auto partials = all_pixels_reduction(device, src, Reduction::PRODUCT);
  return static_cast<float>(std::accumulate(partials.begin(), partials.end(), 1.0, std::multiplies<>()));
}

auto
maximum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  const auto partials = all_pixels_reduction(device, src, Reduction::MAXIMUM);
  return *std::max_element(partials.begin(), partials.end());
}

auto
minimum_of_all_pixels_func(const Device::Pointer & device, const Array::Pointer & src) -> float
{
  const auto partials = all_pixels_reduction(device, src, Reduction::MINIMUM);
  return *std::min_element(partials.begin(), partials.end());
}

auto
minimum_of_masked_pixels_func(const Device::Pointer & device, const Array::Pointer & src, const Array::Pointer & mask) -> float
{
  Array::check_ptr(src, "Error: 'src' is null. Please ensure the provided parameters before calling this function.");
  Array::check_ptr(mask, "Error: 'mask' is null. Please ensure the provided parameters before calling this function.");
  if (mask->width() != src->width() || mask->height() != src->height() || mask->depth() != src->depth())
  {
    throw std::invalid_argument("Error: 'src' and 'mask' must have the same shape.");
  }
  // FLT_MAX when the mask is empty
  const auto partials = partial_reduction(
    device, { "masked_minimum_reduction", kernel::masked_minimum_reduction }, { { "src", src }, { "mask", mask } }, {}, src);
  return *std::min_element(partials.begin(), partials.end());
}

} // namespace cle::tier1
