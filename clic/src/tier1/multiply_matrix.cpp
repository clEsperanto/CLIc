#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_multiply_matrix.h"

namespace cle::tier1
{

auto
multiply_matrix_func(const Device::Pointer & device,
                     const Array::Pointer &  matrix1,
                     const Array::Pointer &  matrix2,
                     Array::Pointer          matrix_destination) -> Array::Pointer
{
  tier0::create_dst(matrix1, matrix_destination, matrix2->width(), matrix1->height(), matrix1->depth(), dType::FLOAT);
  const KernelInfo    kernel = { "multiply_matrix", kernel::multiply_matrix };
  const ParameterList params = { { "src0", matrix1 }, { "src1", matrix2 }, { "dst", matrix_destination } };
  const RangeArray range = { matrix_destination->width(), matrix_destination->height(), matrix_destination->depth() };
  execute(device, kernel, params, range);
  return matrix_destination;
}

} // namespace cle::tier1
