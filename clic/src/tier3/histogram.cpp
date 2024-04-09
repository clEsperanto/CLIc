#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"

#include "utils.hpp"

#include "cle_histogram.h"

namespace cle::tier3
{

auto
histogram_func(const Device::Pointer & device,
               const Array::Pointer &  src,
               Array::Pointer          dst,
               int                     nbins,
               float                   min,
               float                   max) -> Array::Pointer
{
  tier0::create_vector(src, dst, nbins, dType::INDEX);
  size_t number_of_partial_histograms = src->height();
  auto   partial_hist =
    Array::create(nbins, 1, number_of_partial_histograms, 3, dType::INDEX, src->mtype(), src->device());
  if (std::isnan(max) || std::isnan(max))
  {
    min = tier2::minimum_of_all_pixels_func(device, src);
    max = tier2::maximum_of_all_pixels_func(device, src);
  }
  const KernelInfo    kernel = { "histogram", kernel::histogram };
  const ParameterList params = { { "src", src },       { "dst", partial_hist }, { "minimum", min },  { "maximum", max },
                                 { "step_size_x", 1 }, { "step_size_y", 1 },    { "step_size_z", 1 } };
  const ConstantList  consts = { { "NUMBER_OF_HISTOGRAM_BINS", nbins } };
  const RangeArray    range = { number_of_partial_histograms, 1, 1 };
  execute(device, kernel, params, range, consts);
  return tier1::sum_z_projection_func(device, partial_hist, dst);
}

} // namespace cle::tier3
