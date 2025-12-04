#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_multiply_matrix.h"

namespace cle::tier1
{

  namespace
  {
    constexpr const char * alt_kernel = R"CLC(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void multiply_matrix(
    IMAGE_src0_TYPE  src0,
    IMAGE_src1_TYPE  src1,
    IMAGE_dst_TYPE   dst
) 
{
  const int x = get_global_id(0);
  const int y = get_global_id(1);
  const int local_x = get_local_id(0);
  const int local_y = get_local_id(1);
  
  const int src0_width = GET_IMAGE_WIDTH(src0);
  
  __local float tile_src0[TILE_SIZE][TILE_SIZE];
  __local float tile_src1[TILE_SIZE][TILE_SIZE];
  
  float sum = 0;
  
  // Process matrix in tiles
  for (int tile = 0; tile < (src0_width + TILE_SIZE - 1) / TILE_SIZE; ++tile) {
      // Load tiles into local memory
      int tile_col = tile * TILE_SIZE + local_x;
      int tile_row = tile * TILE_SIZE + local_y;
      
      if (tile_col < src0_width && y < GET_IMAGE_HEIGHT(src0)) {
          tile_src0[local_y][local_x] = READ_IMAGE(src0, sampler, POS_src0_INSTANCE(tile_col, y, 0, 0)).x;
      } else {
          tile_src0[local_y][local_x] = 0;
      }
      
      if (tile_row < GET_IMAGE_HEIGHT(src1) && x < GET_IMAGE_WIDTH(src1)) {
          tile_src1[local_y][local_x] = READ_IMAGE(src1, sampler, POS_src1_INSTANCE(x, tile_row, 0, 0)).x;
      } else {
          tile_src1[local_y][local_x] = 0;
      }
      
      barrier(CLK_LOCAL_MEM_FENCE);
      
      // Compute partial dot product
      for (int i = 0; i < TILE_SIZE; ++i) {
          sum += tile_src0[local_y][i] * tile_src1[i][local_x];
      }
      
      barrier(CLK_LOCAL_MEM_FENCE);
  }
  
  WRITE_IMAGE(dst, POS_dst_INSTANCE(x, y, 0, 0), CONVERT_dst_PIXEL_TYPE(sum));
  })CLC";

    auto suggest_tile_size(const Device::Pointer & device) -> int
    {
        auto local_mem = device->getLocalMemorySize();
        auto max_work_group = device->getMaximumWorkGroupSize();
        
        // 2 tiles (src0 + src1) × 4 bytes per float
        int max_tile_from_memory = sqrt(local_mem / (2 * 4));
        int max_tile_from_workgroup = sqrt(max_work_group);
        
        int suggested = (max_tile_from_memory < max_tile_from_workgroup) ?
                        max_tile_from_memory : max_tile_from_workgroup;
        
        // Round down to power of 2 for efficiency
        while ((suggested & (suggested - 1)) != 0) suggested--;
        
        return suggested; // Returns 16, 32, 64, etc.
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
  int TILE_SIZE = static_cast<int>(suggest_tile_size(device));
  tier0::create_dst(matrix1, matrix_destination, matrix2->width(), matrix1->height(), matrix1->depth(), dType::FLOAT);
  const KernelInfo    kernel = { "multiply_matrix", alt_kernel };
  const ParameterList params = { { "src0", matrix1 }, { "src1", matrix2 }, { "dst", matrix_destination } };
  const RangeArray    range = { 
    ((matrix_destination->width() + TILE_SIZE-1) / TILE_SIZE) * TILE_SIZE, 
    ((matrix_destination->height() + TILE_SIZE-1) / TILE_SIZE) * TILE_SIZE, 
    1 
  };
  const RangeArray    local = { static_cast<size_t>(TILE_SIZE), static_cast<size_t>(TILE_SIZE), 1 };
  const ConstantList  constants = { {"TILE_SIZE", TILE_SIZE} };
  execute(device, kernel, params, range, local, constants);
  return matrix_destination;
}

} // namespace cle::tier1
