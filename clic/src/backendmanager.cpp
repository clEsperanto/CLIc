#include "backend.hpp"
#include <map>
#include <sstream>

namespace cle
{

// Static error message storage
std::string BackendManager::cudaErrorMsg = "";
std::string BackendManager::openCLErrorMsg = "";
std::string BackendManager::metalErrorMsg = "";

auto
BackendManager::cudaEnabled() -> bool
{
#if USE_CUDA
  CUresult initErr = cuInit(0);
  if (initErr != CUDA_SUCCESS)
  {
    const char * errStr = nullptr;
    cuGetErrorString(initErr, &errStr);
    cudaErrorMsg = std::string(errStr ? errStr : "unknown error");
    cudaErrorMsg += std::string(" (cuInit failed)");
    return false;
  }
  int      deviceCount = 0;
  CUresult countErr = cuDeviceGetCount(&deviceCount);
  if (countErr != CUDA_SUCCESS)
  {
    const char * errStr = nullptr;
    cuGetErrorString(countErr, &errStr);
    cudaErrorMsg = std::string(errStr ? errStr : "unknown error");
    cudaErrorMsg += std::string(" (cuDeviceGetCount failed)");
    return false;
  }
  if (deviceCount == 0)
  {
    cudaErrorMsg = "No CUDA devices found";
    return false;
  }
  cudaErrorMsg = ""; // Clear error on success
  return true;
#else
  cudaErrorMsg = "CUDA not compiled into this build (USE_CUDA=OFF)";
  return false;
#endif
}

auto
BackendManager::openCLEnabled() -> bool
{
#if USE_OPENCL
  cl_uint platformCount = 0;
  cl_int  err = clGetPlatformIDs(0, nullptr, &platformCount);
  if (err != CL_SUCCESS)
  {
    std::ostringstream oss;
    oss << "clGetPlatformIDs failed with error code " << err;
    openCLErrorMsg = oss.str();
    return false;
  }
  if (platformCount == 0)
  {
    openCLErrorMsg = "No OpenCL platforms found";
    return false;
  }
  openCLErrorMsg = ""; // Clear error on success
  return true;
#else
  openCLErrorMsg = "OpenCL not compiled into this build (USE_OPENCL=OFF)";
  return false;
#endif
}

auto
BackendManager::metalEnabled() -> bool
{
#if USE_METAL
  // Metal availability is determined at compile time, so if we're here, it's enabled.
  return true;
#else
  metalErrorMsg = "Metal not compiled into this build (USE_METAL=OFF)";
  return false;
#endif
}

auto
BackendManager::getBackendsList() -> std::vector<std::string>
{
  static const std::map<std::string, bool (*)()> backendChecks = { { "cuda", cudaEnabled },
                                                                   { "opencl", openCLEnabled },
                                                                   { "metal", metalEnabled } };

  std::vector<std::string> availableBackends;
  for (const auto & [backend, isEnabled] : backendChecks)
  {
    if (isEnabled())
    {
      availableBackends.push_back(backend);
    }
  }
  return availableBackends;
}

auto
BackendManager::getInstance() -> BackendManager &
{
  static BackendManager instance;
  return instance;
}


auto
BackendManager::setBackend(const std::string & backend) -> void
{
  // Backends enabled at compile time, with their runtime availability check, its error message and their factory
  struct BackendEntry
  {
    bool (*isEnabled)();
    const std::string & (*error)();
    std::unique_ptr<Backend> (*create)();
  };
  static const std::map<std::string, BackendEntry> backendMap = {
#if USE_CUDA
    { "cuda", { cudaEnabled, getCudaError, []() -> std::unique_ptr<Backend> { return std::make_unique<CUDABackend>(); } } },
#endif
#if USE_OPENCL
    { "opencl", { openCLEnabled, getOpenCLError, []() -> std::unique_ptr<Backend> { return std::make_unique<OpenCLBackend>(); } } },
#endif
#if USE_METAL
    { "metal", { metalEnabled, getMetalError, []() -> std::unique_ptr<Backend> { return std::make_unique<MetalBackend>(); } } },
#endif
  };

  const auto entry = backendMap.find(backend);
  if (entry == backendMap.end())
  {
    std::string list;
    for (const auto & [name, _] : backendMap)
    {
      list += (list.empty() ? "" : ", ") + name;
    }
    throw std::runtime_error("Unknown backend '" + backend + "'. This build supports: " + (list.empty() ? "none" : list));
  }
  if (!entry->second.isEnabled())
  {
    throw std::runtime_error("Backend '" + backend + "' is not available: " + entry->second.error());
  }
  this->backend = entry->second.create();
}

auto
BackendManager::getCudaError() -> const std::string &
{
  return cudaErrorMsg;
}

auto
BackendManager::getOpenCLError() -> const std::string &
{
  return openCLErrorMsg;
}

auto
BackendManager::getMetalError() -> const std::string &
{
  return metalErrorMsg;
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
