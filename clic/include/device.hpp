#pragma once

#include "cache.hpp"
#include "clic.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

namespace cle
{

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
    METAL,
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
  auto
  setWaitToFinish(bool flag) -> void
  {
    waitFinish = flag;
  }

  /**
   * @brief Whether the device waits for completion after each operation (see setWaitToFinish())
   */
  [[nodiscard]] auto
  getWaitToFinish() const -> bool
  {
    return waitFinish;
  }

  /**
   * @brief Check if device is initialized
   * @return bool
   */
  [[nodiscard]] auto
  isInitialized() const -> bool
  {
    return initialized;
  }

  /**
   * @brief Get device name
   * @param lowercase return the name in lowercase ( = False )
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
  [[nodiscard]] auto
  getProgramFromCache(const std::string & key) const -> std::shared_ptr<void>
  {
    return cache->get(key);
  }

  /**
   * @brief Add program to cache with a given key
   * @param key The key to identify the program
   * @param program The program pointer to cache
   */
  auto
  addProgramToCache(const std::string & key, const std::shared_ptr<void> & program) -> void
  {
    cache->put(key, program);
  }

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
      case Device::Type::METAL:
        out << "Metal";
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

  /**
   * @brief Hash identifying the device in the kernel caches, computed once from getInfo()
   *        (internal use by the backends, found by argument-dependent lookup)
   */
  friend auto
  deviceHash(const Device & device) -> const std::string &
  {
    if (device.hash_.empty())
    {
      device.hash_ = DiskCache::hash(device.getInfo());
    }
    return device.hash_;
  }

protected:
  std::shared_ptr<ProgramCache> cache = std::make_shared<ProgramCache>(); // compiled kernels of the device
  bool                          initialized = false;
  bool                          waitFinish = false;

private:
  mutable std::string hash_;
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

    // Not copyable (double free); not movable either: only held by shared_ptr
    Context(const Context &) = delete;
    Context &
    operator=(const Context &) = delete;
  };

  struct CommandQueue
  {
    cl_command_queue ptr = nullptr;

    CommandQueue(const cl_command_queue & ptr);
    ~CommandQueue();
    auto
    get() const -> const cl_command_queue &;

    // Not copyable (double free); not movable either: only held by shared_ptr
    CommandQueue(const CommandQueue &) = delete;

    CommandQueue &
    operator=(const CommandQueue &) = delete;
  };

  struct Resources
  {
    cl_device_id   device_ptr = nullptr;
    cl_platform_id platform_ptr = nullptr;
    cl_device_type device_type = 0;
    std::string    device_name = "";
    std::string    platform_name = "";
    bool           image_support = false;
    size_t         device_index = 0;

    Resources(const cl_platform_id & platform, const cl_device_id & device, size_t index);
    ~Resources() = default;
    auto
    get_device() const -> const cl_device_id &;
    auto
    get_platform() const -> const cl_platform_id &;

    Resources(const Resources &) = delete;
    Resources &
    operator=(const Resources &) = delete;
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
   * @param resources
   * @param context
   * @param command_queue
   * @return OpenCLDevice
   */
  OpenCLDevice(const std::shared_ptr<Resources> &    resources,
               const std::shared_ptr<Context> &      context,
               const std::shared_ptr<CommandQueue> & command_queue);

  /**
   * @brief Destroy the OpenCLDevice object
   */
  ~OpenCLDevice() override;

  // Device interface (documented in Device)
  auto
  initialize() -> void override;

  auto
  finalize() -> void override;

  auto
  finish() const -> void override;

  [[nodiscard]] auto
  getPlatform() const -> std::string override;

  [[nodiscard]] auto
  getType() const -> Device::Type override;

  [[nodiscard]] auto
  getDeviceType() const -> std::string override;

  [[nodiscard]] auto
  getDeviceIndex() const -> size_t override;

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

  [[nodiscard]] auto
  getName(bool lowercase = false) const -> std::string override;

  [[nodiscard]] auto
  getInfo() const -> std::string override;

  [[nodiscard]] auto
  getInfoExtended() const -> std::string override;

  [[nodiscard]] auto
  getNbDevicesFromContext() const -> size_t override;

  [[nodiscard]] auto
  supportImage() const -> bool override;

  [[nodiscard]] auto
  getMaximumBufferSize() const -> size_t override;

  [[nodiscard]] auto
  getLocalMemorySize() const -> size_t override;

  [[nodiscard]] auto
  getMaximumWorkGroupSize() const -> size_t override;

private:
  std::shared_ptr<Resources>    clResources = nullptr;
  std::shared_ptr<Context>      clContext = nullptr;
  std::shared_ptr<CommandQueue> clCommandQueue = nullptr;
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

  // Device interface (documented in Device)
  auto
  initialize() -> void override;

  auto
  finalize() -> void override;

  auto
  finish() const -> void override;

  [[nodiscard]] auto
  getPlatform() const -> std::string override;

  [[nodiscard]] auto
  getType() const -> Device::Type override;

  [[nodiscard]] auto
  getDeviceType() const -> std::string override;

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
   * @brief Get the memory pool used for stream-ordered allocations on the device stream
   * @return CUmemoryPool, nullptr if the device or driver does not support memory pools (synchronous allocations)
   */
  [[nodiscard]] auto
  getCUDAMemoryPool() const -> CUmemoryPool;

  /**
   * @brief Get the event recorded on the device stream to make another stream wait for it (Backend::syncToStream)
   * @return CUevent, nullptr if it could not be created
   */
  [[nodiscard]] auto
  getCUDASyncEvent() const -> CUevent;

  [[nodiscard]] auto
  getName(bool lowercase = false) const -> std::string override;

  [[nodiscard]] auto
  getInfo() const -> std::string override;

  [[nodiscard]] auto
  getInfoExtended() const -> std::string override;

  /**
   * @brief Get device architecture
   * @return std::string
   */
  [[nodiscard]] auto
  getArch() const -> std::string;

  [[nodiscard]] auto
  supportImage() const -> bool override;

  [[nodiscard]] auto
  getNbDevicesFromContext() const -> size_t override;

  [[nodiscard]] auto
  getDeviceIndex() const -> size_t override;

  [[nodiscard]] auto
  getMaximumBufferSize() const -> size_t override;

  [[nodiscard]] auto
  getLocalMemorySize() const -> size_t override;

  [[nodiscard]] auto
  getMaximumWorkGroupSize() const -> size_t override;

private:
  int          cudaDeviceIndex;
  CUdevice     cudaDevice;
  CUcontext    cudaContext;
  CUstream     cudaStream;
  CUmemoryPool memoryPool = nullptr; // stream-ordered allocations, nullptr if not supported
  CUevent      syncEvent = nullptr;  // reused by syncToStream (re-recording does not affect earlier waits)
};
#endif // USE_CUDA

#if USE_METAL
class MetalDevice : public Device
{
public:
  explicit MetalDevice(int deviceIndex);
  ~MetalDevice() override;

  auto
  initialize() -> void override;
  auto
  finalize() -> void override;
  auto
  finish() const -> void override;

  [[nodiscard]] auto
  getName(bool lc = false) const -> std::string override;
  [[nodiscard]] auto
  getInfo() const -> std::string override;
  [[nodiscard]] auto
  getInfoExtended() const -> std::string override;
  [[nodiscard]] auto
  getType() const -> Device::Type override;
  [[nodiscard]] auto
  getDeviceType() const -> std::string override;
  [[nodiscard]] auto
  getDeviceIndex() const -> size_t override;
  [[nodiscard]] auto
  getPlatform() const -> std::string override;
  [[nodiscard]] auto
  supportImage() const -> bool override;
  [[nodiscard]] auto
  getNbDevicesFromContext() const -> size_t override;
  [[nodiscard]] auto
  getMaximumBufferSize() const -> size_t override;
  [[nodiscard]] auto
  getMaximumWorkGroupSize() const -> size_t override;
  [[nodiscard]] auto
  getLocalMemorySize() const -> size_t override;

  // Metal-specific accessors (opaque pointers — cast in .mm files)
  [[nodiscard]] auto
  getMetalDevice() const -> void *;
  [[nodiscard]] auto
  getMetalCommandQueue() const -> void *;
  [[nodiscard]] auto
  getMetalDeviceIndex() const -> int;

  /**
   * @brief Track a command buffer committed to the queue (opaque MTL::CommandBuffer *), retained by the
   *        device until its completion has been awaited. Completed buffers are dropped along the way.
   */
  auto
  addPendingCommandBuffer(void * command_buffer) const -> void;

  /**
   * @brief Block until all the command buffers tracked by addPendingCommandBuffer() have completed,
   *        regardless of setWaitToFinish(). Must be called before any host access to buffer contents.
   */
  auto
  waitForCompletion() const -> void;

private:
  int                         metalDeviceIndex;
  void *                      metalDevice = nullptr;
  void *                      metalQueue = nullptr;
  mutable std::vector<void *> pendingCommandBuffers;
};
#endif // USE_METAL

} // namespace cle
