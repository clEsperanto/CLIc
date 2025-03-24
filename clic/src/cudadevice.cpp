#include "device.hpp"
#include "utils.hpp"
#include <iomanip> // for std::setw and std::left

namespace cle
{

#if USE_CUDA

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

auto
CUDADevice::getType() const -> Device::Type
{
  return Device::Type::CUDA;
}

auto
CUDADevice::initialize() -> void
{
  if (isInitialized())
  {
    return;
  }
  auto err = cuDeviceGet(&cudaDevice, 0);
  if (err != CUDA_SUCCESS)
  {
    std::cerr << "Failed to get CUDA device" << std::endl;
    return;
  }
  err = cuCtxCreate(&cudaContext, 0, cudaDevice);
  if (err != CUDA_SUCCESS)
  {
    std::cerr << "Failed to create CUDA context" << std::endl;
    return;
  }
  err = cuStreamCreate(&cudaStream, CU_STREAM_DEFAULT);
  if (err != CUDA_SUCCESS)
  {
    std::cerr << "Failed to create CUDA stream" << std::endl;
    return;
  }
  initialized = true;
}

auto
CUDADevice::finalize() -> void
{
  if (!isInitialized())
  {
    std::cerr << "CUDA device not initialized" << std::endl;
    return;
  }
  this->waitFinish = true;
  this->finish();
  cuStreamDestroy(cudaStream);
  cuCtxDestroy(cudaContext);
  cuCtxSetCurrent(nullptr);
  initialized = false;
}

auto
CUDADevice::finish() const -> void
{
  if (!isInitialized())
  {
    std::cerr << "CUDA device not initialized" << std::endl;
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
  this->waitFinish = flag;
}

[[nodiscard]] auto
CUDADevice::getPlatform() const -> const std::string
{
  return "NVIDIA";
}

auto
CUDADevice::isInitialized() const -> bool
{
  return initialized;
}

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
CUDADevice::getName(bool lowercase) const -> std::string
{
  char device_name[256];
  cuDeviceGetName(device_name, sizeof(device_name), cudaDevice);
  if (lowercase)
  {
    return to_lower(std::string(device_name));
  }
  return std::string(device_name);
}


[[nodiscard]] auto
getDeviceType() const -> std::string
{
  return "gpu";
}

auto
CUDADevice::getArch() const -> std::string
{
  int major = 0, minor = 0;
  cuDeviceGetAttribute(&major, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR, this->getCUDADevice());
  cuDeviceGetAttribute(&minor, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR, this->getCUDADevice());
  return std::to_string(major) + std::to_string(minor);
}

auto
CUDADevice::supportImage() const -> const bool
{
  return true;
}

auto
CUDADevice::getMaximumBufferSize() const -> size_t
{
  size_t mem_size;
  cuDeviceGetAttribute(&mem_size, CU_DEVICE_ATTRIBUTE_MAX_MEM_ALLOC_SIZE, device);
  return mem_size;
}

auto
CUDADevice::getLocalMemorySize() const -> size_t
{
  size_t mem_size;
  cuDeviceGetAttribute(&mem_size, CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_BLOCK, cudaDevice);
  return mem_size;
}

auto
CUDADevice::getInfo() const -> std::string
{
  std::ostringstream result;
  size_t             totalGlobalMem;
  int driverVersion, sharedMemPerBlock, runtimeVersion, regsPerBlock, warpSize, maxThreadsPerBlock, totalConstMem,
    major, minor, clockRate, textureAlignment, multiProcessorCount, maxBlockDimX, maxBlockDimY, maxBlockDimZ,
    maxGridDimX, maxGridDimY, maxGridDimZ;

  cudaRuntimeGetVersion(&runtimeVersion);
  cuDriverGetVersion(&driverVersion);

  cuDeviceTotalMem_v2(&totalGlobalMem, cudaDevice);
  cuDeviceGetAttribute(&sharedMemPerBlock, CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_BLOCK, cudaDevice);
  cuDeviceGetAttribute(&regsPerBlock, CU_DEVICE_ATTRIBUTE_MAX_REGISTERS_PER_BLOCK, cudaDevice);
  cuDeviceGetAttribute(&warpSize, CU_DEVICE_ATTRIBUTE_WARP_SIZE, cudaDevice);
  cuDeviceGetAttribute(&maxThreadsPerBlock, CU_DEVICE_ATTRIBUTE_MAX_THREADS_PER_BLOCK, cudaDevice);
  cuDeviceGetAttribute(&totalConstMem, CU_DEVICE_ATTRIBUTE_TOTAL_CONSTANT_MEMORY, cudaDevice);
  cuDeviceGetAttribute(&major, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR, cudaDevice);
  cuDeviceGetAttribute(&minor, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR, cudaDevice);
  cuDeviceGetAttribute(&clockRate, CU_DEVICE_ATTRIBUTE_CLOCK_RATE, cudaDevice);
  cuDeviceGetAttribute(&textureAlignment, CU_DEVICE_ATTRIBUTE_TEXTURE_ALIGNMENT, cudaDevice);
  cuDeviceGetAttribute(&multiProcessorCount, CU_DEVICE_ATTRIBUTE_MULTIPROCESSOR_COUNT, cudaDevice);
  cuDeviceGetAttribute(&maxBlockDimX, CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_X, cudaDevice);
  cuDeviceGetAttribute(&maxBlockDimY, CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_Y, cudaDevice);
  cuDeviceGetAttribute(&maxBlockDimZ, CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_Z, cudaDevice);
  cuDeviceGetAttribute(&maxGridDimX, CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_X, cudaDevice);
  cuDeviceGetAttribute(&maxGridDimY, CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_Y, cudaDevice);
  cuDeviceGetAttribute(&maxGridDimZ, CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_Z, cudaDevice);

  result << "(" << this->getType() << ") " << getName() << " (" << static_cast<float>(runtimeVersion) / 1000 << ")\n";
  result << std::left << std::setw(30) << "\tVendor:"
         << "NVIDIA Corporation"
         << "\n";
  result << std::left << std::setw(30) << "\tDevice index:" << cudaDeviceIndex << "\n";

  result << std::left << std::setw(30) << "\tDriver version:" << static_cast<float>(driverVersion) / 1000 << "\n";
  result << std::left << std::setw(30) << "\tDevice type:"
         << "GPU"
         << "\n";
  result << std::left << std::setw(30) << "\tMultiprocessor count:" << multiProcessorCount << "\n";

  result << std::left << std::setw(30) << "\tTotal global memory:" << totalGlobalMem / (1024 * 1024) << " MB\n";
  result << std::left << std::setw(30) << "\tShared memory per block:" << sharedMemPerBlock / 1024 << " KB\n";
  result << std::left << std::setw(30) << "\tClock rate:" << clockRate / 1000 << " MHz"
         << "\n";
  result << std::left << std::setw(30) << "\tTotal constant memory:" << totalConstMem / 1024 << " KB\n";
  result << std::left << std::setw(30) << "\tRegisters per block:" << regsPerBlock << "\n";
  result << std::left << std::setw(30) << "\tWarp size:" << warpSize << "\n";
  result << std::left << std::setw(30) << "\tMax threads per block:" << maxThreadsPerBlock << "\n";
  result << std::left << std::setw(30) << "\tMax block dimension:" << maxBlockDimX << ", " << maxBlockDimY << ", "
         << maxBlockDimZ << "\n";
  result << std::left << std::setw(30) << "\tMax grid dimension:" << maxGridDimX << ", " << maxGridDimY << ", "
         << maxGridDimZ << "\n";
  result << std::left << std::setw(30) << "\tCompute capability version:" << major << "." << minor << "\n";
  result << std::left << std::setw(30) << "\tTexture alignment:" << textureAlignment << "\n";

  return result.str();
}

auto
CUDADevice::getInfoExtended() const -> std::string
{
  return getInfo();
}

#endif // USE_CUDA

} // namespace cle
