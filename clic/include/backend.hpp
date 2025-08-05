#ifndef __INCLUDE_BACKEND_HPP
#define __INCLUDE_BACKEND_HPP

#include "cache.hpp"
#include "clic.hpp"
#include "device.hpp"
#include "utils.hpp"

#include <algorithm>
#include <array>
#include <vector>

namespace cle
{
/**
 * @brief Backend class
 *
 * This class holds low-level device operations. It is used to allocate memory, copy data, execute kernels, etc.
 * It is standardized to operate with different hardware API (CUDA, OpenCL, etc).
 */
class Backend
{
public:
  enum class Type
  {
    CUDA,
    OPENCL
  };

  /**
   * @brief Construct a new Backend object
   */
  Backend() = default;

  /**
   * @brief Construct by copy a new Backend object
   */
  Backend(const Backend &) = default;

  /**
   * @brief Construct by move a new Backend object
   */
  Backend(Backend &&) = default;

  /**
   * @brief Destroy the Backend object
   */
  virtual ~Backend() = default;

  /**
   * @brief Copy assignment operator
   */
  auto
  operator=(const Backend &) -> Backend & = default;

  /**
   * @brief Move assignment operator
   */
  auto
  operator=(Backend &&) -> Backend & = default;

  /**
   * @brief Get the type of the backend
   *
   * @return Backend::Type
   */
  [[nodiscard]] virtual auto
  getType() const -> Backend::Type = 0;

  /**
   * @brief Get the list of devices names of a specific type
   *
   * @param type
   * @return std::vector<std::string>
   */
  [[nodiscard]] virtual auto
  getDevicesList(const std::string & type) const -> std::vector<std::string> = 0;

  /**
   * @brief Get the list of devices of a specific type
   *
   * @param type
   * @return std::vector<Device::Pointer>
   */
  [[nodiscard]] virtual auto
  getDevices(const std::string & type) const -> std::vector<Device::Pointer> = 0;

  /**
   * @brief Get a device by name and type
   *
   * @param name
   * @param type
   * @return Device::Pointer
   */
  [[nodiscard]] virtual auto
  getDevice(const std::string & name, const std::string & type) const -> Device::Pointer = 0;

  /**
   * @brief Get a device by index and type
   *
   * @param index
   * @param type
   * @return Device::Pointer
   */
  [[nodiscard]] virtual auto
  getDeviceFromIndex(size_t index, const std::string & type) const -> Device::Pointer = 0;

  /**
   * @brief Get the preamble of the backend
   *        The preamble is a string that contains the necessary code to be included in
   *        the kernel source code before compiling it.
   *
   * @return std::string
   */
  [[nodiscard]] virtual auto
  getPreamble() const -> std::string = 0;

  /**
   * @brief Get number of reference count
   *
   * @return Cache::Pointer
   */
  [[nodiscard]] virtual auto
  getRefCount(void * data_ptr) const -> int = 0;

  /**
   * @brief Allocate a memory space in the device
   *
   * @param device
   * @param size
   * @param data_ptr
   */
  virtual auto
  allocateMemory(const Device::Pointer &       device,
                 const std::array<size_t, 3> & region,
                 const dType &                 dtype,
                 const mType &                 mtype,
                 void **                       data_ptr) const -> void = 0;

  /**
   * @brief Free a memory space in the device
   *
   * @param device
   * @param mtype
   * @param data_ptr
   */
  virtual auto
  freeMemory(const Device::Pointer & device, const mType & mtype, void ** data_ptr) const -> void = 0;

  /**
   * @brief Write data from host to device buffer
   *
   * @param device
   * @param buffer_ptr
   * @param buffer_shape
   * @param buffer_origin
   * @param region
   * @param host_ptr
   */
  virtual auto
  writeMemory(const Device::Pointer & device,
              void **                 buffer_ptr,
              std::array<size_t, 3> & buffer_shape,
              std::array<size_t, 3> & buffer_origin,
              std::array<size_t, 3> & region,
              const dType &           dtype,
              const mType &           mtype,
              const void *            host_ptr) const -> void = 0;

  /**
   * @brief Read data from device buffer to host
   *
   * @param device
   * @param buffer_ptr
   * @param buffer_shape
   * @param buffer_origin
   * @param region
   * @param host_ptr
   */
  virtual auto
  readMemory(const Device::Pointer & device,
             const void **           buffer_ptr,
             std::array<size_t, 3> & buffer_shape,
             std::array<size_t, 3> & buffer_origin,
             std::array<size_t, 3> & region,
             const dType &           dtype,
             const mType &           mtype,
             void *                  host_ptr) const -> void = 0;


  /**
   * @brief Copy data from device buffer to device buffer
   *
   * @param device
   * @param src_ptr
   * @param src_origin
   * @param src_shape
   * @param dst_ptr
   * @param dst_origin
   * @param dst_shape
   * @param region
   * @param bytes
   */
  virtual auto
  copyMemoryBufferToBuffer(const Device::Pointer & device,
                           const void **           src_ptr,
                           std::array<size_t, 3> & src_origin,
                           std::array<size_t, 3> & src_shape,
                           void **                 dst_ptr,
                           std::array<size_t, 3> & dst_origin,
                           std::array<size_t, 3> & dst_shape,
                           std::array<size_t, 3> & region,
                           const size_t &          bytes) const -> void = 0;

  /**
   * @brief Copy data from device image to device buffer
   *
   * @param device
   * @param src_ptr
   * @param src_origin
   * @param src_shape
   * @param dst_ptr
   * @param dst_origin
   * @param dst_shape
   * @param region
   * @param bytes
   */
  virtual auto
  copyMemoryImageToBuffer(const Device::Pointer & device,
                          const void **           src_ptr,
                          std::array<size_t, 3> & src_origin,
                          std::array<size_t, 3> & src_shape,
                          void **                 dst_ptr,
                          std::array<size_t, 3> & dst_origin,
                          std::array<size_t, 3> & dst_shape,
                          std::array<size_t, 3> & region,
                          const size_t &          bytes) const -> void = 0;

  /**
   * @brief Copy data from device buffer to device image
   *
   * @param device
   * @param src_ptr
   * @param src_origin
   * @param src_shape
   * @param dst_ptr
   * @param dst_origin
   * @param dst_shape
   * @param region
   * @param bytes
   */
  virtual auto
  copyMemoryBufferToImage(const Device::Pointer & device,
                          const void **           src_ptr,
                          std::array<size_t, 3> & src_origin,
                          std::array<size_t, 3> & src_shape,
                          void **                 dst_ptr,
                          std::array<size_t, 3> & dst_origin,
                          std::array<size_t, 3> & dst_shape,
                          std::array<size_t, 3> & region,
                          const size_t &          bytes) const -> void = 0;

  /**
   * @brief Copy data from device image to device image
   *
   * @param device
   * @param src_ptr
   * @param src_origin
   * @param src_shape
   * @param dst_ptr
   * @param dst_origin
   * @param dst_shape
   * @param region
   * @param bytes
   */
  virtual auto
  copyMemoryImageToImage(const Device::Pointer & device,
                         const void **           src_ptr,
                         std::array<size_t, 3> & src_origin,
                         std::array<size_t, 3> & src_shape,
                         void **                 dst_ptr,
                         std::array<size_t, 3> & dst_origin,
                         std::array<size_t, 3> & dst_shape,
                         std::array<size_t, 3> & region,
                         const size_t &          bytes) const -> void = 0;

  /**
   * @brief Set a memory space to a specific value
   *
   * @param device
   * @param buffer_ptr
   * @param buffer_shape
   * @param buffer_origin
   * @param region
   * @param dtype
   * @param mtype
   * @param value
   */
  virtual auto
  setMemory(const Device::Pointer & device,
            void **                 buffer_ptr,
            std::array<size_t, 3> & buffer_shape,
            std::array<size_t, 3> & buffer_origin,
            std::array<size_t, 3> & region,
            const dType &           dtype,
            const mType &           mtype,
            const float &           value) const -> void = 0;

  /**
   * @brief Build a kernel from source code
   *
   * @param device
   * @param kernel_source
   * @param kernel_name
   * @param kernel
   */
  virtual auto
  buildKernel(const Device::Pointer & device,
              const std::string &     kernel_source,
              const std::string &     kernel_name,
              void *                  kernel) const -> void = 0;

  /**
   * @brief Execute a kernel
   *
   * @param device
   * @param kernel_source
   * @param kernel_name
   * @param global_size
   * @param args
   * @param sizes
   */
  virtual auto
  executeKernel(const Device::Pointer &       device,
                const std::string &           kernel_source,
                const std::string &           kernel_name,
                const std::array<size_t, 3> & global_size,
                const std::vector<void *> &   args,
                const std::vector<size_t> &   sizes) const -> void = 0;

  /**
   * @brief Operator << to print the backend type
   */
  friend auto
  operator<<(std::ostream & out, const Backend::Type & backend_type) -> std::ostream &
  {
    switch (backend_type)
    {
      case Backend::Type::CUDA:
        out << "CUDA";
        break;
      case Backend::Type::OPENCL:
        out << "OpenCL";
        break;
    }
    return out;
  }

  /**
   * @brief Operator << to print the backend
   */
  friend auto
  operator<<(std::ostream & out, const Backend & backend) -> std::ostream &
  {
    out << backend.getType() << " backend";
    return out;
  }
};

/**
 * @brief CUDA backend class
 *       This class holds low-level device operations for CUDA devices.
 */
class CUDABackend : public Backend
{
public:
  CUDABackend();

  CUDABackend(const CUDABackend &) = default;

  CUDABackend(CUDABackend &&) = default;

  ~CUDABackend() override = default;

  auto
  operator=(const CUDABackend &) -> CUDABackend & = default;

  auto
  operator=(CUDABackend &&) -> CUDABackend & = default;

  [[nodiscard]] auto
  getDevices(const std::string & type) const -> std::vector<Device::Pointer> override;

  [[nodiscard]] auto
  getDevice(const std::string & name, const std::string & type) const -> Device::Pointer override;

  [[nodiscard]] auto
  getDeviceFromIndex(size_t index, const std::string & type) const -> Device::Pointer override;

  [[nodiscard]] auto
  getDevicesList(const std::string & type) const -> std::vector<std::string> override;

  [[nodiscard]] auto
  getType() const -> Backend::Type override;

  static auto
  allocateBuffer(const Device::Pointer & device, const size_t & size, void ** data_ptr) -> void;

  static auto
  allocateImage(const Device::Pointer &       device,
                const std::array<size_t, 3> & region,
                const dType &                 dtype,
                void **                       data_ptr) -> void;

  auto
  allocateMemory(const Device::Pointer &       device,
                 const std::array<size_t, 3> & region,
                 const dType &                 dtype,
                 const mType &                 mtype,
                 void **                       data_ptr) const -> void override;

  auto
  freeMemory(const Device::Pointer & device, const mType & mtype, void ** data_ptr) const -> void override;

  static auto
  writeBuffer(const Device::Pointer &       device,
              void **                       buffer_ptr,
              const std::array<size_t, 3> & buffer_shape,
              const std::array<size_t, 3> & buffer_origin,
              const std::array<size_t, 3> & region,
              const void *                  host_ptr) -> void;
  auto
  writeMemory(const Device::Pointer & device,
              void **                 buffer_ptr,
              std::array<size_t, 3> & buffer_shape,
              std::array<size_t, 3> & buffer_origin,
              std::array<size_t, 3> & region,
              const dType &           dtype,
              const mType &           mtype,
              const void *            host_ptr) const -> void override;

  static auto
  readBuffer(const Device::Pointer &       device,
             const void **                 buffer_ptr,
             const std::array<size_t, 3> & buffer_shape,
             const std::array<size_t, 3> & buffer_origin,
             const std::array<size_t, 3> & region,
             void *                        host_ptr) -> void;

  auto
  readMemory(const Device::Pointer & device,
             const void **           buffer_ptr,
             std::array<size_t, 3> & buffer_shape,
             std::array<size_t, 3> & buffer_origin,
             std::array<size_t, 3> & region,
             const dType &           dtype,
             const mType &           mtype,
             void *                  host_ptr) const -> void override;

  auto
  copyMemoryBufferToBuffer(const Device::Pointer & device,
                           const void **           src_ptr,
                           std::array<size_t, 3> & src_origin,
                           std::array<size_t, 3> & src_shape,
                           void **                 dst_ptr,
                           std::array<size_t, 3> & dst_origin,
                           std::array<size_t, 3> & dst_shape,
                           std::array<size_t, 3> & region,
                           const size_t &          bytes) const -> void override;

  auto
  copyMemoryImageToBuffer(const Device::Pointer & device,
                          const void **           src_ptr,
                          std::array<size_t, 3> & src_origin,
                          std::array<size_t, 3> & src_shape,
                          void **                 dst_ptr,
                          std::array<size_t, 3> & dst_origin,
                          std::array<size_t, 3> & dst_shape,
                          std::array<size_t, 3> & region,
                          const size_t &          bytes) const -> void override;

  auto
  copyMemoryBufferToImage(const Device::Pointer & device,
                          const void **           src_ptr,
                          std::array<size_t, 3> & src_origin,
                          std::array<size_t, 3> & src_shape,
                          void **                 dst_ptr,
                          std::array<size_t, 3> & dst_origin,
                          std::array<size_t, 3> & dst_shape,
                          std::array<size_t, 3> & region,
                          const size_t &          bytes) const -> void override;

  auto
  copyMemoryImageToImage(const Device::Pointer & device,
                         const void **           src_ptr,
                         std::array<size_t, 3> & src_origin,
                         std::array<size_t, 3> & src_shape,
                         void **                 dst_ptr,
                         std::array<size_t, 3> & dst_origin,
                         std::array<size_t, 3> & dst_shape,
                         std::array<size_t, 3> & region,
                         const size_t &          bytes) const -> void override;

  auto
  setMemory(const Device::Pointer & device,
            void **                 buffer_ptr,
            std::array<size_t, 3> & buffer_shape,
            std::array<size_t, 3> & buffer_origin,
            std::array<size_t, 3> & region,
            const dType &           dtype,
            const mType &           mtype,
            const float &           value) const -> void override;

  static auto
  setBuffer(const Device::Pointer &       device,
            void **                       buffer_ptr,
            const std::array<size_t, 3> & buffer_shape,
            const std::array<size_t, 3> & buffer_origin,
            const std::array<size_t, 3> & region,
            const dType &                 dtype,
            const float &                 value) -> void;

  auto
  buildKernel(const Device::Pointer & device,
              const std::string &     kernel_source,
              const std::string &     kernel_name,
              void *                  kernel) const -> void override;

  auto
  executeKernel(const Device::Pointer &       device,
                const std::string &           kernel_source,
                const std::string &           kernel_name,
                const std::array<size_t, 3> & global_size,
                const std::vector<void *> &   args,
                const std::vector<size_t> &   sizes) const -> void override;

  [[nodiscard]] auto
  getPreamble() const -> std::string override;

  [[nodiscard]] auto
  getRefCount(void * data_ptr) const -> int override;
};


/**
 * @brief OpenCL backend class
 *       This class holds low-level device operations for OpenCL devices.
 */
class OpenCLBackend : public Backend
{
private:
  std::vector<Device::Pointer> device_list_;

public:
  OpenCLBackend();
  ~OpenCLBackend() override;

  OpenCLBackend(const OpenCLBackend &) = default;
  OpenCLBackend(OpenCLBackend &&) = default;


  auto
  operator=(const OpenCLBackend &) -> OpenCLBackend & = default;

  auto
  operator=(OpenCLBackend &&) -> OpenCLBackend & = default;

  auto
  initialiseRessources() -> void;

  [[nodiscard]] auto
  getDevices(const std::string & type) const -> std::vector<Device::Pointer> override;

  [[nodiscard]] auto
  getDevice(const std::string & name, const std::string & type) const -> Device::Pointer override;

  [[nodiscard]] auto
  getDeviceFromIndex(size_t index, const std::string & type) const -> Device::Pointer override;

  [[nodiscard]] auto
  getDevicesList(const std::string & type) const -> std::vector<std::string> override;

  [[nodiscard]] auto
  getType() const -> Backend::Type override;

  static auto
  allocateBuffer(const Device::Pointer & device, const size_t & size, void ** data_ptr) -> void;
  static auto
  allocateImage(const Device::Pointer &       device,
                const std::array<size_t, 3> & region,
                const dType &                 dtype,
                void **                       data_ptr) -> void;
  auto
  allocateMemory(const Device::Pointer &       device,
                 const std::array<size_t, 3> & region,
                 const dType &                 dtype,
                 const mType &                 mtype,
                 void **                       data_ptr) const -> void override;

  auto
  freeMemory(const Device::Pointer & device, const mType & mtype, void ** data_ptr) const -> void override;

  static auto
  writeBuffer(const Device::Pointer &       device,
              void **                       buffer_ptr,
              const std::array<size_t, 3> & buffer_shape,
              const std::array<size_t, 3> & buffer_origin,
              const std::array<size_t, 3> & region,
              const void *                  host_ptr) -> void;

  static auto
  writeImage(const Device::Pointer &       device,
             void **                       buffer_ptr,
             const std::array<size_t, 3> & buffer_shape,
             const std::array<size_t, 3> & buffer_origin,
             const std::array<size_t, 3> & region,
             const void *                  host_ptr) -> void;

  auto
  writeMemory(const Device::Pointer & device,
              void **                 buffer_ptr,
              std::array<size_t, 3> & buffer_shape,
              std::array<size_t, 3> & buffer_origin,
              std::array<size_t, 3> & region,
              const dType &           dtype,
              const mType &           mtype,
              const void *            host_ptr) const -> void override;

  static auto
  readBuffer(const Device::Pointer &       device,
             const void **                 buffer_ptr,
             const std::array<size_t, 3> & buffer_shape,
             const std::array<size_t, 3> & buffer_origin,
             const std::array<size_t, 3> & region,
             void *                        host_ptr) -> void;

  static auto
  readImage(const Device::Pointer &       device,
            const void **                 buffer_ptr,
            const std::array<size_t, 3> & buffer_shape,
            const std::array<size_t, 3> & buffer_origin,
            const std::array<size_t, 3> & region,
            void *                        host_ptr) -> void;

  auto
  readMemory(const Device::Pointer & device,
             const void **           buffer_ptr,
             std::array<size_t, 3> & buffer_shape,
             std::array<size_t, 3> & buffer_origin,
             std::array<size_t, 3> & region,
             const dType &           dtype,
             const mType &           mtype,
             void *                  host_ptr) const -> void override;


  auto
  copyMemoryBufferToBuffer(const Device::Pointer & device,
                           const void **           src_ptr,
                           std::array<size_t, 3> & src_origin,
                           std::array<size_t, 3> & src_shape,
                           void **                 dst_ptr,
                           std::array<size_t, 3> & dst_origin,
                           std::array<size_t, 3> & dst_shape,
                           std::array<size_t, 3> & region,
                           const size_t &          bytes) const -> void override;

  auto
  copyMemoryImageToBuffer(const Device::Pointer & device,
                          const void **           src_ptr,
                          std::array<size_t, 3> & src_origin,
                          std::array<size_t, 3> & src_shape,
                          void **                 dst_ptr,
                          std::array<size_t, 3> & dst_origin,
                          std::array<size_t, 3> & dst_shape,
                          std::array<size_t, 3> & region,
                          const size_t &          bytes) const -> void override;

  auto
  copyMemoryBufferToImage(const Device::Pointer & device,
                          const void **           src_ptr,
                          std::array<size_t, 3> & src_origin,
                          std::array<size_t, 3> & src_shape,
                          void **                 dst_ptr,
                          std::array<size_t, 3> & dst_origin,
                          std::array<size_t, 3> & dst_shape,
                          std::array<size_t, 3> & region,
                          const size_t &          bytes) const -> void override;

  auto
  copyMemoryImageToImage(const Device::Pointer & device,
                         const void **           src_ptr,
                         std::array<size_t, 3> & src_origin,
                         std::array<size_t, 3> & src_shape,
                         void **                 dst_ptr,
                         std::array<size_t, 3> & dst_origin,
                         std::array<size_t, 3> & dst_shape,
                         std::array<size_t, 3> & region,
                         const size_t &          bytes) const -> void override;

  auto
  setMemory(const Device::Pointer & device,
            void **                 buffer_ptr,
            std::array<size_t, 3> & buffer_shape,
            std::array<size_t, 3> & buffer_origin,
            std::array<size_t, 3> & region,
            const dType &           dtype,
            const mType &           mtype,
            const float &           value) const -> void override;

  static auto
  setImage(const Device::Pointer &       device,
           void **                       buffer_ptr,
           const std::array<size_t, 3> & buffer_shape,
           const std::array<size_t, 3> & buffer_origin,
           const std::array<size_t, 3> & region,
           const dType &                 dtype,
           const float &                 value) -> void;

  static auto
  setBuffer(const Device::Pointer &       device,
            void **                       buffer_ptr,
            const std::array<size_t, 3> & buffer_shape,
            const std::array<size_t, 3> & buffer_origin,
            const std::array<size_t, 3> & region,
            const dType &                 dtype,
            const float &                 value) -> void;

  auto
  buildKernel(const Device::Pointer & device,
              const std::string &     kernel_source,
              const std::string &     kernel_name,
              void *                  kernel) const -> void override;
  auto
  executeKernel(const Device::Pointer &       device,
                const std::string &           kernel_source,
                const std::string &           kernel_name,
                const std::array<size_t, 3> & global_size,
                const std::vector<void *> &   args,
                const std::vector<size_t> &   sizes) const -> void override;
  [[nodiscard]] auto
  getPreamble() const -> std::string override;

    [[nodiscard]] auto
  getRefCount(void * data_ptr) const -> int override;
};


/**
 * @brief BackendManager class
 *       This class is a singleton that holds the current backend and provides a way to change it.
 */
class BackendManager
{
public:
  /**
   * @brief Get the singleton instance of the BackendManager
   */
  static auto
  getInstance() -> BackendManager &;

  /**
   * @brief Get the list of available backends
   */
  [[nodiscard]] static auto
  getBackendsList() -> std::vector<std::string>;

  /**
   * @brief Check if CUDA is enabled
   *
   * @return bool True if CUDA is enabled, False otherwise
   */
  [[nodiscard]] static auto
  cudaEnabled() -> bool;

  /**
   * @brief Check if OpenCL is enabled
   *
   * @return bool True if OpenCL is enabled, False otherwise
   */
  [[nodiscard]] static auto
  openCLEnabled() -> bool;

  /**
   * @brief Set the backend
   *
   * @param backend The backend to be set, default is "opencl"
   */
  auto
  setBackend(const std::string & backend = "opencl") -> void;

  /**
   * @brief Get the backend
   *
   * @return const Backend&
   */
  [[nodiscard]] auto
  getBackend() const -> const Backend &;

  /**
   * @brief Operator << to print the backend manager
   */
  friend auto
  operator<<(std::ostream & out, const BackendManager & backend_manager) -> std::ostream &
  {
    out << backend_manager.getBackend().getType() << " backend";
    return out;
  }

  /**
   * @brief Destroy the BackendManager object
   */
  ~BackendManager() = default;

  /**
   * @brief Copy constructor (delete)
   */
  BackendManager(const BackendManager &) = delete;
  auto

  /**
   * @brief Copy assignment operator (delete)
   */
  operator=(const BackendManager &) -> BackendManager & = delete;


private:
  std::shared_ptr<Backend> backend;

  BackendManager() = default;
  BackendManager(BackendManager &&) = default;
  auto
  operator=(BackendManager &&) -> BackendManager & = default;
};

} // namespace cle

#endif // __INCLUDE_BACKEND_HPP
