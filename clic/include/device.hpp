#ifndef __INCLUDE_DEVICE_HPP
#define __INCLUDE_DEVICE_HPP

#include "clic.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>

namespace cle
{
class Device
{
public:
  enum class Type
  {
    CUDA,
    OPENCL
  };

  using Pointer = std::shared_ptr<Device>;

  Device() = default;
  virtual ~Device() = default;

  virtual auto
  initialize() -> void = 0;
  virtual auto
  finalize() -> void = 0;
  virtual auto
  finish() const -> void = 0;
  virtual auto
  setWaitToFinish(bool flag) -> void = 0;

  [[nodiscard]] virtual auto
  isInitialized() const -> bool = 0;
  [[nodiscard]] virtual auto
  getName() const -> std::string = 0;
  [[nodiscard]] virtual auto
  getInfo() const -> std::string = 0;
  [[nodiscard]] virtual auto
  getType() const -> Device::Type = 0;

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

  friend auto
  operator<<(std::ostream & out, const Device & device) -> std::ostream &
  {
    out << "(" << device.getType() << ") " << device.getName();
    return out;
  }
};

#if USE_OPENCL
class OpenCLDevice : public Device
{
public:
  using CacheType = std::unordered_map<std::string, cl_program>;

  OpenCLDevice(const cl_platform_id & platform, const cl_device_id & device);
  ~OpenCLDevice() override;

  auto
  initialize() -> void override;
  auto
  finalize() -> void override;
  auto
  finish() const -> void override;
  auto
  setWaitToFinish(bool flag) -> void override;

  [[nodiscard]] auto
  getType() const -> Device::Type override;
  [[nodiscard]] auto
  isInitialized() const -> bool override;
  [[nodiscard]] auto
  getCLPlatform() const -> const cl_platform_id &;
  [[nodiscard]] auto
  getCLDevice() const -> const cl_device_id &;
  [[nodiscard]] auto
  getCLContext() const -> const cl_context &;
  [[nodiscard]] auto
  getCLCommandQueue() const -> const cl_command_queue &;
  [[nodiscard]] auto
  getName() const -> std::string override;
  [[nodiscard]] auto
  getInfo() const -> std::string override;
  [[nodiscard]] auto
  getCache() -> CacheType &;

private:
  cl_device_id     clDevice;
  cl_platform_id   clPlatform;
  cl_context       clContext;
  cl_command_queue clCommandQueue;
  CacheType        cache;
  bool             initialized = false;
  bool             waitFinish = false;
};
#endif // USE_OPENCL

#if USE_CUDA
class CUDADevice : public Device
{
public:
  using CacheType = std::unordered_map<std::string, CUmodule>;

  explicit CUDADevice(int deviceIndex);
  ~CUDADevice() override;

  auto
  initialize() -> void override;
  auto
  finalize() -> void override;
  auto
  finish() const -> void override;
  auto
  setWaitToFinish(bool flag) -> void override;

  [[nodiscard]] auto
  getType() const -> Device::Type override;
  [[nodiscard]] auto
  isInitialized() const -> bool override;
  [[nodiscard]] auto
  getCUDADeviceIndex() const -> int;
  [[nodiscard]] auto
  getCUDADevice() const -> const CUdevice &;
  [[nodiscard]] auto
  getCUDAContext() const -> const CUcontext &;
  [[nodiscard]] auto
  getCUDAStream() const -> const CUstream &;
  [[nodiscard]] auto
  getName() const -> std::string override;
  [[nodiscard]] auto
  getInfo() const -> std::string override;
  [[nodiscard]] auto
  getArch() const -> std::string;
  [[nodiscard]] auto
  getCache() -> CacheType &;

private:
  int       cudaDeviceIndex;
  CUdevice  cudaDevice;
  CUcontext cudaContext;
  CUstream  cudaStream;
  bool      initialized = false;
  bool      waitFinish = false;
  CacheType cache;
};
#endif // USE_CUDA

} // namespace cle

#endif // __INCLUDE_DEVICE_HPP
