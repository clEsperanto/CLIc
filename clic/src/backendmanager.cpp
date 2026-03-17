#include "backend.hpp"
#include <functional>
#include <map>

namespace cle
{

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
BackendManager::getBackendsList() -> std::vector<std::string>
{
  // Define a map to associate backend names with their check functions
  static std::map<std::string, std::function<bool()>> backendCheckMap = {
    { "cuda", [cudaEnabled = cudaEnabled]() { return cudaEnabled(); } },
    { "opencl", [openCLEnabled = openCLEnabled]() { return openCLEnabled(); } }
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
  };

  // Check if the requested backend is known (i.e. compiled in)
  if (backendMap.find(backend) == backendMap.end())
  {
    throw std::runtime_error("Unknown backend '" + backend + "'. This build supports: " +
                             [&]() {
                               std::string list;
                               for (const auto & [name, _] : backendMap)
                               {
                                 if (!list.empty()) list += ", ";
                                 list += name;
                               }
                               return list.empty() ? "none" : list;
                             }());
  }

  // Get the type and creation function for the requested backend
  auto [backend_type, createBackend] = backendMap[backend];

  // Check if the requested backend is available at runtime
  bool isEnabled = (backend_type == Backend::Type::CUDA) ? cudaEnabled() : openCLEnabled();
  if (!isEnabled)
  {
    throw std::runtime_error("Backend '" + backend +
                             "' is not available. No compatible device found or runtime not installed.");
  }

  // Create the requested backend
  this->backend = createBackend();
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
