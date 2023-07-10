#include "backend.hpp"

namespace cle
{

auto
BackendManager::getBackendsList() -> std::vector<std::string>
{
  std::vector<std::string> availableBackend;
  if (cudaEnabled())
  {
    availableBackend.push_back("cuda");
  }
  if (openCLEnabled())
  {
    availableBackend.push_back("opencl");
  }
  return availableBackend;
}

auto
BackendManager::getInstance() -> BackendManager &
{
  static BackendManager instance;
  return instance;
}

auto
BackendManager::cudaEnabled() -> bool
{
#if USE_CUDA
  cuInit(0);
  int  deviceCount = 0;
  auto error = cuDeviceGetCount(&deviceCount);
  if (error != CUDA_SUCCESS)
  {
    return false;
  }
  return deviceCount > 0;
#else
  return false;
#endif
}

auto
BackendManager::openCLEnabled() -> bool
{
#if USE_OPENCL
  cl_uint platformCount = 0;
  clGetPlatformIDs(0, nullptr, &platformCount);
  return platformCount > 0;
#else
  return false;
#endif
}

auto
BackendManager::setBackend(const std::string & backend) -> void
{
  if (cudaEnabled() && backend == "cuda")
  {
    this->backend = std::make_unique<CUDABackend>();
  }
  else if (openCLEnabled())
  {
    this->backend = std::make_unique<OpenCLBackend>();
  }
  else
  {
    throw std::runtime_error("No backend available.");
  }
}

auto
BackendManager::getBackend() const -> const Backend &
{
  if (!this->backend)
  {
    throw std::runtime_error("Backend not selected.");
  }
  return *this->backend;
}

} // namespace cle
