#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_multiply_matrix.h"

namespace cle::tier1
{

namespace
{
auto
suggest_tile_size(const Device::Pointer & device) -> int
{
  auto local_mem = device->getLocalMemorySize();
  auto max_work_group = device->getMaximumWorkGroupSize();

  // 2 tiles (src0 + src1) × 4 bytes per float
  int max_tile_from_memory = sqrt(local_mem / (2 * 4));
  // TILE_SIZE² work-items needed, so sqrt(max_work_group)
  int max_tile_from_workgroup = sqrt(max_work_group);
  // Take the minimum of both constraints
  int suggested = (max_tile_from_memory < max_tile_from_workgroup) ? max_tile_from_memory : max_tile_from_workgroup;
  // Round down to multiple of 4 for efficiency
  suggested = (suggested / 4) * 4;

  // Clamp between 4 and 32
  if (suggested > 32)
    suggested = 32;
  if (suggested < 4)
    suggested = 4;

  return suggested;
}
} // namespace

auto
multiply_matrix_func(const Device::Pointer & device,
                     const Array::Pointer &  matrix1,
                     const Array::Pointer &  matrix2,
                     Array::Pointer          matrix_destination) -> Array::Pointer
{
  if (matrix1->dim() > 2 || matrix2->dim() > 2)
  {
    std::cerr << "Warning: multiply_matrix expected 2D arrays but got " << matrix1->dim() << " and " << matrix2->dim() << std::endl;
  }

  if (matrix1->width() != matrix2->height())
  {
    std::cerr << "Warning: matrix dimensions are not compatible for multiplication, we expect (k,n?),(m?,k)->(m?,n?) but got ("
              << matrix1->width() << "," << matrix1->height() << "),(" << matrix2->width() << "," << matrix2->height() << ")" << std::endl;
  }

  tier0::create_dst(matrix1, matrix_destination, matrix2->width(), matrix1->height(), 1, dType::FLOAT);

  
  int TILE_SIZE = static_cast<int>(suggest_tile_size(device));
  const KernelInfo    kernel = { "multiply_matrix", kernel::multiply_matrix };
  const ParameterList params = { { "src0", matrix1 }, { "src1", matrix2 }, { "dst", matrix_destination } };
  RangeArray          range = { 
    ((matrix_destination->width() + TILE_SIZE - 1) / TILE_SIZE) * TILE_SIZE,
    ((matrix_destination->height() + TILE_SIZE - 1) / TILE_SIZE) * TILE_SIZE,
    1 };
  RangeArray   local = { static_cast<size_t>(TILE_SIZE), static_cast<size_t>(TILE_SIZE), 1 };
  ConstantList constants = { { "TILE_SIZE", TILE_SIZE } };
  try
  {
    execute(device, kernel, params, range, local, constants);
  }
  catch (const std::runtime_error & e)
  {
    /// Fallback to TILE_SIZE = 1 no optimization
    // std::cerr << "Warning: multiply_matrix kernel execution failed with TILE_SIZE=" << TILE_SIZE << ". Fall back to TILE_SIZE=1.\n"
    //           << "Original error: " << e.what() << std::endl;
    range = { static_cast<size_t>(matrix_destination->width()), static_cast<size_t>(matrix_destination->height()), 1 };
    local = { 1, 1, 1 };
    constants = { { "TILE_SIZE", 1 } };
    execute(device, kernel, params, range, local, constants);
  }
  return matrix_destination;
}

} // namespace cle::tier1

