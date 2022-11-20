#include "cleProcessor.hpp"
#include "cleBackend.hpp"


#include <sstream>

namespace cle
{

Processor::Processor(const std::string & name)
{
  this->SelectDevice(name);
}

auto
Processor::PlatformPtr() const -> cl::Platform
{
  return this->platform_;
}

auto
Processor::DevicePtr() const -> cl::Device
{
  return this->device_;
}

auto
Processor::ContextPtr() const -> cl::Context
{
  return this->context_;
}

auto
Processor::QueuePtr() const -> cl::CommandQueue
{
  return this->command_queue_;
}

auto
Processor::ListAvailableDevices() -> std::vector<std::string>
{
  std::vector<std::string>        list_available_device;
  const std::vector<cl::Platform> platforms_list = Backend::GetPlatformPointerList();
  for (const auto & platform_ite : platforms_list)
  {
    const std::vector<cl::Device> devices_list = Backend::GetDevicesListFromPlatform(platform_ite);
    for (const auto & device_ite : devices_list)
    {
      if (Backend::DeviceIsAvailable(device_ite))
      {
        std::string device_name = Backend::GetDeviceName(device_ite);
        list_available_device.push_back(device_name);
      }
    }
  }
  return list_available_device;
}

auto
Processor::SelectDevice(const std::string & name) -> void
{
  bool                            found_flag = false;
  const std::vector<cl::Platform> platforms_list = Backend::GetPlatformPointerList();
  for (const auto & platform_ite : platforms_list)
  {
    const std::vector<cl::Device> devices_list = Backend::GetDevicesListFromPlatform(platform_ite);
    for (const auto & device_ite : devices_list)
    {
      if (Backend::DeviceIsAvailable(device_ite))
      {
        std::string device_name = Backend::GetDeviceName(device_ite);
        if (device_name.find(name) != std::string::npos)
        {
          this->device_ = device_ite;
          this->platform_ = platform_ite;
          this->context_ = Backend::GetContextPointer(this->device_);
          this->command_queue_ = Backend::GetQueuePointer(this->device_, this->context_, true);
          return;
        }
      }
    }
  }
  std::cerr << "Error: Fail to find/allocate requested device\n";
}

auto
Processor::GetDeviceName() const -> std::string
{
  return Backend::GetDeviceName(this->DevicePtr());
}

auto
Processor::GetDeviceInfo() const -> std::string
{
  return this->GetDeviceName();
}

auto
Processor::GetProgramMemory() -> std::map<size_t, cl::Program> &
{
  return this->program_memory_;
}

auto
Processor::ImageSupport() const -> bool
{
  return Backend::ImageSupport(this->DevicePtr());
}

auto
Processor::WaitForKernelToFinish(const bool & flag) -> void
{
  this->wait_to_finish_ = flag;
}

auto
Processor::Finish() const -> void
{
  if (this->wait_to_finish_)
  {
    Backend::WaitQueueToFinish(this->QueuePtr());
  }
}

}; // namespace cle
