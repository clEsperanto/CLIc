
#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_maximum_touching_neighbors.h"
#include "cle_mean_touching_neighbors.h"
#include "cle_median_touching_neighbors.h"
#include "cle_minimum_touching_neighbors.h"
#include "cle_mode_touching_neighbors.h"
#include "cle_standard_deviation_touching_neighbors.h"

namespace cle::tier1
{

auto
mean_of_touching_neighbors_func(const Device::Pointer & device,
                                const Array::Pointer &  vector,
                                const Array::Pointer &  matrix,
                                Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(vector, dst, dType::FLOAT);
  int                 x_corr = (matrix->width() == vector->size() + 1) ? -1 : 0;
  const KernelInfo    kernel = { "mean_touching_neighbors", kernel::mean_touching_neighbors };
  const ParameterList params = { { "src_vector", vector }, { "src_matrix", matrix }, { "dst", dst }, { "x_correction", x_corr } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}


auto
median_of_touching_neighbors_func(const Device::Pointer & device,
                                  const Array::Pointer &  vector,
                                  const Array::Pointer &  matrix,
                                  Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(vector, dst, dType::FLOAT);
  int                 x_corr = (matrix->width() == vector->size() + 1) ? -1 : 0;
  const KernelInfo    kernel = { "median_touching_neighbors", kernel::median_touching_neighbors };
  const ParameterList params = { { "src_vector", vector }, { "src_matrix", matrix }, { "dst", dst }, { "x_correction", x_corr } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const ConstantList  constants = { { "MAX_ARRAY_SIZE", 256 } };
  execute(device, kernel, params, range, { 0, 0, 0 }, constants);
  return dst;
}


auto
minimum_of_touching_neighbors_func(const Device::Pointer & device,
                                   const Array::Pointer &  vector,
                                   const Array::Pointer &  matrix,
                                   Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(vector, dst, dType::FLOAT);
  int                 x_corr = (matrix->width() == vector->size() + 1) ? -1 : 0;
  const KernelInfo    kernel = { "minimum_touching_neighbors", kernel::minimum_touching_neighbors };
  const ParameterList params = { { "src_vector", vector }, { "src_matrix", matrix }, { "dst", dst }, { "x_correction", x_corr } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}


auto
maximum_of_touching_neighbors_func(const Device::Pointer & device,
                                   const Array::Pointer &  vector,
                                   const Array::Pointer &  matrix,
                                   Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(vector, dst, dType::FLOAT);
  int                 x_corr = (matrix->width() == vector->size() + 1) ? -1 : 0;
  const KernelInfo    kernel = { "maximum_touching_neighbors", kernel::maximum_touching_neighbors };
  const ParameterList params = { { "src_vector", vector }, { "src_matrix", matrix }, { "dst", dst }, { "x_correction", x_corr } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}


auto
standard_deviation_of_touching_neighbors_func(const Device::Pointer & device,
                                              const Array::Pointer &  vector,
                                              const Array::Pointer &  matrix,
                                              Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(vector, dst, dType::FLOAT);
  int                 x_corr = (matrix->width() == vector->size() + 1) ? -1 : 0;
  const KernelInfo    kernel = { "standard_deviation_touching_neighbors", kernel::standard_deviation_touching_neighbors };
  const ParameterList params = { { "src_vector", vector }, { "src_matrix", matrix }, { "dst", dst }, { "x_correction", x_corr } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}


auto
mode_of_touching_neighbors_func(const Device::Pointer & device,
                                const Array::Pointer &  vector,
                                const Array::Pointer &  matrix,
                                Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(vector, dst, dType::FLOAT);
  int                 x_corr = (matrix->width() == vector->size() + 1) ? -1 : 0;
  const KernelInfo    kernel = { "mode_touching_neighbors", kernel::mode_touching_neighbors };
  const ParameterList params = { { "src_vector", vector }, { "src_matrix", matrix }, { "dst", dst }, { "x_correction", x_corr } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}

} // namespace cle::tier1
