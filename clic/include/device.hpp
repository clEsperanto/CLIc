#ifndef __INCLUDE_DEVICE_HPP
#define __INCLUDE_DEVICE_HPP

#include "clic.hpp"

#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <unordered_map>

namespace cle
{

/**
 * @brief Cache struct for pre-compiled programs and Least Recently Used (LRU) management
 * This structure stores programs pointers in a unordered_map with a maximum size (64), allowing devices
 * to search the cache before compiling a new program. If not found, the new program is added to the cache.
 * The cache uses an LRU list to track usage and evict the least recently used program when the cache is full.
 */
struct Cache
{
  static constexpr size_t MAX_PROGRAM_CACHE_SIZE = 64;

  // Store program and its position in LRU list
  struct Entry
  {
    std::shared_ptr<void>            program;
    std::list<std::string>::iterator lru_it;
  };

  std::unordered_map<std::string, Entry> program_cache;
  std::list<std::string>                 program_lru;

  Cache() { program_cache.reserve(MAX_PROGRAM_CACHE_SIZE); }
  ~Cache() = default;

  /**
   * @brief Cache a program with a given key
   * @param key The key to identify the program
   * @param program The program pointer to cache
   */
  auto
  cacheProgram(const std::string & key, const std::shared_ptr<void> & program) -> void
  {
    auto it = program_cache.find(key);
    if (it != program_cache.end())
    {
      // Move key to back (most recently used)
      program_lru.erase(it->second.lru_it);
      program_lru.push_back(key);
      it->second.lru_it = std::prev(program_lru.end());
      it->second.program = program;
      return;
    }
    if (program_cache.size() >= MAX_PROGRAM_CACHE_SIZE)
    {
      // Remove oldest
      auto oldest = program_lru.front();
      program_lru.pop_front();
      program_cache.erase(oldest);
    }
    program_lru.push_back(key);
    program_cache[key] = { program, std::prev(program_lru.end()) };
  }

  /**
   * @brief Retrieve a cached program by key
   * @param key The key to identify the program
   * @return The program pointer if found, nullptr otherwise
   */
  auto
  getCachedProgram(const std::string & key) -> std::shared_ptr<void>
  {
    auto it = program_cache.find(key);
    if (it != program_cache.end())
    {
      // Move accessed key to back (most recently used)
      program_lru.erase(it->second.lru_it);
      program_lru.push_back(key);
      it->second.lru_it = std::prev(program_lru.end());
      return it->second.program;
    }
    return nullptr;
  }
};

/**
 * @brief Device class
 *        This class is used to manage devices
 */
class Device
{
public:
  /**
   * @brief Device type
   */
  enum class Type
  {
    CUDA,
    OPENCL
  };

  using Pointer = std::shared_ptr<Device>;

  /**
   * @brief Construct a new Device object
   */
  Device() = default;

  /**
   * @brief Destroy the Device object
   */
  virtual ~Device() = default;

  /**
   * @brief Initialize a device pointer
   */
  virtual auto
  initialize() -> void = 0;

  /**
   * @brief Release the device pointer
   */
  virtual auto
  finalize() -> void = 0;

  /**
   * @brief Wait for command queue to finish
   */
  virtual auto
  finish() const -> void = 0;

  /**
   * @brief Set flag for waiting for command queue to finish
   */
  virtual auto
  setWaitToFinish(bool flag) -> void = 0;

  /**
   * @brief Check if device is initialized
   * @return bool
   */
  [[nodiscard]] virtual auto
  isInitialized() const -> bool = 0;

  /**
   * @brief Get device name
   * @return std::string
   */
  [[nodiscard]] virtual auto
  getName(bool lowercase = false) const -> std::string = 0;

  /**
   * @brief Get device info
   * @return std::string
   */
  [[nodiscard]] virtual auto
  getInfo() const -> std::string = 0;

  /**
   * @brief Get device with more info
   * @return std::string
   */
  [[nodiscard]] virtual auto
  getInfoExtended() const -> std::string = 0;

  /**
   * @brief Get device type
   * @return Device::Type
   */
  [[nodiscard]] virtual auto
  getType() const -> Device::Type = 0;

  /**
   * @brief Get device type as string
   * @return std::string
   */
  [[nodiscard]] virtual auto
  getDeviceType() const -> std::string = 0;

  /**
   * @brief Get device index in context
   */
  [[nodiscard]] virtual auto
  getDeviceIndex() const -> size_t = 0;

  /**
   * @brief Get device platform
   * @return std::string
   */
  [[nodiscard]] virtual auto
  getPlatform() const -> std::string = 0;

  /**
   * @brief check if device is compatible with cl_image
   * @return bool
   */
  [[nodiscard]] virtual auto
  supportImage() const -> bool = 0;

  /**
   * @brief Get shared context devices
   */
  [[nodiscard]] virtual auto
  getNbDevicesFromContext() const -> size_t = 0;

  /**
   * @brief Get maximum buffer size
   * @return size_t
   */
  [[nodiscard]] virtual auto
  getMaximumBufferSize() const -> size_t = 0;

  /**
   * @brief Get maximum work group size
   * @return size_t
   */
  [[nodiscard]] virtual auto
  getMaximumWorkGroupSize() const -> size_t = 0;

  /**
   * @brief Get local memory size
   * @return size_t
   */
  [[nodiscard]] virtual auto
  getLocalMemorySize() const -> size_t = 0;

  /**
   * @brief Get program from cache by key
   * @param key The key to identify the program
   * @return The program pointer if found, nullptr otherwise
   */
  [[nodiscard]] virtual auto
  getProgramFromCache(const std::string & key) const -> std::shared_ptr<void> = 0;

  /**
   * @brief Add program to cache with a given key
   * @param key The key to identify the program
   * @param program The program pointer to cache
   */
  virtual auto
  addProgramToCache(const std::string & key, std::shared_ptr<void> program) -> void = 0;

  /**
   * @brief operator << for Device::Type
   */
  friend auto
  operator<<(std::ostream & out, const Device::Type & device_type) -> std::ostream &
  {
    switch (device_type)
    {
      case Device::Type::CUDA:
        out << "CUDA";
        break;
      case Device::Type::OPENCL:
        out << "OpenCL";
        break;
    }
    return out;
  }

  /**
   * @brief operator << for Device::Pointer
   */
  friend auto
  operator<<(std::ostream & out, const Device::Pointer & device) -> std::ostream &
  {
    out << "(" << device->getType() << ") " << device->getName();
    return out;
  }
};

#if USE_OPENCL

/**
 * @brief OpenCLDevice class
 *        This class is used to manage OpenCL devices
 */
class OpenCLDevice : public Device
{
public:
  struct Context
  {
    cl_context ptr = nullptr;
    size_t     nb_device = 0;

    Context(const cl_context & ptr);
    ~Context();
    auto
    get() const -> const cl_context &;

    // Prevent accidental copy (double free)
    Context(const Context &) = delete;
    Context &
    operator=(const Context &) = delete;

    // Allow move
    Context(Context && other) noexcept;

    Context &
    operator=(Context && other) noexcept;
  };

  struct CommandQueue
  {
    cl_command_queue ptr = nullptr;

    CommandQueue(const cl_command_queue & ptr);
    ~CommandQueue();
    auto
    get() const -> const cl_command_queue &;

    // Prevent accidental copy (double free)
    CommandQueue(const CommandQueue &) = delete;

    CommandQueue &
    operator=(const CommandQueue &) = delete;

    // Allow move
    CommandQueue(CommandQueue && other) noexcept;

    CommandQueue &
    operator=(CommandQueue && other) noexcept;
  };

  struct Ressources
  {
    cl_device_id   device_ptr = nullptr;
    cl_platform_id platform_ptr = nullptr;
    cl_device_type device_type = 0;
    std::string    device_name = "";
    std::string    platform_name = "";
    std::string    platform_vendor = "";
    bool           image_support = false;
    size_t         device_index = 0;

    Ressources(const cl_platform_id & platform, const cl_device_id & device, size_t index);
    ~Ressources() = default;
    auto
    get_device() const -> const cl_device_id &;
    auto
    get_platform() const -> const cl_platform_id &;

    Ressources(const Ressources &) = delete;
    Ressources &
    operator=(const Ressources &) = delete;
  };


  /**
   * @brief Construct a new OpenCLDevice object
   * @param platform
   * @param device
   * @return OpenCLDevice
   */
  OpenCLDevice(const cl_platform_id & platform, const cl_device_id & device);

  /**
   * @brief Construct a new Device object
   * @param ressources
   * @param context
   * @param command_queue
   * @param device_index
   * @param nb_device
   * @return OpenCLDevice
   */
  OpenCLDevice(const std::shared_ptr<Ressources> &   ressources,
               const std::shared_ptr<Context> &      context,
               const std::shared_ptr<CommandQueue> & command_queue);

  /**
   * @brief Destroy the OpenCLDevice object
   */
  ~OpenCLDevice() override;

  /**
   * @brief Initialize a device by alocating the platform, device, context and command queue
   */
  auto
  initialize() -> void override;

  /**
   * @brief Release the device by releasing the platform, device, context and command queue pointers
   */
  auto
  finalize() -> void override;

  /**
   * @brief Wait for command queue to finish
   */
  auto
  finish() const -> void override;

  /**
   * @brief Set flag for waiting for command queue to finish
   */
  auto
  setWaitToFinish(bool flag) -> void override;

  /**
   * @brief Get device platform
   * @return std::string
   */
  [[nodiscard]] auto
  getPlatform() const -> std::string override;

  /**
   * @brief Get device type
   * @return Device::Type
   */
  [[nodiscard]] auto
  getType() const -> Device::Type override;

  /**
   * @brief Get device type as string
   * @return std::string
   */
  [[nodiscard]] auto
  getDeviceType() const -> std::string override;

  /**
   * @brief Get device index in context
   */
  [[nodiscard]] auto
  getDeviceIndex() const -> size_t override;

  /**
   * @brief Check if device is initialized
   * @return bool
   */
  [[nodiscard]] auto
  isInitialized() const -> bool override;

  /**
   * @brief Get OpenCL platform
   * @return const cl_platform_id &
   */
  [[nodiscard]] auto
  getCLPlatform() const -> const cl_platform_id &;

  /**
   * @brief Get OpenCL device
   * @return const cl_device_id &
   */
  [[nodiscard]] auto
  getCLDevice() const -> const cl_device_id &;

  /**
   * @brief Get OpenCL context
   * @return const cl_context &
   */
  [[nodiscard]] auto
  getCLContext() const -> const cl_context &;

  /**
   * @brief Get OpenCL command queue
   * @return const cl_command_queue &
   */
  [[nodiscard]] auto
  getCLCommandQueue() const -> const cl_command_queue &;

  /**
   * @brief Get device name
   * @param lowercase return the name in lowercase ( = False )
   * @return std::string
   */
  [[nodiscard]] auto
  getName(bool lowercase = false) const -> std::string override;

  /**
   * @brief Get device info
   * @return std::string
   */
  [[nodiscard]] auto
  getInfo() const -> std::string override;

  /**
   * @brief Get device with more info
   * @return std::string
   */
  [[nodiscard]] auto
  getInfoExtended() const -> std::string override;

  /**
   * @brief Get shared context devices
   */
  [[nodiscard]] auto
  getNbDevicesFromContext() const -> size_t override;

  /**
   * @brief check if device is compatible with cl_image
   * @return bool
   */
  [[nodiscard]] auto
  supportImage() const -> bool override;

  /**
   * @brief Get maximum buffer size
   * @return size_t
   */
  [[nodiscard]] auto
  getMaximumBufferSize() const -> size_t override;

  /**
   * @brief Get local memory size
   * @return size_t
   */
  [[nodiscard]] auto
  getLocalMemorySize() const -> size_t override;

  /**
   * @brief Get maximum work group size
   * @return size_t
   */
  [[nodiscard]] virtual auto
  getMaximumWorkGroupSize() const -> size_t override;

  /**
   * @brief Get program from cache by key
   * @param key The key to identify the program
   * @return The program pointer if found, nullptr otherwise
   */
  [[nodiscard]] auto
  getProgramFromCache(const std::string & key) const -> std::shared_ptr<void> override;

  /**
   * @brief Add program to cache with a given key
   * @param key The key to identify the program
   * @param program The program pointer to cache
   */
  auto
  addProgramToCache(const std::string & key, std::shared_ptr<void> program) -> void override;

private:
  std::shared_ptr<Ressources>   clRessources = nullptr;
  std::shared_ptr<Context>      clContext = nullptr;
  std::shared_ptr<CommandQueue> clCommandQueue = nullptr;
  std::shared_ptr<Cache>        cache = std::make_shared<Cache>();
  bool                          initialized = false;
  bool                          waitFinish = false;
};
#endif // USE_OPENCL

#if USE_CUDA

/**
 * @brief CUDADevice class
 *        This class is used to manage CUDA devices
 */
class CUDADevice : public Device
{
public:
  /**
   * @brief Construct a new CUDADevice object
   */
  explicit CUDADevice(int deviceIndex);

  /**
   * @brief Destroy the CUDADevice object
   */
  ~CUDADevice() override;

  /**
   * @brief Initialize a device by alocating the device, context and stream
   */
  auto
  initialize() -> void override;

  /**
   * @brief Release the device by releasing the device, context and stream pointers
   */
  auto
  finalize() -> void override;

  /**
   * @brief Wait for command queue to finish
   */
  auto
  finish() const -> void override;

  /**
   * @brief Set flag for waiting for command queue to finish
   */
  auto
  setWaitToFinish(bool flag) -> void override;

  /**
   * @brief Get device platform
   * @return std::string
   */
  [[nodiscard]] auto
  getPlatform() const -> std::string override;

  /**
   * @brief Get device type
   * @return Device::Type
   */
  [[nodiscard]] auto
  getType() const -> Device::Type override;

  /**
   * @brief Get device type as string
   * @return std::string
   */
  [[nodiscard]] auto
  getDeviceType() const -> std::string override;

  /**
   * @brief Check if device is initialized
   * @return bool
   */
  [[nodiscard]] auto
  isInitialized() const -> bool override;

  /**
   * @brief Get CUDA device index
   * @return int
   */
  [[nodiscard]] auto
  getCUDADeviceIndex() const -> int;

  /**
   * @brief Get CUDA device
   * @return const CUdevice &
   */
  [[nodiscard]] auto
  getCUDADevice() const -> const CUdevice &;

  /**
   * @brief Get CUDA context
   * @return const CUcontext &
   */
  [[nodiscard]] auto
  getCUDAContext() const -> const CUcontext &;

  /**
   * @brief Get CUDA stream
   * @return const CUstream &
   */
  [[nodiscard]] auto
  getCUDAStream() const -> const CUstream &;

  /**
   * @brief Get device name
   * @param lowercase return the name in lowercase ( = False )
   * @return std::string
   */
  [[nodiscard]] auto
  getName(bool lowercase = false) const -> std::string override;

  /**
   * @brief Get device info
   * @return std::string
   */
  [[nodiscard]] auto
  getInfo() const -> std::string override;

  /**
   * @brief Get device with more info
   * @return std::string
   */
  [[nodiscard]] auto
  getInfoExtended() const -> std::string override;

  /**
   * @brief Get device architecture
   * @return std::string
   */
  [[nodiscard]] auto
  getArch() const -> std::string;

  /**
   * @brief check if device is compatible with cl_image
   * @return bool
   */
  [[nodiscard]] auto
  supportImage() const -> bool override;

  /**
   * @brief Get shared context devices
   */
  [[nodiscard]] auto
  getNbDevicesFromContext() const -> size_t override;

  /**
   * @brief Get device index in context
   */
  [[nodiscard]] auto
  getDeviceIndex() const -> size_t override;

  /**
   * @brief Get maximum buffer size
   * @return size_t
   */
  [[nodiscard]] auto
  getMaximumBufferSize() const -> size_t override;

  /**
   * @brief Get local memory size
   * @return size_t
   */
  [[nodiscard]] auto
  getLocalMemorySize() const -> size_t override;

  /**
   * @brief Get maximum work group size
   * @return size_t
   */
  [[nodiscard]] virtual auto
  getMaximumWorkGroupSize() const -> size_t override;

  /**
   * @brief Get program from cache by key
   * @param key The key to identify the program
   * @return The program pointer if found, nullptr otherwise
   */
  [[nodiscard]] auto
  getProgramFromCache(const std::string & key) const -> std::shared_ptr<void> override;

  /**
   * @brief Add program to cache with a given key
   * @param key The key to identify the program
   * @param program The program pointer to cache
   */
  auto
  addProgramToCache(const std::string & key, std::shared_ptr<void> program) -> void override;

private:
  int              cudaDeviceIndex;
  CUdevice         cudaDevice;
  CUcontext        cudaContext;
  CUstream         cudaStream;
  shared_ptr<void> cache = std::make_shared<Cache>();
  bool             initialized = false;
  bool             waitFinish = false;
};
#endif // USE_CUDA

} // namespace cle

#endif // __INCLUDE_DEVICE_HPP
