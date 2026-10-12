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
 * @brief Create the memory pool used for stream-ordered allocations (cuMemAllocFromPoolAsync) on a device.
 *
 * A pool of our own, so that the attributes of the device's default pool, shared with other libraries of the
 * process (CuPy, PyTorch), are left untouched. Freed memory is kept for reuse up to 1/8 of the device memory; the
 * rest is returned to the driver at the next synchronisation.
 *
 * @return the pool, or nullptr if the device or driver does not support memory pools (allocations then fall back
 *         to synchronous cuMemAlloc / cuMemFree)
 */
[[nodiscard]] static auto
createMemoryPool(CUdevice device, int device_index) -> CUmemoryPool
{
  if (queryDeviceAttribute(CU_DEVICE_ATTRIBUTE_MEMORY_POOLS_SUPPORTED, device) == 0)
  {
    return nullptr;
  }
  CUmemPoolProps props = {};
  props.allocType = CU_MEM_ALLOCATION_TYPE_PINNED;
  props.handleTypes = CU_MEM_HANDLE_TYPE_NONE;
  props.location.type = CU_MEM_LOCATION_TYPE_DEVICE;
  props.location.id = device_index;

  CUmemoryPool pool = nullptr;
  if (cuMemPoolCreate(&pool, &props) != CUDA_SUCCESS)
  {
    return nullptr;
  }
  size_t total_mem = 0;
  if (cuDeviceTotalMem_v2(&total_mem, device) == CUDA_SUCCESS)
  {
    cuuint64_t threshold = total_mem / 8;
    cuMemPoolSetAttribute(pool, CU_MEMPOOL_ATTR_RELEASE_THRESHOLD, &threshold);
  }
  return pool;
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

  // The device's primary context, as used by the CUDA runtime and by CuPy / PyTorch: memory and streams exchanged with
  // them over DLPack belong to the same context, and the process keeps one context per device. It is shared, so
  // finalize() releases it (it is destroyed when its last user releases it) and must free what it created in it.
  err = cuDevicePrimaryCtxRetain(&cudaContext, cudaDevice);
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to retain the CUDA primary context of device " + std::to_string(cudaDeviceIndex));
  }

  // Unlike cuCtxCreate, retaining does not make the context current: needed for the stream and the event
  err = cuCtxSetCurrent(cudaContext);
  if (err == CUDA_SUCCESS)
  {
    err = cuStreamCreate(&cudaStream, CU_STREAM_DEFAULT);
  }
  if (err != CUDA_SUCCESS)
  {
    cuDevicePrimaryCtxRelease(cudaDevice);
    cudaContext = nullptr;
    throw std::runtime_error("Error: Failed to create CUDA stream for device " + std::to_string(cudaDeviceIndex));
  }

  memoryPool = createMemoryPool(cudaDevice, cudaDeviceIndex);
  if (cuEventCreate(&syncEvent, CU_EVENT_DISABLE_TIMING) != CUDA_SUCCESS)
  {
    syncEvent = nullptr; // syncToStream then uses a temporary event
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

  // Wait for the pending work before releasing the stream, whatever the wait setting
  cuStreamSynchronize(cudaStream);

  // Release the cached kernels while their context is alive and current: their deleters unload the modules
  // (cuModuleUnload acts on the current context). Unloading them after the context is gone crashes the driver; and the
  // primary context may outlive this device (other users), so the modules must be unloaded explicitly anyway.
  if (cuCtxSetCurrent(cudaContext) == CUDA_SUCCESS)
  {
    cache = std::make_shared<ProgramCache>();
  }

  initialized = false; // Mark before releasing the context, so memory deleters skip their free

  if (memoryPool != nullptr)
  {
    // memory still allocated from the pool is released with it once freed, or with the context
    cuMemPoolDestroy(memoryPool);
    memoryPool = nullptr;
  }

  if (syncEvent != nullptr)
  {
    cuEventDestroy(syncEvent); // waits already queued on other streams are not affected
    syncEvent = nullptr;
  }

  if (cudaStream != nullptr)
  {
    CUresult err = cuStreamDestroy(cudaStream);
    cudaStream = nullptr;
    // If CUDA driver is already shut down, just bail
    if (err == CUDA_ERROR_DEINITIALIZED)
      return;
  }

  if (cudaContext != nullptr)
  {
    // leave no dangling current context on this thread if the release destroys it (last user)
    CUcontext current = nullptr;
    if (cuCtxGetCurrent(&current) == CUDA_SUCCESS && current == cudaContext)
    {
      cuCtxSetCurrent(nullptr);
    }
    cuDevicePrimaryCtxRelease(cudaDevice);
    cudaContext = nullptr;
  }
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
CUDADevice::getPlatform() const -> std::string
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
CUDADevice::getNbDevicesFromContext() const -> size_t
{
  return 1;
}

auto
CUDADevice::getDeviceIndex() const -> size_t
{
  return static_cast<size_t>(cudaDeviceIndex);
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

auto
CUDADevice::getCUDAMemoryPool() const -> CUmemoryPool
{
  return memoryPool;
}

auto
CUDADevice::getCUDASyncEvent() const -> CUevent
{
  return syncEvent;
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

#endif // USE_CUDA

} // namespace cle
