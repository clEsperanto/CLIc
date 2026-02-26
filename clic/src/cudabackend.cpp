#include "backend.hpp"
#include "cle_preamble_cu.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <numeric>
#include <string>
#include <vector>

namespace cle
{

// ============================================================================
// Compile-time CUDA guard
// ============================================================================

#if !USE_CUDA

// ── Stub implementations when CUDA is disabled ──────────────────────────────

CUDABackend::CUDABackend() {}

auto
CUDABackend::getDevices(const std::string &) const -> std::vector<Device::Pointer>
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::getDevice(const std::string &, const std::string &) const -> Device::Pointer
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::getDeviceFromIndex(size_t, const std::string &) const -> Device::Pointer
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::getDevicesList(const std::string &) const -> std::vector<std::string>
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::getType() const -> Backend::Type
{
  return Backend::Type::CUDA;
}

auto
CUDABackend::allocateMemory(const Device::Pointer &, const std::array<size_t, 3> &, const dType &, const mType &, std::shared_ptr<void> &)
  const -> void
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::freeMemory(const Device::Pointer &, const mType &, const std::shared_ptr<void> &) const -> void
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::getRefCount(void *) const -> int
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::writeMemory(const Device::Pointer &,
                         const std::shared_ptr<void> &,
                         std::array<size_t, 3> &,
                         std::array<size_t, 3> &,
                         std::array<size_t, 3> &,
                         const dType &,
                         const mType &,
                         const void *) const -> void
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::readMemory(const Device::Pointer &,
                        const std::shared_ptr<void> &,
                        std::array<size_t, 3> &,
                        std::array<size_t, 3> &,
                        std::array<size_t, 3> &,
                        const dType &,
                        const mType &,
                        void *) const -> void
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::copyMemoryBufferToBuffer(const Device::Pointer &,
                                      const std::shared_ptr<void> &,
                                      std::array<size_t, 3> &,
                                      std::array<size_t, 3> &,
                                      const std::shared_ptr<void> &,
                                      std::array<size_t, 3> &,
                                      std::array<size_t, 3> &,
                                      std::array<size_t, 3> &,
                                      const size_t &) const -> void
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::copyMemoryImageToBuffer(const Device::Pointer &,
                                     const std::shared_ptr<void> &,
                                     std::array<size_t, 3> &,
                                     std::array<size_t, 3> &,
                                     const std::shared_ptr<void> &,
                                     std::array<size_t, 3> &,
                                     std::array<size_t, 3> &,
                                     std::array<size_t, 3> &,
                                     const size_t &) const -> void
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::copyMemoryBufferToImage(const Device::Pointer &,
                                     const std::shared_ptr<void> &,
                                     std::array<size_t, 3> &,
                                     std::array<size_t, 3> &,
                                     const std::shared_ptr<void> &,
                                     std::array<size_t, 3> &,
                                     std::array<size_t, 3> &,
                                     std::array<size_t, 3> &,
                                     const size_t &) const -> void
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::copyMemoryImageToImage(const Device::Pointer &,
                                    const std::shared_ptr<void> &,
                                    std::array<size_t, 3> &,
                                    std::array<size_t, 3> &,
                                    const std::shared_ptr<void> &,
                                    std::array<size_t, 3> &,
                                    std::array<size_t, 3> &,
                                    std::array<size_t, 3> &,
                                    const size_t &) const -> void
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::setMemory(const Device::Pointer &,
                       const std::shared_ptr<void> &,
                       std::array<size_t, 3> &,
                       std::array<size_t, 3> &,
                       std::array<size_t, 3> &,
                       const dType &,
                       const mType &,
                       const float &) const -> void
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::buildKernel(const Device::Pointer &, const std::string &, const std::string &, std::shared_ptr<void> &) const -> void
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::executeKernel(const Device::Pointer &,
                           const std::string &,
                           const std::string &,
                           const std::array<size_t, 3> &,
                           const std::array<size_t, 3> &,
                           const std::vector<std::shared_ptr<void>> &,
                           const std::vector<size_t> &) const -> void
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::getPreamble() const -> std::string
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::allocateBuffer(const Device::Pointer &, const size_t &, std::shared_ptr<void> &) -> void
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::writeBuffer(const Device::Pointer &,
                         const std::shared_ptr<void> &,
                         const std::array<size_t, 3> &,
                         const std::array<size_t, 3> &,
                         const std::array<size_t, 3> &,
                         const void *) -> void
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::readBuffer(const Device::Pointer &,
                        const std::shared_ptr<void> &,
                        const std::array<size_t, 3> &,
                        const std::array<size_t, 3> &,
                        const std::array<size_t, 3> &,
                        void *) -> void
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

auto
CUDABackend::setBuffer(const Device::Pointer &,
                       const std::shared_ptr<void> &,
                       const std::array<size_t, 3> &,
                       const std::array<size_t, 3> &,
                       const std::array<size_t, 3> &,
                       const dType &,
                       const float &) -> void
{
  throw std::runtime_error("Error: CUDA is not enabled");
}

#else // USE_CUDA ═════════════════════════════════════════════════════════════

// ============================================================================
// Error Handling
// ============================================================================

/**
 * @brief Convert a CUresult error code to a human-readable string.
 *        Returns "Unknown CUDA error" if the driver provides no string.
 */
[[nodiscard]] static auto
getErrorString(CUresult error) -> std::string
{
  const char * str = nullptr;
  cuGetErrorString(error, &str);
  return (str != nullptr) ? std::string(str) : "Unknown CUDA error";
}

/**
 * @brief Convert an nvrtcResult error code to a human-readable string.
 */
[[nodiscard]] static auto
getErrorString(nvrtcResult error) -> std::string
{
  return std::string(nvrtcGetErrorString(error));
}

/// Check a CUDA driver API call; throw with context on failure.
#  define CU_CHECK(call, msg)                                                                                                    \
    do                                                                                                                           \
    {                                                                                                                            \
      CUresult err_ = (call);                                                                                                    \
      if (err_ != CUDA_SUCCESS)                                                                                                  \
      {                                                                                                                          \
        throw std::runtime_error(std::string(msg) + " CUDA error: " + getErrorString(err_) + " (" + std::to_string(err_) + ")"); \
      }                                                                                                                          \
    } while (0)

/// Check an NVRTC call; throw with context on failure.
#  define NVRTC_CHECK(call, msg)                                                                                                  \
    do                                                                                                                            \
    {                                                                                                                             \
      nvrtcResult err_ = (call);                                                                                                  \
      if (err_ != NVRTC_SUCCESS)                                                                                                  \
      {                                                                                                                           \
        throw std::runtime_error(std::string(msg) + " NVRTC error: " + getErrorString(err_) + " (" + std::to_string(err_) + ")"); \
      }                                                                                                                           \
    } while (0)

// ============================================================================
// RAII Context Guard
// ============================================================================

/**
 * @brief RAII helper that sets the CUDA context for the given device on
 *        construction. Performs dynamic_pointer_cast and validates once.
 *
 * Eliminates the repeated 3-line boilerplate (cast + validate + set context)
 * from every method.
 */
class CUDAContextGuard
{
public:
  explicit CUDAContextGuard(const Device::Pointer & device)
    : cuda_device_(std::dynamic_pointer_cast<const CUDADevice>(device))
  {
    if (!cuda_device_)
    {
      throw std::runtime_error("Error: Failed to cast device to CUDADevice");
    }
    CU_CHECK(cuCtxSetCurrent(cuda_device_->getCUDAContext()), "Error: Failed to set CUDA context");
  }

  [[nodiscard]] auto
  device() const -> const std::shared_ptr<const CUDADevice> &
  {
    return cuda_device_;
  }

  [[nodiscard]] auto
  context() const -> CUcontext
  {
    return cuda_device_->getCUDAContext();
  }

  [[nodiscard]] auto
  stream() const -> CUstream
  {
    return cuda_device_->getCUDAStream();
  }

private:
  std::shared_ptr<const CUDADevice> cuda_device_;
};

// ============================================================================
// Memcpy Helpers (unified 1D/2D/3D logic)
// ============================================================================

/// Direction of a memcpy operation.
enum class MemcpyDirection
{
  HostToDevice,
  DeviceToHost,
  DeviceToDevice
};

/**
 * @brief Perform a 1D, 2D, or 3D memory copy between host and/or device.
 *
 * Selects cuMemcpy / cuMemcpy2D / cuMemcpy3D based on the shape, eliminating
 * the triplicated branching logic that was in writeBuffer/readBuffer/copyBuffer.
 *
 * @param src_type        Source memory type (host or device).
 * @param src_ptr         Source pointer (host void* or CUdeviceptr cast).
 * @param src_origin      Byte offset origin for the source.
 * @param src_row_pitch   Row pitch in bytes for the source (0 for 1D).
 * @param src_slice_pitch Slice pitch (height) for the source (0 for 1D/2D).
 * @param dst_type        Destination memory type.
 * @param dst_ptr         Destination pointer.
 * @param dst_origin      Byte offset origin for the destination.
 * @param dst_row_pitch   Row pitch in bytes for the destination.
 * @param dst_slice_pitch Slice pitch for the destination.
 * @param region          Size of the region to copy {widthBytes, height, depth}.
 * @param shape           Full buffer shape to determine dimensionality.
 */
static auto
performMemcpy(CUmemorytype                  src_type,
              const void *                  src_ptr,
              const std::array<size_t, 3> & src_origin,
              size_t                        src_row_pitch,
              size_t                        src_slice_pitch,
              CUmemorytype                  dst_type,
              const void *                  dst_ptr,
              const std::array<size_t, 3> & dst_origin,
              size_t                        dst_row_pitch,
              size_t                        dst_slice_pitch,
              const std::array<size_t, 3> & region,
              const std::array<size_t, 3> & shape) -> void
{
  CUresult err;

  if (shape[2] > 1)
  {
    // ── 3D copy ──
    CUDA_MEMCPY3D params = {};

    params.srcMemoryType = src_type;
    if (src_type == CU_MEMORYTYPE_HOST)
    {
      params.srcHost = src_ptr;
    }
    else
    {
      params.srcDevice = reinterpret_cast<CUdeviceptr>(src_ptr);
    }
    params.srcXInBytes = src_origin[0];
    params.srcY = src_origin[1];
    params.srcZ = src_origin[2];
    params.srcPitch = src_row_pitch;
    params.srcHeight = src_slice_pitch;

    params.dstMemoryType = dst_type;
    if (dst_type == CU_MEMORYTYPE_HOST)
    {
      params.dstHost = const_cast<void *>(dst_ptr);
    }
    else
    {
      params.dstDevice = reinterpret_cast<CUdeviceptr>(dst_ptr);
    }
    params.dstXInBytes = dst_origin[0];
    params.dstY = dst_origin[1];
    params.dstZ = dst_origin[2];
    params.dstPitch = dst_row_pitch;
    params.dstHeight = dst_slice_pitch;

    params.WidthInBytes = region[0];
    params.Height = region[1];
    params.Depth = region[2];

    err = cuMemcpy3D(&params);
  }
  else if (shape[1] > 1)
  {
    // ── 2D copy ──
    CUDA_MEMCPY2D params = {};

    params.srcMemoryType = src_type;
    if (src_type == CU_MEMORYTYPE_HOST)
    {
      params.srcHost = src_ptr;
    }
    else
    {
      params.srcDevice = reinterpret_cast<CUdeviceptr>(src_ptr);
    }
    params.srcXInBytes = src_origin[0];
    params.srcY = src_origin[1];
    params.srcPitch = src_row_pitch;

    params.dstMemoryType = dst_type;
    if (dst_type == CU_MEMORYTYPE_HOST)
    {
      params.dstHost = const_cast<void *>(dst_ptr);
    }
    else
    {
      params.dstDevice = reinterpret_cast<CUdeviceptr>(dst_ptr);
    }
    params.dstXInBytes = dst_origin[0];
    params.dstY = dst_origin[1];
    params.dstPitch = dst_row_pitch;

    params.WidthInBytes = region[0];
    params.Height = region[1];

    err = cuMemcpy2D(&params);
  }
  else
  {
    // ── 1D copy ──
    auto src_addr = reinterpret_cast<CUdeviceptr>(reinterpret_cast<const char *>(src_ptr) + src_origin[0]);
    auto dst_addr = reinterpret_cast<CUdeviceptr>(reinterpret_cast<const char *>(dst_ptr) + dst_origin[0]);

    err = cuMemcpy(dst_addr, src_addr, region[0]);
  }

  CU_CHECK(err, "Error: Memory copy failed");
}

// ============================================================================
// Disk cache helpers
// ============================================================================

static auto
savePtxToCache(const std::string & device_hash, const std::string & source_hash, const std::string & ptx) -> void
{
  DiskCache::instance().saveBinary(device_hash, source_hash, "ptx", ptx.c_str(), ptx.size());
}

[[nodiscard]] static auto
loadPtxFromCache(const std::string & device_hash, const std::string & source_hash) -> std::string
{
  auto binary = DiskCache::instance().loadBinary(device_hash, source_hash, "ptx");
  if (binary.empty())
  {
    return {};
  }
  return std::string(binary.begin(), binary.end());
}

// ============================================================================
// Bit-cast helper (safe type-punning without UB)
// ============================================================================

/**
 * @brief Safely reinterpret the bits of one type as another of the same size.
 *        Equivalent to std::bit_cast (C++20) but works in C++17.
 */
template <typename To, typename From>
static auto
bitCast(const From & src) -> To
{
  static_assert(sizeof(To) == sizeof(From), "bit_cast: size mismatch");
  To dst;
  std::memcpy(&dst, &src, sizeof(To));
  return dst;
}

// ============================================================================
// Kernel compilation helper
// ============================================================================

/**
 * @brief Compile an NVRTC program to PTX.
 *
 * On failure, retrieves and logs the build log before throwing.
 */
[[nodiscard]] static auto
compileToPtx(const std::string & kernel_source, const std::string & arch) -> std::string
{
  nvrtcProgram prog;
  NVRTC_CHECK(nvrtcCreateProgram(&prog, kernel_source.c_str(), nullptr, 0, nullptr, nullptr), "Error: Failed to create NVRTC program");

  // RAII cleanup for the program handle
  struct ProgramGuard
  {
    nvrtcProgram * p;
    ~ProgramGuard() { nvrtcDestroyProgram(p); }
  } guard{ &prog };

  const std::string                 arch_opt = "--gpu-architecture=compute_" + arch;
  const std::string                 warn_opt = "--disable-warnings";
  const std::array<const char *, 2> options = { arch_opt.c_str(), warn_opt.c_str() };

  nvrtcResult res = nvrtcCompileProgram(prog, static_cast<int>(options.size()), options.data());
  if (res != NVRTC_SUCCESS)
  {
    // Retrieve and report the build log
    size_t log_size = 0;
    nvrtcGetProgramLogSize(prog, &log_size);
    std::string log(log_size, '\0');
    nvrtcGetProgramLog(prog, log.data());
    throw std::runtime_error("Error: Failed to compile kernel. NVRTC error: " + getErrorString(res) + " (" + std::to_string(res) +
                             ")\nBuild log:\n" + log);
  }

  size_t ptx_size = 0;
  NVRTC_CHECK(nvrtcGetPTXSize(prog, &ptx_size), "Error: Failed to get PTX size");

  // NVRTC includes the null terminator in ptx_size
  std::string ptx(ptx_size, '\0');
  NVRTC_CHECK(nvrtcGetPTX(prog, ptx.data()), "Error: Failed to get PTX");

  return ptx;
}

// ============================================================================
// Block size calculation helper
// ============================================================================

/**
 * @brief Compute a reasonable CUDA block (thread) size for the given global size.
 *
 * Strategy:
 *   - Determine the active dimensions (global_size[i] > 1).
 *   - Distribute threads per block across active dimensions, capped at 1024 total
 *     and 64 in the z-dimension (hardware limit on many architectures).
 *   - Inactive dimensions get block size = 1.
 */
[[nodiscard]] static auto
computeBlockSize(const std::array<size_t, 3> & global_size) -> std::array<size_t, 3>
{
  int dim = 0;
  for (size_t i = 0; i < 3; ++i)
  {
    if (global_size[i] > 1)
    {
      ++dim;
    }
  }

  std::array<size_t, 3> block = { 1, 1, 1 };

  switch (dim)
  {
    case 0:
      break;
    case 1: {
      constexpr size_t kMaxThreads1D = 256;
      for (size_t i = 0; i < 3; ++i)
      {
        if (global_size[i] > 1)
        {
          block[i] = std::min(global_size[i], kMaxThreads1D);
        }
      }
      break;
    }
    case 2: {
      // Prefer wider x for coalescing: 32×8 instead of 16×16
      constexpr size_t kBlockDimX_2D = 32;     // <- changed
      constexpr size_t kBlockDimOther_2D = 8;   // <- changed
      bool first_active = true;
      for (size_t i = 0; i < 3; ++i)
      {
        if (global_size[i] > 1)
        {
          if (first_active)
          {
            block[i] = std::min(global_size[i], kBlockDimX_2D);
            first_active = false;
          }
          else
          {
            block[i] = std::min(global_size[i], kBlockDimOther_2D);
          }
        }
      }
      break;
    }
    case 3: {
      // x=32 for coalescing, y=8, z=1 in block (z goes to grid)
      constexpr std::array<size_t, 3> kBlockDim3D = { 32, 8, 1 };  // <- changed
      for (size_t i = 0; i < 3; ++i)
      {
        block[i] = std::min(global_size[i], kBlockDim3D[i]);
      }
      break;
    }
  }

  block[2] = std::min(block[2], size_t{ 64 });

  size_t total = block[0] * block[1] * block[2];
  while (total > 1024)
  {
    auto max_it = std::max_element(block.begin(), block.end());
    *max_it = (*max_it + 1) / 2;
    total = block[0] * block[1] * block[2];
  }

  return block;
}

/**
 * @brief Compute grid size from global size and block size (ceiling division).
 */
[[nodiscard]] static auto
computeGridSize(const std::array<size_t, 3> & global_size, const std::array<size_t, 3> & block_size) -> std::array<size_t, 3>
{
  return { (global_size[0] + block_size[0] - 1) / block_size[0],
           (global_size[1] + block_size[1] - 1) / block_size[1],
           (global_size[2] + block_size[2] - 1) / block_size[2] };
}

// ============================================================================
// CUDABackend Implementation
// ============================================================================

CUDABackend::CUDABackend()
{
  CU_CHECK(cuInit(0), "Error: Failed to initialize CUDA");
}

// ── Device enumeration ──────────────────────────────────────────────────────

auto
CUDABackend::getDevices(const std::string & type) const -> std::vector<Device::Pointer>
{
  int deviceCount = 0;
  CU_CHECK(cuDeviceGetCount(&deviceCount), "Error: Failed to get device count");

  std::vector<Device::Pointer> devices;
  devices.reserve(static_cast<size_t>(deviceCount));
  for (int i = 0; i < deviceCount; ++i)
  {
    devices.push_back(std::make_shared<CUDADevice>(i));
  }

  if (devices.empty())
  {
    std::cerr << "Warning: Failed to find '" << type << "' CUDA compatible devices." << std::endl;
  }
  return devices;
}

auto
CUDABackend::getDevice(const std::string & name, const std::string & type) const -> Device::Pointer
{
  auto       devices = getDevices(type);
  const auto lower_name = to_lower(name);

  auto it = std::find_if(devices.begin(), devices.end(), [&lower_name](const Device::Pointer & dev) {
    return dev->getName(true).find(lower_name) != std::string::npos;
  });

  if (it != devices.end())
  {
    return *it;
  }
  if (!devices.empty())
  {
    return devices.back();
  }

  std::cerr << "Warning: No CUDA compatible devices found." << std::endl;
  return nullptr;
}

auto
CUDABackend::getDeviceFromIndex(size_t index, const std::string & type) const -> Device::Pointer
{
  auto devices = getDevices(type);

  if (index < devices.size())
  {
    return devices[index];
  }
  if (!devices.empty())
  {
    return devices.back();
  }

  std::cerr << "Warning: No CUDA compatible devices found." << std::endl;
  return nullptr;
}

auto
CUDABackend::getDevicesList(const std::string & type) const -> std::vector<std::string>
{
  auto                     devices = getDevices(type);
  std::vector<std::string> names;
  names.reserve(devices.size());
  for (const auto & dev : devices)
  {
    names.emplace_back(dev->getName());
  }
  return names;
}

auto
CUDABackend::getType() const -> Backend::Type
{
  return Backend::Type::CUDA;
}

// ── Memory allocation ───────────────────────────────────────────────────────

auto
CUDABackend::allocateMemory(const Device::Pointer &       device,
                            const std::array<size_t, 3> & region,
                            const dType &                 dtype,
                            const mType &                 mtype,
                            std::shared_ptr<void> &       data_ptr) const -> void
{
  // @StRigaud TODO: implement true image support for CUDA
  // For now, both BUFFER and IMAGE allocate linear memory.
  const size_t size = region[0] * region[1] * region[2] * toBytes(dtype);
  allocateBuffer(device, size, data_ptr);
}

auto
CUDABackend::allocateBuffer(const Device::Pointer & device, const size_t & size, std::shared_ptr<void> & data_ptr) -> void
{
  CUDAContextGuard ctx(device);

  CUdeviceptr mem = 0;
  CU_CHECK(cuMemAlloc(&mem, size), "Error: Failed to allocate buffer memory");
  *data_ptr = reinterpret_cast<void *>(mem);
}

auto
CUDABackend::freeMemory(const Device::Pointer & device, const mType & mtype, const std::shared_ptr<void> & data_ptr) const -> void
{
  CUDAContextGuard ctx(device);

  // @StRigaud TODO: differentiate image vs buffer free when image support lands
  CU_CHECK(cuMemFree(reinterpret_cast<CUdeviceptr>(*data_ptr)), "Error: Failed to free memory");
}

auto
CUDABackend::getRefCount(void * data_ptr) const -> int
{
  // CUDA driver API does not provide reference counting for allocations.
  return 1;
}

// ── Memory read / write ─────────────────────────────────────────────────────

auto
CUDABackend::writeMemory(const Device::Pointer &       device,
                         const std::shared_ptr<void> & buffer_ptr,
                         std::array<size_t, 3> &       buffer_shape,
                         std::array<size_t, 3> &       buffer_origin,
                         std::array<size_t, 3> &       region,
                         const dType &                 dtype,
                         const mType &                 mtype,
                         const void *                  host_ptr) const -> void
{
  // Convert element counts to byte counts for the first dimension
  const size_t elem_size = toBytes(dtype);
  buffer_shape[0] *= elem_size;
  buffer_origin[0] *= elem_size;
  region[0] *= elem_size;

  // @StRigaud TODO: differentiate IMAGE path when image support lands
  writeBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
}

auto
CUDABackend::writeBuffer(const Device::Pointer &       device,
                         const std::shared_ptr<void> & buffer_ptr,
                         const std::array<size_t, 3> & buffer_shape,
                         const std::array<size_t, 3> & buffer_origin,
                         const std::array<size_t, 3> & region,
                         const void *                  host_ptr) -> void
{
  CUDAContextGuard ctx(device);

  const size_t                dst_row_pitch = (buffer_shape[1] > 1) ? buffer_shape[0] : 0;
  const size_t                dst_slice_pitch = (buffer_shape[2] > 1) ? buffer_shape[1] : 0;
  const std::array<size_t, 3> host_origin = { 0, 0, 0 };

  performMemcpy(CU_MEMORYTYPE_HOST,
                host_ptr,
                host_origin,
                0,
                0,
                CU_MEMORYTYPE_DEVICE,
                *buffer_ptr,
                buffer_origin,
                dst_row_pitch,
                dst_slice_pitch,
                region,
                buffer_shape);
}

auto
CUDABackend::readMemory(const Device::Pointer &       device,
                        const std::shared_ptr<void> & buffer_ptr,
                        std::array<size_t, 3> &       buffer_shape,
                        std::array<size_t, 3> &       buffer_origin,
                        std::array<size_t, 3> &       region,
                        const dType &                 dtype,
                        const mType &                 mtype,
                        void *                        host_ptr) const -> void
{
  const size_t elem_size = toBytes(dtype);
  buffer_shape[0] *= elem_size;
  buffer_origin[0] *= elem_size;
  region[0] *= elem_size;

  // @StRigaud TODO: differentiate IMAGE path when image support lands
  readBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
}

auto
CUDABackend::readBuffer(const Device::Pointer &       device,
                        const std::shared_ptr<void> & buffer_ptr,
                        const std::array<size_t, 3> & buffer_shape,
                        const std::array<size_t, 3> & buffer_origin,
                        const std::array<size_t, 3> & region,
                        void *                        host_ptr) -> void
{
  CUDAContextGuard ctx(device);

  const size_t                src_row_pitch = (buffer_shape[1] > 1) ? buffer_shape[0] : 0;
  const size_t                src_slice_pitch = (buffer_shape[2] > 1) ? buffer_shape[1] : 0;
  const std::array<size_t, 3> host_origin = { 0, 0, 0 };

  performMemcpy(CU_MEMORYTYPE_DEVICE,
                *buffer_ptr,
                buffer_origin,
                src_row_pitch,
                src_slice_pitch,
                CU_MEMORYTYPE_HOST,
                host_ptr,
                host_origin,
                0,
                0,
                region,
                buffer_shape);
}

// ── Memory copy (device-to-device) ──────────────────────────────────────────

auto
CUDABackend::copyMemoryBufferToBuffer(const Device::Pointer &       device,
                                      const std::shared_ptr<void> & src_ptr,
                                      std::array<size_t, 3> &       src_origin,
                                      std::array<size_t, 3> &       src_shape,
                                      const std::shared_ptr<void> & dst_ptr,
                                      std::array<size_t, 3> &       dst_origin,
                                      std::array<size_t, 3> &       dst_shape,
                                      std::array<size_t, 3> &       region,
                                      const size_t &                bytes) const -> void
{
  CUDAContextGuard ctx(device);

  // Convert element counts to byte counts
  region[0] *= bytes;
  src_origin[0] *= bytes;
  src_shape[0] *= bytes;
  dst_origin[0] *= bytes;
  dst_shape[0] *= bytes;

  const size_t src_row_pitch = (src_shape[1] > 1) ? src_shape[0] : 0;
  const size_t dst_row_pitch = (dst_shape[1] > 1) ? dst_shape[0] : 0;
  const size_t src_slice_pitch = (src_shape[2] > 1) ? src_shape[1] : 0;
  const size_t dst_slice_pitch = (dst_shape[2] > 1) ? dst_shape[1] : 0;

  performMemcpy(CU_MEMORYTYPE_DEVICE,
                *src_ptr,
                src_origin,
                src_row_pitch,
                src_slice_pitch,
                CU_MEMORYTYPE_DEVICE,
                *dst_ptr,
                dst_origin,
                dst_row_pitch,
                dst_slice_pitch,
                region,
                src_shape);
}

auto
CUDABackend::copyMemoryImageToBuffer(const Device::Pointer &       device,
                                     const std::shared_ptr<void> & src_ptr,
                                     std::array<size_t, 3> &       src_origin,
                                     std::array<size_t, 3> &       src_shape,
                                     const std::shared_ptr<void> & dst_ptr,
                                     std::array<size_t, 3> &       dst_origin,
                                     std::array<size_t, 3> &       dst_shape,
                                     std::array<size_t, 3> &       region,
                                     const size_t &                bytes) const -> void
{
  // @StRigaud TODO: implement proper image-to-buffer copy when image support lands
  copyMemoryBufferToBuffer(device, src_ptr, src_origin, src_shape, dst_ptr, dst_origin, dst_shape, region, bytes);
}

auto
CUDABackend::copyMemoryBufferToImage(const Device::Pointer &       device,
                                     const std::shared_ptr<void> & src_ptr,
                                     std::array<size_t, 3> &       src_origin,
                                     std::array<size_t, 3> &       src_shape,
                                     const std::shared_ptr<void> & dst_ptr,
                                     std::array<size_t, 3> &       dst_origin,
                                     std::array<size_t, 3> &       dst_shape,
                                     std::array<size_t, 3> &       region,
                                     const size_t &                bytes) const -> void
{
  // @StRigaud TODO: implement proper buffer-to-image copy when image support lands
  copyMemoryBufferToBuffer(device, src_ptr, src_origin, src_shape, dst_ptr, dst_origin, dst_shape, region, bytes);
}

auto
CUDABackend::copyMemoryImageToImage(const Device::Pointer &       device,
                                    const std::shared_ptr<void> & src_ptr,
                                    std::array<size_t, 3> &       src_origin,
                                    std::array<size_t, 3> &       src_shape,
                                    const std::shared_ptr<void> & dst_ptr,
                                    std::array<size_t, 3> &       dst_origin,
                                    std::array<size_t, 3> &       dst_shape,
                                    std::array<size_t, 3> &       region,
                                    const size_t &                bytes) const -> void
{
  // @StRigaud TODO: implement proper image-to-image copy when image support lands
  copyMemoryBufferToBuffer(device, src_ptr, src_origin, src_shape, dst_ptr, dst_origin, dst_shape, region, bytes);
}

// ── Memory fill ─────────────────────────────────────────────────────────────

auto
CUDABackend::setBuffer(const Device::Pointer &       device,
                       const std::shared_ptr<void> & buffer_ptr,
                       const std::array<size_t, 3> & buffer_shape,
                       const std::array<size_t, 3> & buffer_origin,
                       const std::array<size_t, 3> & region,
                       const dType &                 dtype,
                       const float &                 value) -> void
{
  CUDAContextGuard ctx(device);

  const auto        count = region[0] * region[1] * region[2];
  const CUdeviceptr dev_ptr = reinterpret_cast<CUdeviceptr>(*buffer_ptr);
  CUresult          err = CUDA_SUCCESS;

  switch (dtype)
  {
    case dType::FLOAT: {
      const auto cval = static_cast<float>(value);
      err = cuMemsetD32(dev_ptr, bitCast<uint32_t>(cval), count);
      break;
    }
    case dType::INT32: {
      const auto cval = static_cast<int32_t>(value);
      err = cuMemsetD32(dev_ptr, bitCast<uint32_t>(cval), count);
      break;
    }
    case dType::UINT32: {
      const auto cval = static_cast<uint32_t>(value);
      err = cuMemsetD32(dev_ptr, cval, count);
      break;
    }
    case dType::INT16: {
      const auto cval = static_cast<int16_t>(value);
      err = cuMemsetD16(dev_ptr, bitCast<uint16_t>(cval), count);
      break;
    }
    case dType::UINT16: {
      const auto cval = static_cast<uint16_t>(value);
      err = cuMemsetD16(dev_ptr, cval, count);
      break;
    }
    case dType::INT8: {
      const auto cval = static_cast<int8_t>(value);
      err = cuMemsetD8(dev_ptr, bitCast<uint8_t>(cval), count);
      break;
    }
    case dType::UINT8: {
      const auto cval = static_cast<uint8_t>(value);
      err = cuMemsetD8(dev_ptr, cval, count);
      break;
    }
    default:
      throw std::runtime_error("Error: Unsupported data type for CUDA setMemory");
  }

  CU_CHECK(err, "Error: Failed to fill memory");
}

auto
CUDABackend::setMemory(const Device::Pointer &       device,
                       const std::shared_ptr<void> & buffer_ptr,
                       std::array<size_t, 3> &       buffer_shape,
                       std::array<size_t, 3> &       buffer_origin,
                       std::array<size_t, 3> &       region,
                       const dType &                 dtype,
                       const mType &                 mtype,
                       const float &                 value) const -> void
{
  // @StRigaud TODO: differentiate IMAGE path when image support lands
  setBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, dtype, value);
}

// ── Kernel build and execution ──────────────────────────────────────────────

auto
CUDABackend::buildKernel(const Device::Pointer & device,
                         const std::string &     kernel_source,
                         const std::string &     kernel_name,
                         std::shared_ptr<void> & kernel) const -> void
{
  CUDAContextGuard ctx(device);
  const auto &     cuda_device = ctx.device();

  const auto source_hash = DiskCache::hash(kernel_source);
  const auto device_hash = DiskCache::hash(cuda_device->getInfo());
  auto &     disk_cache = DiskCache::instance();

  // ── Level 1: in-memory cache (fastest) ──
  const auto cache_key = device_hash + "_" + source_hash + "_" + kernel_name;
  auto       cached = device->getProgramFromCache(cache_key);
  if (cached != nullptr)
  {
    *reinterpret_cast<CUfunction *>(kernel.get()) = *reinterpret_cast<CUfunction *>(cached.get());
    return;
  }

  // ── Level 2: disk cache (avoids recompilation) ──
  std::string ptx;
  if (disk_cache.isEnabled())
  {
    ptx = loadPtxFromCache(device_hash, source_hash);
  }

  // ── Level 3: compile from source ──
  if (ptx.empty())
  {
    ptx = compileToPtx(kernel_source, cuda_device->getArch());

    if (disk_cache.isEnabled())
    {
      savePtxToCache(device_hash, source_hash, ptx);
    }
  }

  // ── Load module and extract function ──
  CUmodule cuModule = nullptr;
  CU_CHECK(cuModuleLoadData(&cuModule, ptx.c_str()), "Error: Failed to load PTX module");

  CUfunction cuFunction = nullptr;
  CU_CHECK(cuModuleGetFunction(&cuFunction, cuModule, kernel_name.c_str()),
           "Error: Failed to get function '" + kernel_name + "' from module");

  // Cache both the function and the module (module must stay alive while
  // the function is in use). We attach the module to the shared_ptr deleter.
  auto function_ptr = std::shared_ptr<void>(new CUfunction(cuFunction), [cuModule](void * p) {
    cuModuleUnload(cuModule);
    delete reinterpret_cast<CUfunction *>(p);
  });
  device->addProgramToCache(cache_key, function_ptr);

  *reinterpret_cast<CUfunction *>(kernel.get()) = cuFunction;
}

auto
CUDABackend::executeKernel(const Device::Pointer &                    device,
                           const std::string &                        kernel_source,
                           const std::string &                        kernel_name,
                           const std::array<size_t, 3> &              global_size,
                           const std::array<size_t, 3> &              local_size,
                           const std::vector<std::shared_ptr<void>> & args,
                           const std::vector<size_t> &                sizes) const -> void
{
  CUDAContextGuard ctx(device);

  // ── Build / retrieve the kernel function ──
  CUfunction            cuFunction = nullptr;
  std::shared_ptr<void> kernel_holder(reinterpret_cast<void *>(&cuFunction), [](void *) { /* no-op, stack-owned */ });
  buildKernel(device, kernel_source, kernel_name, kernel_holder);

  // ── Prepare kernel arguments ──
  // cuLaunchKernel expects an array of pointers, where each element points
  // to the argument value. For us, args[i].get() points to the value.
  std::vector<void *> arg_ptrs(args.size());
  std::transform(args.begin(), args.end(), arg_ptrs.begin(), [](const std::shared_ptr<void> & sp) { return sp.get(); });

  // ── Compute launch configuration ──
  const auto block_size = computeBlockSize(global_size);
  const auto grid_size = computeGridSize(global_size, block_size);

  // ── Launch ──
  CU_CHECK(cuLaunchKernel(cuFunction,
                          static_cast<unsigned>(grid_size[0]),
                          static_cast<unsigned>(grid_size[1]),
                          static_cast<unsigned>(grid_size[2]),
                          static_cast<unsigned>(block_size[0]),
                          static_cast<unsigned>(block_size[1]),
                          static_cast<unsigned>(block_size[2]),
                          0,               // shared memory bytes
                          ctx.stream(),    // stream
                          arg_ptrs.data(), // kernel arguments
                          nullptr),        // extra
           "Error: Failed to launch kernel '" + kernel_name + "'");

  ctx.device()->finish();
}

auto
CUDABackend::getPreamble() const -> std::string
{
  return kernel::preamble_cu;
}

#endif // USE_CUDA

} // namespace cle
