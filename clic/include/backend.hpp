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
  allocateMemory(const Device::Pointer & device, const size_t & size, void ** data_ptr) const -> void = 0;
  virtual auto
  allocateMemory(const Device::Pointer & device,
                 const size_t &          width,
                 const size_t &          height,
                 const size_t &          depth,
                 const dType &           dtype,
                 void **                 data_ptr) const -> void = 0;
  virtual auto
  freeMemory(const Device::Pointer & device, const mType & mtype, void ** data_ptr) const -> void = 0;
  virtual auto
  writeMemory(const Device::Pointer & device, void ** data_ptr, const size_t & size, const void * host_ptr) const
    -> void = 0;
  virtual auto
  writeMemory(const Device::Pointer & device,
              void **                 data_ptr,
              const size_t &          width,
              const size_t &          height,
              const size_t &          depth,
              const size_t &          bytes,
              const void *            host_ptr) const -> void = 0;
  virtual auto
  readMemory(const Device::Pointer & device, const void ** data_ptr, const size_t & size, void * host_ptr) const
    -> void = 0;
  virtual auto
  readMemory(const Device::Pointer & device,
             const void **           data_ptr,
             const size_t &          width,
             const size_t &          height,
             const size_t &          depth,
             const size_t &          bytes,
             void *                  host_ptr) const -> void = 0;
  virtual auto
  copyMemoryBufferToBuffer(const Device::Pointer & device,
                           const void **           src_data_ptr,
                           const size_t &          size,
                           void **                 dst_data_ptr) const -> void = 0;
  virtual auto
  copyMemoryImageToBuffer(const Device::Pointer & device,
                          const void **           src_data_ptr,
                          const size_t &          width,
                          const size_t &          height,
                          const size_t &          depth,
                          const size_t &          bytes,
                          void **                 dst_data_ptr) const -> void = 0;
  virtual auto
  copyMemoryBufferToImage(const Device::Pointer & device,
                          const void **           src_data_ptr,
                          const size_t &          width,
                          const size_t &          height,
                          const size_t &          depth,
                          const size_t &          bytes,
                          void **                 dst_data_ptr) const -> void = 0;
  virtual auto
  copyMemoryImageToImage(const Device::Pointer & device,
                         const void **           src_data_ptr,
                         const size_t &          width,
                         const size_t &          height,
                         const size_t &          depth,
                         const size_t &          bytes,
                         void **                 dst_data_ptr) const -> void = 0;
  virtual auto
  setMemory(const Device::Pointer & device,
            void **                 data_ptr,
            const size_t &          size,
            const float &           value,
            const dType &           dtype) const -> void = 0;
  virtual auto
  setMemory(const Device::Pointer & device,
            void **                 data_ptr,
            const size_t &          width,
            const size_t &          height,
            const size_t &          depth,
            const float &           value,
            const dType &           dtype) const -> void = 0;

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

  auto
  allocateMemory(const Device::Pointer & device, const size_t & size, void ** data_ptr) const -> void override;
  auto
  allocateMemory(const Device::Pointer & device,
                 const size_t &          width,
                 const size_t &          height,
                 const size_t &          depth,
                 const dType &           dtype,
                 void **                 data_ptr) const -> void override;
  auto
  freeMemory(const Device::Pointer & device, const mType & mtype, void ** data_ptr) const -> void override;
  auto
  writeMemory(const Device::Pointer & device, void ** data_ptr, const size_t & size, const void * host_ptr) const
    -> void override;
  auto
  writeMemory(const Device::Pointer & device,
              void **                 data_ptr,
              const size_t &          width,
              const size_t &          height,
              const size_t &          depth,
              const size_t &          bytes,
              const void *            host_ptr) const -> void override;
  auto
  readMemory(const Device::Pointer & device, const void ** data_ptr, const size_t & size, void * host_ptr) const
    -> void override;
  auto
  readMemory(const Device::Pointer & device,
             const void **           data_ptr,
             const size_t &          width,
             const size_t &          height,
             const size_t &          depth,
             const size_t &          bytes,
             void *                  host_ptr) const -> void override;

  auto
  copyMemoryBufferToBuffer(const Device::Pointer & device,
                           const void **           src_data_ptr,
                           const size_t &          size,
                           void **                 dst_data_ptr) const -> void override;
  auto
  copyMemoryImageToBuffer(const Device::Pointer & device,
                          const void **           src_data_ptr,
                          const size_t &          width,
                          const size_t &          height,
                          const size_t &          depth,
                          const size_t &          bytes,
                          void **                 dst_data_ptr) const -> void override;
  auto
  copyMemoryBufferToImage(const Device::Pointer & device,
                          const void **           src_data_ptr,
                          const size_t &          width,
                          const size_t &          height,
                          const size_t &          depth,
                          const size_t &          bytes,
                          void **                 dst_data_ptr) const -> void override;
  auto
  copyMemoryImageToImage(const Device::Pointer & device,
                         const void **           src_data_ptr,
                         const size_t &          width,
                         const size_t &          height,
                         const size_t &          depth,
                         const size_t &          bytes,
                         void **                 dst_data_ptr) const -> void override;
  auto
  setMemory(const Device::Pointer & device,
            void **                 data_ptr,
            const size_t &          size,
            const float &           value,
            const dType &           dtype) const -> void override;
  auto
  setMemory(const Device::Pointer & device,
            void **                 data_ptr,
            const size_t &          width,
            const size_t &          height,
            const size_t &          depth,
            const float &           value,
            const dType &           dtype) const -> void override;
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

  auto
  allocateMemory(const Device::Pointer & device, const size_t & size, void ** data_ptr) const -> void override;
  auto
  allocateMemory(const Device::Pointer & device,
                 const size_t &          width,
                 const size_t &          height,
                 const size_t &          depth,
                 const dType &           dtype,
                 void **                 data_ptr) const -> void override;
  auto
  freeMemory(const Device::Pointer & device, const mType & mtype, void ** data_ptr) const -> void override;
  auto
  writeMemory(const Device::Pointer & device, void ** data_ptr, const size_t & size, const void * host_ptr) const
    -> void override;
  auto
  writeMemory(const Device::Pointer & device,
              void **                 data_ptr,
              const size_t &          width,
              const size_t &          height,
              const size_t &          depth,
              const size_t &          bytes,
              const void *            host_ptr) const -> void override;
  auto
  readMemory(const Device::Pointer & device, const void ** data_ptr, const size_t & size, void * host_ptr) const
    -> void override;
  auto
  readMemory(const Device::Pointer & device,
             const void **           data_ptr,
             const size_t &          width,
             const size_t &          height,
             const size_t &          depth,
             const size_t &          bytes,
             void *                  host_ptr) const -> void override;
  auto
  copyMemoryBufferToBuffer(const Device::Pointer & device,
                           const void **           src_data_ptr,
                           const size_t &          size,
                           void **                 dst_data_ptr) const -> void override;
  auto
  copyMemoryImageToBuffer(const Device::Pointer & device,
                          const void **           src_data_ptr,
                          const size_t &          width,
                          const size_t &          height,
                          const size_t &          depth,
                          const size_t &          bytes,
                          void **                 dst_data_ptr) const -> void override;
  auto
  copyMemoryBufferToImage(const Device::Pointer & device,
                          const void **           src_data_ptr,
                          const size_t &          width,
                          const size_t &          height,
                          const size_t &          depth,
                          const size_t &          bytes,
                          void **                 dst_data_ptr) const -> void override;
  auto
  copyMemoryImageToImage(const Device::Pointer & device,
                         const void **           src_data_ptr,
                         const size_t &          width,
                         const size_t &          height,
                         const size_t &          depth,
                         const size_t &          bytes,
                         void **                 dst_data_ptr) const -> void override;
  auto
  setMemory(const Device::Pointer & device,
            void **                 data_ptr,
            const size_t &          size,
            const float &           value,
            const dType &           dtype) const -> void override;
  auto
  setMemory(const Device::Pointer & device,
            void **                 data_ptr,
            const size_t &          width,
            const size_t &          height,
            const size_t &          depth,
            const float &           value,
            const dType &           dtype) const -> void override;
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
