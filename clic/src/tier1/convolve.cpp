#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

#include "cle_convolve.h"
#include "cle_convolve_shared.h"

namespace cle::tier1
{

  
namespace
{
  /// Round up value to the nearest multiple of tile size
  size_t
  round_up(size_t value, size_t tile_size)
  {
    return ((value + tile_size - 1) / tile_size) * tile_size;
  }

  /// Define the tile sizes
  constexpr RangeArray
  get_tile_size(int dimension)
  {
    switch (dimension)
    {
      case 1:
        return { 256, 1, 1 };
      case 2:
        return { 16, 16, 1 };
      default: // 3D
        return { 8, 8, 8 };
    }
  }
    
  /// Determine optimal local work group size based on array dimension and kernel radius
  /// Larger radii require smaller work groups to fit within device memory constraints
  constexpr RangeArray
  get_local_range(int dimension, int max_radius)
  {
    if (dimension == 3)
    {
      // 3D arrays: small work groups due to memory constraints
      return (max_radius <= 5) ? RangeArray{ 8, 8, 8 } : RangeArray{ 4, 4, 4 };
    }
    
    if (dimension == 2)
    {
      // 2D arrays: balanced work group sizes
      if (max_radius <= 10)
      {
        return { 16, 16, 1 };
      }
      if (max_radius <= 30)
      {
        return { 8, 8, 1 };
      }
      return { 4, 4, 1 };
    }
    
    // 1D arrays: maximize parallelization along single dimension
    if (max_radius <= 50)
    {
      return { 256, 1, 1 };
    }
    if (max_radius <= 150)
    {
      return { 128, 1, 1 };
    }
    return { 64, 1, 1 };
  }
} // anonymous namespace

/// Perform convolution using global memory (no shared memory optimization)
/// Used when kernel size is too large for shared memory constraints
auto
convolve_global_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::FLOAT);
  
  const KernelInfo    kernel = { "convolve", kernel::convolve };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  
  execute(device, kernel, params, range);
  return dst;
}

/// Perform convolution using shared memory optimization
/// Loads data into local/shared memory in tiles for improved performance
auto
convolve_shared_func(const Device::Pointer & device,
                     const Array::Pointer &  src0,
                     const Array::Pointer &  src1,
                     Array::Pointer          dst,
                     const RangeArray &      tile,
                     const int               max_radius) -> Array::Pointer
{
  tier0::create_like(src0, dst, dType::FLOAT);
  
  const KernelInfo    kernel = { "convolve_shared", kernel::convolve_shared };
  const ParameterList params = { { "src0", src0 }, { "src1", src1 }, { "dst", dst } };
  const ConstantList  constants = { { "TILE_X", tile[0] },
                                    { "TILE_Y", tile[1] },
                                    { "TILE_Z", tile[2] },
                                    { "MAX_KERNEL_RADIUS", max_radius } };
  
  // Round global range up to tile boundaries
  const RangeArray global_range = { round_up(dst->width(), tile[0]),
                                    round_up(dst->height(), tile[1]),
                                    round_up(dst->depth(), tile[2]) };
  
  // Determine optimal local work group size
  const RangeArray local_range = get_local_range(src0->dimension(), max_radius);
  
  execute(device, kernel, params, global_range, local_range, constants);
  return dst;
}

/// Main convolution function - automatically selects optimal implementation
/// Chooses between shared memory optimization or global memory based on device constraints
auto
convolve_func(const Device::Pointer & device,
              const Array::Pointer &  src0,
              const Array::Pointer &  src1,
              Array::Pointer          dst) -> Array::Pointer
{
  // Determine tile size based on input dimension
  auto tile = get_tile_size(src0->dimension());

  // Calculate maximum kernel radius from convolution kernel dimensions
  const size_t max_radius = static_cast<size_t>(std::max({ src1->width(), src1->height(), src1->depth() }) * 0.5f);

  // Calculate required shared memory: tile + halo region (2 * radius on each side)
  const size_t shared_mem_size = (tile[0] + 2 * max_radius) * 
                                  (tile[1] + 2 * max_radius) * 
                                  (tile[2] + 2 * max_radius) * sizeof(float);
  const size_t max_shared_mem_size = device->getLocalMemorySize();

  // Use shared memory optimization if it fits within device limits
  if (shared_mem_size <= max_shared_mem_size)
  {
    return convolve_shared_func(device, src0, src1, dst, tile, max_radius);
  }
  // Fall back to global memory implementation for large kernels
  return convolve_global_func(device, src0, src1, dst);
}


} // namespace cle::tier1
