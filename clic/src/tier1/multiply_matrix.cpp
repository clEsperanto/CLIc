#include "tier0.hpp"
#include "tier1.hpp"

#include "utils.hpp"

namespace cle::tier1
{

namespace kernel
{

// ═══════════════════════════════════════════════════════════════════════════
// Kernel source (embedded)
// ═══════════════════════════════════════════════════════════════════════════

const char * kernel_code = R"CLC(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

#define LM_STRIDE (TILE_SIZE + 1)

#ifndef WPT_M
  #define WPT_M 1
#endif

#define RTS_M (TILE_SIZE / WPT_M)

__kernel void multiply_matrix(
    IMAGE_src0_TYPE  src0,
    IMAGE_src1_TYPE  src1,
    IMAGE_dst_TYPE   dst
)
{
  const int col = get_global_id(0);
  const int local_x = get_local_id(0);
  const int local_y = get_local_id(1);

  const int M = GET_IMAGE_HEIGHT(src0);
  const int K = GET_IMAGE_WIDTH(src0);
  const int N = GET_IMAGE_WIDTH(src1);

  const int row_base = get_group_id(1) * TILE_SIZE + local_y;

  // tile_src0 is stored TRANSPOSED: [k][m] instead of [m][k]
  // This means the compute loop reads sequential memory addresses
  // when iterating over the m (row) dimension within a tile.
  __local float tile_src0[TILE_SIZE][LM_STRIDE];
  __local float tile_src1[TILE_SIZE][LM_STRIDE];

  float acc[WPT_M];
  #pragma unroll
  for (int w = 0; w < WPT_M; ++w)
    acc[w] = 0.0f;

  const int num_tiles = (K + TILE_SIZE - 1) / TILE_SIZE;

  for (int tile = 0; tile < num_tiles; ++tile)
  {
    const int tile_offset = tile * TILE_SIZE;

    #pragma unroll
    for (int w = 0; w < WPT_M; ++w)
    {
      const int load_row = local_y + w * RTS_M;
      const int a_row = get_group_id(1) * TILE_SIZE + load_row;
      const int a_col = tile_offset + local_x;

      // TRANSPOSED store: swap indices so A is stored as [k][m]
      // local_x corresponds to the k dimension within the tile
      // load_row corresponds to the m dimension within the tile
      if (a_row < M && a_col < K)
        tile_src0[local_x][load_row] = READ_IMAGE(src0, sampler, POS_src0_INSTANCE(a_col, a_row, 0, 0)).x;
      else
        tile_src0[local_x][load_row] = 0.0f;

      // B tile unchanged: stored as [k][n]
      const int b_row = tile_offset + load_row;
      const int b_col = col;

      if (b_row < K && b_col < N)
        tile_src1[load_row][local_x] = READ_IMAGE(src1, sampler, POS_src1_INSTANCE(b_col, b_row, 0, 0)).x;
      else
        tile_src1[load_row][local_x] = 0.0f;
    }

    barrier(CLK_LOCAL_MEM_FENCE);

    #pragma unroll
    for (int k = 0; k < TILE_SIZE; ++k)
    {
      const float b_val = tile_src1[k][local_x];

      #pragma unroll
      for (int w = 0; w < WPT_M; ++w)
      {
        // TRANSPOSED read: tile_src0[k][m] instead of tile_src0[m][k]
        // Adjacent threads (different w values) now read adjacent memory
        // addresses tile_src0[k][local_y], tile_src0[k][local_y+RTS_M], ...
        // which are consecutive in the row, eliminating bank conflicts.
        acc[w] += tile_src0[k][local_y + w * RTS_M] * b_val;
      }
    }

    barrier(CLK_LOCAL_MEM_FENCE);
  }

  #pragma unroll
  for (int w = 0; w < WPT_M; ++w)
  {
    const int out_row = row_base + w * RTS_M;
    if (out_row < M && col < N)
    {
      WRITE_IMAGE(dst, POS_dst_INSTANCE(col, out_row, 0, 0), CONVERT_dst_PIXEL_TYPE(acc[w]));
    }
  }
}
)CLC";

} // namespace kernel

namespace
{


// ═══════════════════════════════════════════════════════════════════════════
// Matmul tuning configuration
// ═══════════════════════════════════════════════════════════════════════════

struct MatmulConfig
{
  int    tile_size;
  int    wpt_m;
  size_t local_x;  // = tile_size
  size_t local_y;  // = tile_size / wpt_m
  size_t global_x; // = ceil(N / tile_size) * tile_size
  size_t global_y; // = ceil(M / tile_size) * tile_size / wpt_m
};


// ── Helper: round up to next power of 2 ────────────────────────────────

static auto
next_power_of_2(int v) -> int
{
  if (v <= 0)
    return 1;
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  return v + 1;
}


// ── Helper: compute local memory requirement for a config ──────────────

static auto
local_mem_required(int tile_size) -> size_t
{
  // 2 tiles × TILE_SIZE rows × (TILE_SIZE + 1) columns × sizeof(float)
  return 2 * static_cast<size_t>(tile_size) * static_cast<size_t>(tile_size + 1) * sizeof(float);
}


// ── Validate a config against device limits ────────────────────────────

static auto
validate_config(const MatmulConfig & config, const Device::Pointer & device) -> bool
{
  const auto max_wg_size = device->getMaximumWorkGroupSize();
  const auto local_mem = device->getLocalMemorySize();

  // Work group must not exceed device limit
  const size_t wg_size = config.local_x * config.local_y;
  if (wg_size > max_wg_size)
    return false;

  // Workaround: Some devices report max_work_group_size but have
  // per-dimension limits. Common safe limit: each dim ≤ 256
  // (Most GPUs support at least 256 per dimension; some report up to 1024)
  if (config.local_x > 256 || config.local_y > 256)
    return false;

  // Local memory must fit
  if (local_mem_required(config.tile_size) > local_mem)
    return false;

  // WPT_M must divide TILE_SIZE evenly
  if (config.tile_size % config.wpt_m != 0)
    return false;

  // Local y must be at least 1
  if (config.local_y < 1)
    return false;

  return true;
}


// ── Select WPT_M for a given TILE_SIZE ─────────────────────────────────

static auto
select_wpt_m(int tile_size, const Device::Pointer & device) -> int
{
  const auto max_wg_size = device->getMaximumWorkGroupSize();

  // Preferred WPT_M values in descending order of performance
  // Higher WPT_M = better arithmetic intensity, but more register pressure
  static const int candidates[] = { 8, 4, 2, 1 };

  for (int wpt : candidates)
  {
    if (wpt > tile_size)
      continue;
    if (tile_size % wpt != 0)
      continue;

    const size_t local_y = static_cast<size_t>(tile_size / wpt);
    const size_t wg_size = static_cast<size_t>(tile_size) * local_y;

    if (wg_size <= max_wg_size && local_y >= 1)
      return wpt;
  }

  return 1; // safe fallback
}


// ── Select TILE_SIZE based on matrix dimensions + device ───────────────

static auto
select_tile_size(size_t M, size_t K, size_t N, const Device::Pointer & device) -> int
{
  const auto max_wg_size = device->getMaximumWorkGroupSize();
  const auto local_mem = device->getLocalMemorySize();

  // Heuristic: tile should relate to matrix inner dimension
  // and not be larger than the output dimensions
  const auto min_dim = std::min({ M, K, N });

  // Start from the ideal tile size based on matrix dimensions
  int ideal = static_cast<int>(std::sqrt(static_cast<double>(min_dim)));
  ideal = std::min(ideal, 32);
  ideal = next_power_of_2(ideal);

  // Try tile sizes from ideal downward
  static const int tile_candidates[] = { 32, 16, 8, 4, 2 };

  for (int ts : tile_candidates)
  {
    if (ts > ideal)
      continue;

    // Check local memory (with +1 padding)
    if (local_mem_required(ts) > local_mem)
      continue;

    // Check work group size (at minimum WPT_M=1, so wg = ts × ts)
    if (static_cast<size_t>(ts) * static_cast<size_t>(ts) > max_wg_size)
      continue;

    return ts;
  }

  // Absolute fallback
  return 1;
}


// ── Build a complete MatmulConfig ──────────────────────────────────────

static auto
build_config(size_t M, size_t K, size_t N, const Device::Pointer & device) -> MatmulConfig
{
  MatmulConfig config{};

  // ── CPU devices: skip tiling entirely ──
  if (device->getDeviceType() == "cpu")
  {
    config.tile_size = 1;
    config.wpt_m = 1;
    config.local_x = 1;
    config.local_y = 1;
    config.global_x = N;
    config.global_y = M;
    return config;
  }

  // ── Tiny matrices: not worth tiling ──
  if (M <= 4 && K <= 4 && N <= 4)
  {
    config.tile_size = 1;
    config.wpt_m = 1;
    config.local_x = 1;
    config.local_y = 1;
    config.global_x = N;
    config.global_y = M;
    return config;
  }

  // ── Normal path: select tile size then WPT_M ──
  config.tile_size = select_tile_size(M, K, N, device);
  config.wpt_m = select_wpt_m(config.tile_size, device);

  const size_t ts = static_cast<size_t>(config.tile_size);
  const size_t rts_m = ts / static_cast<size_t>(config.wpt_m);

  config.local_x = ts;
  config.local_y = rts_m;
  config.global_x = ((N + ts - 1) / ts) * ts;
  config.global_y = ((M + ts - 1) / ts) * ts / static_cast<size_t>(config.wpt_m);

  // ── Validate and fall back if needed ──
  if (!validate_config(config, device))
  {
    // Try WPT_M = 1 (larger work group but simpler)
    config.wpt_m = 1;
    config.local_y = ts;
    config.global_y = ((M + ts - 1) / ts) * ts;

    if (!validate_config(config, device))
    {
      // Fall back to smaller tile size
      config.tile_size = std::min(config.tile_size, 4);
      const size_t ts2 = static_cast<size_t>(config.tile_size);
      config.wpt_m = 1;
      config.local_x = ts2;
      config.local_y = ts2;
      config.global_x = ((N + ts2 - 1) / ts2) * ts2;
      config.global_y = ((M + ts2 - 1) / ts2) * ts2;

      if (!validate_config(config, device))
      {
        // Absolute safe fallback: no tiling
        config.tile_size = 1;
        config.wpt_m = 1;
        config.local_x = 1;
        config.local_y = 1;
        config.global_x = N;
        config.global_y = M;
      }
    }
  }

  return config;
}


// ── Build a fallback config (guaranteed safe) ──────────────────────────

static auto
build_fallback_config(size_t M, size_t N) -> MatmulConfig
{
  return MatmulConfig{
    /* tile_size */ 1,
    /* wpt_m     */ 1,
    /* local_x   */ 1,
    /* local_y   */ 1,
    /* global_x  */ N,
    /* global_y  */ M,
  };
}


} // anonymous namespace


// ═══════════════════════════════════════════════════════════════════════════
// Public API
// ═══════════════════════════════════════════════════════════════════════════

auto
multiply_matrix_func(const Device::Pointer & device,
                     const Array::Pointer &  matrix1,
                     const Array::Pointer &  matrix2,
                     Array::Pointer          matrix_destination) -> Array::Pointer
{
  // ── Input validation ────────────────────────────────────────────────
  if (matrix1->dim() > 2 || matrix2->dim() > 2)
  {
    std::cerr << "Warning: multiply_matrix expected 2D arrays but got " << matrix1->dim() << "D and " << matrix2->dim() << "D."
              << std::endl;
  }

  if (matrix1->width() != matrix2->height())
  {
    std::cerr << "Warning: matrix dimensions are not compatible for multiplication. "
              << "Expected (M,K)×(K,N) but got (" << matrix1->height() << "," << matrix1->width() << ")×(" << matrix2->height() << ","
              << matrix2->width() << ")." << std::endl;
  }

  // ── Create output ───────────────────────────────────────────────────
  tier0::create_dst(matrix1, matrix_destination, matrix2->width(), matrix1->height(), 1, dType::FLOAT);

  const size_t M = matrix_destination->height();
  const size_t K = matrix1->width();
  const size_t N = matrix_destination->width();

  // ── Select configuration ────────────────────────────────────────────
  const MatmulConfig config = build_config(M, K, N, device);

  const KernelInfo    kernel = { "multiply_matrix", kernel::kernel_code };
  const ParameterList params = { { "src0", matrix1 }, { "src1", matrix2 }, { "dst", matrix_destination } };
  const ConstantList  constants = { { "TILE_SIZE", config.tile_size }, { "WPT_M", config.wpt_m } };
  const RangeArray    range = { config.global_x, config.global_y, 1 };
  const RangeArray    local = { config.local_x, config.local_y, 1 };

  // ── Execute with fallback chain ─────────────────────────────────────
  try
  {
    execute(device, kernel, params, range, local, constants);
  }
  catch (const std::runtime_error & e)
  {
    // ── Fallback 1: TILE_SIZE with WPT_M=1 ──
    if (config.wpt_m > 1)
    {
      std::cerr << "Warning: multiply_matrix failed with TILE_SIZE=" << config.tile_size << ", WPT_M=" << config.wpt_m
                << ". Retrying with WPT_M=1.\n"
                << "  Error: " << e.what() << std::endl;

      const size_t       ts = static_cast<size_t>(config.tile_size);
      const ConstantList fallback_constants = { { "TILE_SIZE", config.tile_size }, { "WPT_M", 1 } };
      const RangeArray   fallback_range = { ((N + ts - 1) / ts) * ts, ((M + ts - 1) / ts) * ts, 1 };
      const RangeArray   fallback_local = { ts, ts, 1 };

      try
      {
        execute(device, kernel, params, fallback_range, fallback_local, fallback_constants);
        return matrix_destination;
      }
      catch (const std::runtime_error & e2)
      {
        std::cerr << "Warning: WPT_M=1 fallback also failed. Falling back to TILE_SIZE=1.\n"
                  << "  Error: " << e2.what() << std::endl;
      }
    }
    else if (config.tile_size > 1)
    {
      std::cerr << "Warning: multiply_matrix failed with TILE_SIZE=" << config.tile_size << ". Falling back to TILE_SIZE=1.\n"
                << "  Error: " << e.what() << std::endl;
    }

    // ── Fallback 2: TILE_SIZE=1, no tiling at all ──
    if (config.tile_size > 1)
    {
      const MatmulConfig safe = build_fallback_config(M, N);
      const ConstantList safe_constants = { { "TILE_SIZE", 1 }, { "WPT_M", 1 } };
      const RangeArray   safe_range = { safe.global_x, safe.global_y, 1 };
      const RangeArray   safe_local = { safe.local_x, safe.local_y, 1 };

      try
      {
        execute(device, kernel, params, safe_range, safe_local, safe_constants);
      }
      catch (const std::runtime_error & e3)
      {
        throw std::runtime_error("multiply_matrix: all kernel configurations failed. "
                                 "Last error: " +
                                 std::string(e3.what()));
      }
    }
    else
    {
      // TILE_SIZE was already 1 and it still failed — nothing left to try
      throw std::runtime_error("multiply_matrix: kernel execution failed with TILE_SIZE=1. "
                               "Error: " +
                               std::string(e.what()));
    }
  }

  return matrix_destination;
}

} // namespace cle::tier1
