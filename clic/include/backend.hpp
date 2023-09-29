#ifndef __INCLUDE_BACKEND_HPP
#define __INCLUDE_BACKEND_HPP

#include "clic.hpp"
#include "device.hpp"
#include "utils.hpp"

#include <algorithm>
#include <map>
#include <memory>
#include <vector>

namespace cle
{
class Backend
{
public:
  enum class Type
  {
    CUDA,
    OPENCL
  };

  Backend() = default;
  Backend(const Backend &) = default;
  Backend(Backend &&) = default;
  virtual ~Backend() = default;
  auto
  operator=(const Backend &) -> Backend & = default;
  auto
  operator=(Backend &&) -> Backend & = default;

  [[nodiscard]] virtual auto
  getType() const -> Backend::Type = 0;
  [[nodiscard]] virtual auto
  getDevicesList(const std::string & type) const -> std::vector<std::string> = 0;
  [[nodiscard]] virtual auto
  getDevices(const std::string & type) const -> std::vector<Device::Pointer> = 0;
  [[nodiscard]] virtual auto
  getDevice(const std::string & name, const std::string & type) const -> Device::Pointer = 0;

  [[nodiscard]] virtual auto
  getPreamble() const -> std::string = 0;

  virtual auto
  allocateMemory(const Device::Pointer &       device,
                 const std::array<size_t, 3> & region,
                 const dType &                 dtype,
                 const mType &                 mtype,
                 void **                       data_ptr) const -> void = 0;

  virtual auto
  freeMemory(const Device::Pointer & device, const mType & mtype, void ** data_ptr) const -> void = 0;

  virtual auto
  writeMemory(const Device::Pointer & device,
              void **                 buffer_ptr,
              std::array<size_t, 3> & buffer_shape,
              std::array<size_t, 3> & buffer_origin,
              std::array<size_t, 3> & region,
              const dType &           dtype,
              const mType &           mtype,
              const void *            host_ptr) const -> void = 0;

  virtual auto
  readMemory(const Device::Pointer & device,
             const void **           buffer_ptr,
             std::array<size_t, 3> & buffer_shape,
             std::array<size_t, 3> & buffer_origin,
             std::array<size_t, 3> & region,
             const dType &           dtype,
             const mType &           mtype,
             void *                  host_ptr) const -> void = 0;


  virtual auto
  copyMemoryBufferToBuffer(const Device::Pointer &       device,
                           const void **                 src_data_ptr,
                           const std::array<size_t, 3> & region,
                           const std::array<size_t, 3> & origin,
                           const size_t &                bytes,
                           void **                       dst_data_ptr) const -> void = 0;
  virtual auto
  copyMemoryImageToBuffer(const Device::Pointer &       device,
                          const void **                 src_data_ptr,
                          const std::array<size_t, 3> & region,
                          const std::array<size_t, 3> & origin,
                          const size_t &                bytes,
                          void **                       dst_data_ptr) const -> void = 0;
  virtual auto
  copyMemoryBufferToImage(const Device::Pointer &       device,
                          const void **                 src_data_ptr,
                          const std::array<size_t, 3> & region,
                          const std::array<size_t, 3> & origin,
                          const size_t &                bytes,
                          void **                       dst_data_ptr) const -> void = 0;
  virtual auto
  copyMemoryImageToImage(const Device::Pointer &       device,
                         const void **                 src_data_ptr,
                         const std::array<size_t, 3> & region,
                         const std::array<size_t, 3> & origin,
                         const size_t &                bytes,
                         void **                       dst_data_ptr) const -> void = 0;

  virtual auto
  setMemory(const Device::Pointer & device,
            void **                 buffer_ptr,
            std::array<size_t, 3> & buffer_shape,
            std::array<size_t, 3> & buffer_origin,
            std::array<size_t, 3> & region,
            const dType &           dtype,
            const mType &           mtype,
            const float &           value) const -> void = 0;

  virtual auto
  buildKernel(const Device::Pointer & device,
              const std::string &     kernel_source,
              const std::string &     kernel_name,
              void *                  kernel) const -> void = 0;
  virtual auto
  loadProgramFromCache(const Device::Pointer & device, const std::string & hash, void * program) const -> void = 0;
  virtual auto
  saveProgramToCache(const Device::Pointer & device, const std::string & hash, void * program) const -> void = 0;
  virtual auto
  executeKernel(const Device::Pointer &       device,
                const std::string &           kernel_source,
                const std::string &           kernel_name,
                const std::array<size_t, 3> & global_size,
                const std::vector<void *> &   args,
                const std::vector<size_t> &   sizes) const -> void = 0;

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

  friend auto
  operator<<(std::ostream & out, const Backend & backend) -> std::ostream &
  {
    out << backend.getType() << " backend";
    return out;
  }
};

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
  copyMemoryBufferToBuffer(const Device::Pointer &       device,
                           const void **                 src_data_ptr,
                           const std::array<size_t, 3> & region,
                           const std::array<size_t, 3> & origin,
                           const size_t &                bytes,
                           void **                       dst_data_ptr) const -> void override;
  auto
  copyMemoryImageToBuffer(const Device::Pointer &       device,
                          const void **                 src_data_ptr,
                          const std::array<size_t, 3> & region,
                          const std::array<size_t, 3> & origin,
                          const size_t &                bytes,
                          void **                       dst_data_ptr) const -> void override;
  auto
  copyMemoryBufferToImage(const Device::Pointer &       device,
                          const void **                 src_data_ptr,
                          const std::array<size_t, 3> & region,
                          const std::array<size_t, 3> & origin,
                          const size_t &                bytes,
                          void **                       dst_data_ptr) const -> void override;
  auto
  copyMemoryImageToImage(const Device::Pointer &       device,
                         const void **                 src_data_ptr,
                         const std::array<size_t, 3> & region,
                         const std::array<size_t, 3> & origin,
                         const size_t &                bytes,
                         void **                       dst_data_ptr) const -> void override;

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
  loadProgramFromCache(const Device::Pointer & device, const std::string & hash, void * program) const -> void override;
  auto
  saveProgramToCache(const Device::Pointer & device, const std::string & hash, void * program) const -> void override;
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
};

class OpenCLBackend : public Backend
{
public:
  OpenCLBackend() = default;
  OpenCLBackend(const OpenCLBackend &) = default;
  OpenCLBackend(OpenCLBackend &&) = default;
  ~OpenCLBackend() override = default;
  auto
  operator=(const OpenCLBackend &) -> OpenCLBackend & = default;
  auto
  operator=(OpenCLBackend &&) -> OpenCLBackend & = default;

  [[nodiscard]] auto
  getDevices(const std::string & type) const -> std::vector<Device::Pointer> override;
  [[nodiscard]] auto
  getDevice(const std::string & name, const std::string & type) const -> Device::Pointer override;
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
  copyMemoryBufferToBuffer(const Device::Pointer &       device,
                           const void **                 src_data_ptr,
                           const std::array<size_t, 3> & region,
                           const std::array<size_t, 3> & origin,
                           const size_t &                bytes,
                           void **                       dst_data_ptr) const -> void override;

  auto
  copyMemoryImageToBuffer(const Device::Pointer &       device,
                          const void **                 src_data_ptr,
                          const std::array<size_t, 3> & region,
                          const std::array<size_t, 3> & origin,
                          const size_t &                bytes,
                          void **                       dst_data_ptr) const -> void override;

  auto
  copyMemoryBufferToImage(const Device::Pointer &       device,
                          const void **                 src_data_ptr,
                          const std::array<size_t, 3> & region,
                          const std::array<size_t, 3> & origin,
                          const size_t &                bytes,
                          void **                       dst_data_ptr) const -> void override;

  auto
  copyMemoryImageToImage(const Device::Pointer &       device,
                         const void **                 src_data_ptr,
                         const std::array<size_t, 3> & region,
                         const std::array<size_t, 3> & origin,
                         const size_t &                bytes,
                         void **                       dst_data_ptr) const -> void override;

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
  loadProgramFromCache(const Device::Pointer & device, const std::string & hash, void * program) const -> void override;
  auto
  saveProgramToCache(const Device::Pointer & device, const std::string & hash, void * program) const -> void override;
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
};

class BackendManager
{
public:
  static auto
  getInstance() -> BackendManager &;

  [[nodiscard]] static auto
  getBackendsList() -> std::vector<std::string>;

  [[nodiscard]] static auto
  cudaEnabled() -> bool;

  [[nodiscard]] static auto
  openCLEnabled() -> bool;

  auto
  setBackend(const std::string & backend = "opencl") -> void;

  [[nodiscard]] auto
  getBackend() const -> const Backend &;

  friend auto
  operator<<(std::ostream & out, const BackendManager & backend_manager) -> std::ostream &
  {
    out << backend_manager.getBackend().getType() << " backend";
    return out;
  }

  ~BackendManager() = default;
  BackendManager(const BackendManager &) = delete;
  auto
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
