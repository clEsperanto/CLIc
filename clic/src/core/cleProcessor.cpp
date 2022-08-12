#include "cleProcessor.hpp"
#include "cleBackend.hpp"
#include "cleUtils.hpp"

#include <sstream>

namespace cle
{

Processor::Processor(const std::string & name)
{
  this->SelectDevice(name);
}

auto
Processor::Platform() const -> cl::Platform
{
  return this->platform_;
}

auto
Processor::Device() const -> cl::Device
{
  return this->device_;
}

auto
Processor::Context() const -> cl::Context
{
  return this->context_;
}

auto
Processor::Queue() const -> cl::CommandQueue
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
Processor::SelectDevice(const std::string name) -> void
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
Processor::DeviceName() const -> std::string
{
  return Backend::GetDeviceName(this->device_);
}

auto
Processor::DeviceInfo() const -> std::string
{
  return this->DeviceName();
}

// auto
// Processor::MemoryAvailable () -> int
// {
//     constexpr int factor = 1000000;
//     const char *cmd = "nvidia-smi --query-gpu=gpu_name,memory.free --format=csv,noheader,nounits";
//     // std::string cmd = "nvidia-smi --query-gpu=gpu_name,memory.free --format=csv,noheader,nounits";
//     // std::string cmd = "nvidia-smi --query-gpu=gpu_name,memory.free --format=csv,noheader,nounits";
//     std::string cmd_out;
//     try
//         {
//             cmd_out = cle::exec (cmd);
//         }
//     catch (const std::exception &e)
//         {
//             std::cerr << e.what () << '\n';
//         }
//     std::stringstream read_out (cmd_out);
//     std::vector<std::string> info;
//     while (read_out.good ())
//         {
//             info.emplace_back ("");
//             std::getline (read_out, info.back (), ',');
//         }
//     auto it = std::find (info.begin (), info.end (), this->DeviceName ());
//     if (it != info.end ())
//         {
//             size_t idx = (it - info.begin ()) + 1;
//             return std::stoi (info[idx]) * factor;
//         }
//     return 0;
// }

auto
Processor::WaitForKernelToFinish(bool flag) -> void
{
  this->wait_to_finish_ = flag;
}

auto
Processor::Finish() -> void
{
  if (this->wait_to_finish_)
  {
    Backend::WaitQueueToFinish(this->Queue());
  }
}

}; // namespace cle
