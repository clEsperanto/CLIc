#ifndef __CORE_CLEPROCESSOR_HPP
#define __CORE_CLEPROCESSOR_HPP

#include <map>
#include <string>
#include <vector>

#include "clic.hpp"

namespace cle
{

class Processor
{
public:
  Processor() = default;
  explicit Processor(const std::string & name, const std::string & type = "all");
  explicit Processor(const cl_device_id & device);
  ~Processor() = default;
  Processor(const Processor & obj) = default;
  Processor(Processor && obj) = default;
  auto
  operator=(const Processor & obj) -> Processor & = default;
  auto
  operator=(Processor && obj) -> Processor & = default;

  [[nodiscard]] auto
  PlatformPtr() const -> cl::Platform;
  [[nodiscard]] auto
  DevicePtr() const -> cl::Device;
  [[nodiscard]] auto
  ContextPtr() const -> cl::Context;
  [[nodiscard]] auto
  QueuePtr() const -> cl::CommandQueue;

  auto
  Finish() const -> void;
  auto
  WaitForKernelToFinish(const bool & flag = true) -> void;
  static auto
  ListAvailableDevices(const std::string & type = "all") -> std::vector<std::string>;

  auto
  SelectDevice(const int & idx, const std::string & type = "all") -> void;
  auto
  SelectDevice(const std::string & name = "default", const std::string & type = "all") -> void;
  [[nodiscard]] auto
  GetDeviceName() const -> std::string;
  [[nodiscard]] auto
  GetDeviceInfo() const -> std::string;

  [[nodiscard]] auto
  GetProgramMemory() -> std::map<size_t, cl::Program> &;

  [[nodiscard]] auto
  ImageSupport() const -> bool;
  [[nodiscard]] auto
  DoubleSupport() const -> bool;

protected:
  static auto
  GetDevices(const std::string & type) -> std::vector<cl::Device>;
  auto
  SetDevicePointers(const cl::Device & device) -> void;

private:
  cl::Platform                  platform_;
  cl::Device                    device_;
  cl::Context                   context_;
  cl::CommandQueue              command_queue_;
  bool                          wait_to_finish_ = false;
  std::map<size_t, cl::Program> program_memory_;
};

} // namespace cle

#endif // __CORE_CLEPROCESSOR_HPP
