#ifndef __CORE_CLEPROCESSOR_HPP
#define __CORE_CLEPROCESSOR_HPP

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
  Platform() const -> cl::Platform;
  [[nodiscard]] auto
  Device() const -> cl::Device;
  [[nodiscard]] auto
  Context() const -> cl::Context;
  [[nodiscard]] auto
  Queue() const -> cl::CommandQueue;

  auto
  Finish() -> void;
  auto
  WaitForKernelToFinish(bool flag = true) -> void;
  static auto
  ListAvailableDevices() -> std::vector<std::string>;
  auto
  SelectDevice(std::string name = "") -> void;
  [[nodiscard]] auto
  DeviceName() const -> std::string;
  [[nodiscard]] auto
  DeviceInfo() const -> std::string;
  // [[nodiscard]] auto MemoryAvailable () -> int;

private:
  cl::Platform     platform_;
  cl::Device       device_;
  cl::Context      context_;
  cl::CommandQueue command_queue_;
  bool             wait_to_finish_ = false;
};

} // namespace cle

#endif // __CORE_CLEPROCESSOR_HPP
