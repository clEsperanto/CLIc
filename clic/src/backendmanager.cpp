#include "backend.hpp"

#include <vector>

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
  Backend::Type backend_type;
  if (backend == "cuda")
  {
    if (cudaEnabled())
    {
      backend_type = Backend::Type::CUDA;
    }
    else
    {
      std::cerr << "Warning: 'CUDA' backend not available. Switching to 'OpenCL'." << std::endl;
      backend_type = Backend::Type::OPENCL;
    }
  }
  else if (backend == "opencl")
  {
    if (openCLEnabled())
    {
      backend_type = Backend::Type::OPENCL;
    }
    else
    {
      std::cerr << "Warning: 'OpenCL' backend not available. Switching to 'CUDA'." << std::endl;
      backend_type = Backend::Type::CUDA;
    }
  }
  switch (backend_type)
  {
    case Backend::Type::CUDA:
      this->backend = std::make_unique<CUDABackend>();
      break;
    case Backend::Type::OPENCL:
      this->backend = std::make_unique<OpenCLBackend>();
      break;
    default:
      this->backend = std::make_unique<OpenCLBackend>();
      std::cerr << "Warning: Using 'OpenCL' as default backend." << std::endl;
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
