#include "device.hpp"
#include "utils.hpp"

#include <iomanip>
#include <sstream>
#include <string>

namespace cle
{

#if USE_CUDA

// ============================================================================
// Internal helpers
// ============================================================================

/**
 * @brief Query a single integer device attribute, returning 0 on failure.
 *
 * Centralises error handling for the many cuDeviceGetAttribute calls throughout
 * the class, avoiding repeated error-check boilerplate.
 */
[[nodiscard]] static auto
queryDeviceAttribute(CUdevice_attribute attrib, CUdevice device) -> int
{
  int      value = 0;
  CUresult err = cuDeviceGetAttribute(&value, attrib, device);
  if (err != CUDA_SUCCESS)
  {
    const char * err_str = nullptr;
    cuGetErrorString(err, &err_str);
    std::cerr << "Warning: cuDeviceGetAttribute failed for attribute " << attrib << ": " << (err_str ? err_str : "unknown error") << " ("
              << err << ")" << std::endl;
  }
  return value;
}

/**
 * @brief Format a labelled info line for device info output.
 */
static auto
infoLine(std::ostringstream & os, const std::string & label, const std::string & value) -> void
{
  os << std::left << std::setw(30) << ("\t" + label) << value << "\n";
}

// ============================================================================
// Construction / Destruction
// ============================================================================

CUDADevice::CUDADevice(int deviceIndex)
  : cudaDeviceIndex(deviceIndex)
{
  initialize();
}

CUDADevice::~CUDADevice()
{
  if (isInitialized())
  {
    finalize();
  }
}

// ============================================================================
// Lifecycle
// ============================================================================

auto
CUDADevice::initialize() -> void
{
  if (isInitialized())
  {
    return;
  }

  CUresult err = cuDeviceGet(&cudaDevice, cudaDeviceIndex);
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to get CUDA device at index " + std::to_string(cudaDeviceIndex));
  }

  err = cuCtxCreate(&cudaContext, 0, cudaDevice);
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to create CUDA context for device " + std::to_string(cudaDeviceIndex));
  }

  err = cuStreamCreate(&cudaStream, CU_STREAM_DEFAULT);
  if (err != CUDA_SUCCESS)
  {
    // Clean up the context we just created before throwing
    cuCtxDestroy(cudaContext);
    cudaContext = nullptr;
    throw std::runtime_error("Error: Failed to create CUDA stream for device " + std::to_string(cudaDeviceIndex));
  }

  initialized = true;
}

auto
CUDADevice::finalize() -> void
{
  if (!isInitialized())
  {
    return;
  }

  // Ensure all pending work completes before tearing down resources
  waitFinish = true;
  finish();

  if (cudaStream != nullptr)
  {
    cuStreamDestroy(cudaStream);
    cudaStream = nullptr;
  }

  if (cudaContext != nullptr)
  {
    cuCtxDestroy(cudaContext);
    cudaContext = nullptr;
  }

  initialized = false;
}

auto
CUDADevice::finish() const -> void
{
  if (!isInitialized())
  {
    return;
  }
  if (waitFinish)
  {
    cuStreamSynchronize(cudaStream);
  }
}

auto
CUDADevice::setWaitToFinish(bool flag) -> void
{
  waitFinish = flag;
}

auto
CUDADevice::isInitialized() const -> bool
{
  return initialized;
}

// ============================================================================
// Device properties
// ============================================================================

auto
CUDADevice::getType() const -> Device::Type
{
  return Device::Type::CUDA;
}

auto
CUDADevice::getDeviceType() const -> std::string
{
  return "gpu";
}

[[nodiscard]] auto
CUDADevice::getPlatform() const -> const std::string
{
  return "NVIDIA";
}

auto
CUDADevice::getName(bool lowercase) const -> std::string
{
  char     device_name[256] = {}; // Zero-initialise for guaranteed null termination
  CUresult err = cuDeviceGetName(device_name, sizeof(device_name), cudaDevice);
  if (err != CUDA_SUCCESS)
  {
    return lowercase ? "unknown cuda device" : "Unknown CUDA Device";
  }
  std::string name(device_name);
  return lowercase ? to_lower(name) : name;
}

auto
CUDADevice::getArch() const -> std::string
{
  const int major = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR, cudaDevice);
  const int minor = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR, cudaDevice);
  return std::to_string(major) + std::to_string(minor);
}

auto
CUDADevice::supportImage() const -> bool
{
  return true;
}

auto
CUDADevice::getMaximumBufferSize() const -> size_t
{
  // cuDeviceTotalMem gives the total GPU memory, which is a better proxy for
  // maximum allocation than the attribute (which may not exist on all drivers).
  size_t   total_mem = 0;
  CUresult err = cuDeviceTotalMem_v2(&total_mem, cudaDevice);
  if (err != CUDA_SUCCESS)
  {
    return 0;
  }
  return total_mem;
}

auto
CUDADevice::getLocalMemorySize() const -> size_t
{
  return static_cast<size_t>(queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_BLOCK, cudaDevice));
}

auto
CUDADevice::getMaximumWorkGroupSize() const -> size_t
{
  return static_cast<size_t>(queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_MAX_THREADS_PER_BLOCK, cudaDevice));
}

// ============================================================================
// Accessors for CUDA handles
// ============================================================================

auto
CUDADevice::getCUDADeviceIndex() const -> int
{
  return cudaDeviceIndex;
}

auto
CUDADevice::getCUDADevice() const -> const CUdevice &
{
  return cudaDevice;
}

auto
CUDADevice::getCUDAContext() const -> const CUcontext &
{
  return cudaContext;
}

auto
CUDADevice::getCUDAStream() const -> const CUstream &
{
  return cudaStream;
}

// ============================================================================
// Device info
// ============================================================================

auto
CUDADevice::getInfo() const -> std::string
{
  std::ostringstream result;

  // ── Version info ──
  int driverVersion = 0;
  cuDriverGetVersion(&driverVersion);

  // ── Memory info ──
  size_t totalGlobalMem = 0;
  cuDeviceTotalMem_v2(&totalGlobalMem, cudaDevice);

  // ── Device attributes ──
  const int sharedMemPerBlock = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_BLOCK, cudaDevice);
  const int regsPerBlock = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_MAX_REGISTERS_PER_BLOCK, cudaDevice);
  const int warpSize = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_WARP_SIZE, cudaDevice);
  const int maxThreadsPerBlock = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_MAX_THREADS_PER_BLOCK, cudaDevice);
  const int totalConstMem = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_TOTAL_CONSTANT_MEMORY, cudaDevice);
  const int major = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR, cudaDevice);
  const int minor = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR, cudaDevice);
  const int clockRate = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_CLOCK_RATE, cudaDevice);
  const int textureAlignment = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_TEXTURE_ALIGNMENT, cudaDevice);
  const int multiProcCount = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_MULTIPROCESSOR_COUNT, cudaDevice);
  const int maxBlockDimX = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_X, cudaDevice);
  const int maxBlockDimY = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_Y, cudaDevice);
  const int maxBlockDimZ = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_Z, cudaDevice);
  const int maxGridDimX = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_X, cudaDevice);
  const int maxGridDimY = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_Y, cudaDevice);
  const int maxGridDimZ = queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_Z, cudaDevice);

  // ── Format output ──
  const float driverVersionF = static_cast<float>(driverVersion) / 1000.0f;

  result << "(" << this->getType() << ") " << getName() << " (driver " << std::fixed << std::setprecision(1) << driverVersionF << ")\n";

  infoLine(result, "Vendor:", "NVIDIA Corporation");
  infoLine(result, "Device index:", std::to_string(cudaDeviceIndex));
  infoLine(result, "Driver version:", std::to_string(driverVersionF));
  infoLine(result, "Device type:", "GPU");
  infoLine(result, "Multiprocessor count:", std::to_string(multiProcCount));
  infoLine(result, "Total global memory:", std::to_string(totalGlobalMem / (1024 * 1024)) + " MB");
  infoLine(result, "Shared memory per block:", std::to_string(sharedMemPerBlock / 1024) + " KB");
  infoLine(result, "Clock rate:", std::to_string(clockRate / 1000) + " MHz");
  infoLine(result, "Total constant memory:", std::to_string(totalConstMem / 1024) + " KB");
  infoLine(result, "Registers per block:", std::to_string(regsPerBlock));
  infoLine(result, "Warp size:", std::to_string(warpSize));
  infoLine(result, "Max threads per block:", std::to_string(maxThreadsPerBlock));
  infoLine(result,
           "Max block dimension:",
           std::to_string(maxBlockDimX) + ", " + std::to_string(maxBlockDimY) + ", " + std::to_string(maxBlockDimZ));
  infoLine(
    result, "Max grid dimension:", std::to_string(maxGridDimX) + ", " + std::to_string(maxGridDimY) + ", " + std::to_string(maxGridDimZ));
  infoLine(result, "Compute capability:", std::to_string(major) + "." + std::to_string(minor));
  infoLine(result, "Texture alignment:", std::to_string(textureAlignment));

  return result.str();
}

auto
CUDADevice::getInfoExtended() const -> std::string
{
  return getInfo();
}

// ============================================================================
// Program cache
// ============================================================================

auto
CUDADevice::getProgramFromCache(const std::string & key) const -> std::shared_ptr<void>
{
  if (!cache)
  {
    return nullptr;
  }
  return cache->get(key);
}

auto
CUDADevice::addProgramToCache(const std::string & key, std::shared_ptr<void> program) -> void
{
  if (!cache)
  {
    return;
  }
  cache->put(key, std::move(program));
}

#endif // USE_CUDA

} // namespace cle
