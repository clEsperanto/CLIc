#include "device.hpp"

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
    // std::cerr << "CUDA device already initialized" << std::endl;
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
  finish();
  cuStreamDestroy(cudaStream);
  cuCtxDestroy(cudaContext);
  cuCtxSetCurrent(nullptr);
  initialized = false;
}

auto
CUDADevice::finish() -> void
{
  if (!isInitialized())
  {
    std::cerr << "CUDA device not initialized" << std::endl;
    return;
  }
  cuStreamSynchronize(cudaStream);
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
CUDADevice::getName() const -> std::string
{
  char device_name[256];
  cuDeviceGetName(device_name, sizeof(device_name), cudaDevice);
  return std::string(device_name);
}

auto
CUDADevice::getInfo() const -> std::string
{
  int    numMultiprocessors;
  int    driverVersion;
  int    driverMajor;
  int    driverMinor;
  size_t totalMemory;

  cuDriverGetVersion(&driverVersion);
  cuDeviceTotalMem(&totalMemory, cudaDevice);
  cuDeviceGetAttribute(&numMultiprocessors, CU_DEVICE_ATTRIBUTE_MULTIPROCESSOR_COUNT, cudaDevice);
  driverMajor = driverVersion / 1000;
  driverMinor = (driverVersion % 1000) / 10;

  std::ostringstream result;
  result << getName() << " (" << driverMajor << "." << driverMinor << ")\n";
  result << "\tType: GPU\n";
  result << "\tCompute Units: " << numMultiprocessors << '\n';
  result << "\tGlobal Memory Size: " << (totalMemory / (1000 * 1000)) << " MB\n";
  return result.str();
}

auto
CUDADevice::getCache() -> std::map<std::string, CUmodule> &
{
  return this->cache;
}

#endif // USE_CUDA

} // namespace cle
