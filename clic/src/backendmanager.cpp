#include "backend.hpp"
#include <functional>
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
  // Define a map to associate backend names with their check functions
  static std::map<std::string, std::function<bool()>> backendCheckMap = {
    { "cuda", [cudaEnabled = cudaEnabled]() { return cudaEnabled(); } },
    { "opencl", [openCLEnabled = openCLEnabled]() { return openCLEnabled(); } },
    { "metal", [metalEnabled = metalEnabled]() { return metalEnabled(); } }
  };

  std::vector<std::string> availableBackends;

  // Iterate over the map and check each backend
  for (const auto & [backend, checkFunction] : backendCheckMap)
  {
    if (checkFunction())
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
  // Define a map to associate backend names with their types and creation functions.
  // Only register backends that were enabled at compile time.
  static std::map<std::string, std::pair<Backend::Type, std::function<std::unique_ptr<Backend>()>>> backendMap = {
#if USE_CUDA
    { "cuda", { Backend::Type::CUDA, []() -> std::unique_ptr<Backend> { return std::make_unique<CUDABackend>(); } } },
#endif
#if USE_OPENCL
    { "opencl", { Backend::Type::OPENCL, []() -> std::unique_ptr<Backend> { return std::make_unique<OpenCLBackend>(); } } },
#endif
#if USE_METAL
    { "metal", { Backend::Type::METAL, []() -> std::unique_ptr<Backend> { return std::make_unique<MetalBackend>(); } } },
#endif
  };

  // Check if the requested backend is known (i.e. compiled in)
  if (backendMap.find(backend) == backendMap.end())
  {
    throw std::runtime_error("Unknown backend '" + backend + "'. This build supports: " + [&]() {
      std::string list;
      for (const auto & [name, _] : backendMap)
      {
        if (!list.empty())
          list += ", ";
        list += name;
      }
      return list.empty() ? "none" : list;
    }());
  }

  // Get the type and creation function for the requested backend
  auto [backend_type, createBackend] = backendMap[backend];

  // Check if the requested backend is available at runtime
  bool isEnabled = false;
  switch (backend_type)
  {    
    case Backend::Type::CUDA:
      if (!cudaEnabled())
      {
        const std::string & errorReason = getCudaError();
        throw std::runtime_error("Backend '" + backend + "' is not available: " + errorReason);
      }
      isEnabled = true;
      break;
    case Backend::Type::OPENCL:
      if (!openCLEnabled())
      {
        const std::string & errorReason = getOpenCLError();
        throw std::runtime_error("Backend '" + backend + "' is not available: " + errorReason);
      }
      isEnabled = true;
      break;
    case Backend::Type::METAL:
      if (!metalEnabled())
      {
        const std::string & errorReason = getMetalError();
        throw std::runtime_error("Backend '" + backend + "' is not available: " + errorReason);
      }
      isEnabled = true;
      break;
    default:
      throw std::runtime_error("Unsupported backend type");
  }
  if (!isEnabled)
  {
    const std::string & errorReason = (backend_type == Backend::Type::CUDA) ? getCudaError() : getOpenCLError();
    throw std::runtime_error("Backend '" + backend + "' is not available: " + errorReason);
  }

  // Create the requested backend
  this->backend = createBackend();
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
