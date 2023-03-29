#include "cleProcessor.hpp"
#include "cleBackend.hpp"

#include <algorithm>
#include <optional>
#include <sstream>

namespace cle
{

Processor::Processor(const std::string & name, const std::string & type)
  : program_memory_{}
{
  this->SelectDevice(name, type);
}

Processor::Processor(const cl_device_id & device)
  : program_memory_{}
{
  this->SetDevicePointers(cl::Device(device));
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
Processor::GetDevices(const std::string & type) -> std::vector<cl::Device>
{
  DeviceType                      device_type = StringToDeviceType(type);
  std::vector<cl::Device>         list_device;
  size_t                          estimated_size = 0;
  const std::vector<cl::Platform> platforms_list = Backend::GetPlatformPointerList();
  for (const auto & platform_ite : platforms_list)
  {
    const std::vector<cl::Device> temp_list = Backend::GetDevicesListFromPlatform(platform_ite, device_type);
    estimated_size += temp_list.size();
    list_device.reserve(estimated_size);
    std::move(temp_list.begin(), temp_list.end(), std::back_inserter(list_device));
    std::inplace_merge(list_device.begin(),
                       list_device.begin() + list_device.size() - temp_list.size(),
                       list_device.end(),
                       [](const cl::Device & lhs, const cl::Device & rhs) { return lhs() < rhs(); });
  }
  return list_device;
}

auto
Processor::ListAvailableDevices(const std::string & type) -> std::vector<std::string>
{
  auto                     list_of_device = Processor::GetDevices(type);
  std::vector<std::string> list_available_device;
  list_available_device.reserve(list_of_device.size());
  for (const auto & device : list_of_device)
  {
    list_available_device.push_back(Backend::GetDeviceName(device));
  }
  return list_available_device;
}

auto
Processor::SetDevicePointers(const cl::Device & device) -> void
{
  this->device_ = device;
  this->platform_ = Backend::GetPlatformPointer(this->device_);
  this->context_ = Backend::GetContextPointer(this->device_);
  this->command_queue_ = Backend::GetQueuePointer(this->device_, this->context_, true);
}

auto
Processor::SelectDevice(const std::string & name, const std::string & type) -> void
{
  auto list_of_device = Processor::GetDevices(type);
  if (list_of_device.empty())
  {
    throw std::runtime_error("Error: Fail to find/allocate device of type :" + type);
    return;
  }
  if (name == "default")
  {
    this->SetDevicePointers(list_of_device.back());
    return;
  }
  auto ite = std::find_if(list_of_device.begin(), list_of_device.end(), [&](const cl::Device & device) {
    return Backend::GetDeviceName(device).find(name) != std::string::npos;
  });
  if (ite == list_of_device.end())
  {
    throw std::runtime_error("Error: Fail to find/allocate device with name '" + name + "' of type '" + type + "'");
  }
  this->SetDevicePointers(*ite);
}

auto
Processor::SelectDevice(const int & idx, const std::string & type) -> void
{
  auto                      list_of_device = Processor::GetDevices(type);
  std::optional<cl::Device> selected_device;
  for (const auto & device : list_of_device)
  {
    if (idx == -1 || idx == &device - &list_of_device[0])
    {
      selected_device = device;
      break;
    }
  }
  if (selected_device.has_value())
  {
    SetDevicePointers(selected_device.value());
  }
  throw std::runtime_error("Error: Fail to find/allocate device with index '" + std::to_string(idx) + "' of type '" +
                           type + "'");
}

auto
Processor::GetDeviceName() const -> std::string
{
  return Backend::GetDeviceName(this->DevicePtr());
}

auto
Processor::GetDeviceInfo() const -> std::string
{
  return Backend::GetDeviceInfo(this->DevicePtr());
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
Processor::DoubleSupport() const -> bool
{
  return Backend::DoubleSupport(this->DevicePtr());
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
