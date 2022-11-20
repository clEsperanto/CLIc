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
  explicit Processor(const std::string & name);
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
  ListAvailableDevices() -> std::vector<std::string>;
  auto
  SelectDevice(const std::string & name = "") -> void;
  [[nodiscard]] auto
  GetDeviceName() const -> std::string;
  [[nodiscard]] auto
  GetDeviceInfo() const -> std::string;
  [[nodiscard]] auto
  GetProgramMemory() -> std::map<size_t, cl::Program> &;
  [[nodiscard]] auto
  ImageSupport() const -> bool;

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
