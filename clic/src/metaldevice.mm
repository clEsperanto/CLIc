
#include "device.hpp"
#include "utils.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

#if USE_METAL

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

#endif

namespace cle
{

#if USE_METAL

MetalDevice::MetalDevice(int deviceIndex)
  : metalDeviceIndex(deviceIndex)
{
  initialize();
}

MetalDevice::~MetalDevice()
{
  if (isInitialized())
  {
    finalize();
  }
}

auto
MetalDevice::initialize() -> void
{
  if (isInitialized())
  {
    return;
  }

  @autoreleasepool
  {
    NS::Array * devices = MTL::CopyAllDevices();
    if (!devices || devices->count() == 0)
    {
      if (devices)
      {
        devices->release();
      }
      throw std::runtime_error("Error: No Metal devices found");
    }

    if (static_cast<size_t>(metalDeviceIndex) >= devices->count())
    {
      devices->release();
      throw std::runtime_error("Error: Metal device index " + std::to_string(metalDeviceIndex) + " out of range");
    }

    auto * dev = devices->object<MTL::Device>(metalDeviceIndex);
    dev->retain();
    devices->release();

    auto * queue = dev->newCommandQueue();
    if (!queue)
    {
      dev->release();
      throw std::runtime_error("Error: Failed to create Metal command queue");
    }

    metalDevice = dev;
    metalQueue = queue;
    initialized = true;
  }
}

auto
MetalDevice::finalize() -> void
{
  if (!isInitialized())
  {
    return;
  }

  // Wait for the pending work before releasing the queue
  waitForCompletion();
  initialized = false;

  if (metalQueue)
  {
    static_cast<MTL::CommandQueue *>(metalQueue)->release();
    metalQueue = nullptr;
  }
  if (metalDevice)
  {
    static_cast<MTL::Device *>(metalDevice)->release();
    metalDevice = nullptr;
  }
}

auto
MetalDevice::finish() const -> void
{
  if (isInitialized() && waitFinish)
  {
    waitForCompletion();
  }
}

auto
MetalDevice::addPendingCommandBuffer(void * command_buffer) const -> void
{
  // Drop the command buffers that already completed, so that the list stays short when the host never waits
  auto done = [](void * p) {
    auto * buffer = static_cast<MTL::CommandBuffer *>(p);
    const auto status = buffer->status();
    if (status == MTL::CommandBufferStatusCompleted || status == MTL::CommandBufferStatusError)
    {
      buffer->release();
      return true;
    }
    return false;
  };
  pendingCommandBuffers.erase(std::remove_if(pendingCommandBuffers.begin(), pendingCommandBuffers.end(), done),
                              pendingCommandBuffers.end());

  static_cast<MTL::CommandBuffer *>(command_buffer)->retain();
  pendingCommandBuffers.push_back(command_buffer);
}

auto
MetalDevice::waitForCompletion() const -> void
{
  // Errors are reported, not thrown: this is also called on finalisation
  for (void * p : pendingCommandBuffers)
  {
    auto * buffer = static_cast<MTL::CommandBuffer *>(p);
    buffer->waitUntilCompleted();
    if (buffer->status() == MTL::CommandBufferStatusError)
    {
      @autoreleasepool
      {
        const auto * error = buffer->error();
        std::cerr << "Error: Metal command buffer failed: "
                  << ((error && error->localizedDescription()) ? error->localizedDescription()->utf8String() : "unknown error")
                  << std::endl;
      }
    }
    buffer->release();
  }
  pendingCommandBuffers.clear();
}

auto
MetalDevice::getType() const -> Device::Type
{
  return Device::Type::METAL;
}

auto
MetalDevice::getDeviceType() const -> std::string
{
  return "gpu";
}

auto
MetalDevice::getPlatform() const -> std::string
{
  return "Apple";
}

auto
MetalDevice::getName(bool lowercase) const -> std::string
{
  auto * dev = static_cast<MTL::Device *>(metalDevice);
  if (!dev)
  {
    return lowercase ? "unknown metal device" : "Unknown Metal Device";
  }
  std::string name;
  @autoreleasepool
  {
    name = dev->name()->utf8String();
  }
  return lowercase ? to_lower(name) : name;
}

auto
MetalDevice::getInfo() const -> std::string
{
  auto * dev = static_cast<MTL::Device *>(metalDevice);
  std::ostringstream os;
  os << std::left << "(" << this->getType() << ") " << getName() << "\n";
  os << std::left << std::setw(30) << "\tVendor: " << getPlatform() << "\n";
  os << std::left << std::setw(30) << "\tDevice Type: " << getDeviceType() << "\n";
  if (dev)
  {
    os << std::left << std::setw(30) << "\tLocal Memory Size: " << (getLocalMemorySize() / 1024) << " KB\n";
    os << std::left << std::setw(30) << "\tMaximum Buffer Size: " << (dev->maxBufferLength() / (1024 * 1024)) << " MB\n";
    os << std::left << std::setw(30) << "\tHas Unified Memory: " << (dev->hasUnifiedMemory() ? "Yes" : "No") << "\n";
    os << std::left << std::setw(30) << "\tImage Support: " << (supportImage() ? "Yes" : "No") << '\n';
  }
  return os.str();
}

auto
MetalDevice::getInfoExtended() const -> std::string
{
  auto * dev = static_cast<MTL::Device *>(metalDevice);
  std::ostringstream os;
  os << this->getInfo();
  if (dev)
  {
    os << std::left << std::setw(30) << "\tMax Threads Per Threadgroup: " << dev->maxThreadsPerThreadgroup().width << "\n";
  }
  return os.str();
}

auto
MetalDevice::getDeviceIndex() const -> size_t
{
  return static_cast<size_t>(metalDeviceIndex);
}

auto
MetalDevice::supportImage() const -> bool
{
  return false;
}

auto
MetalDevice::getNbDevicesFromContext() const -> size_t
{
  // Metal has no multi-device context: each device has its own queue (same convention as OpenCL / CUDA)
  return 1;
}

auto
MetalDevice::getMaximumBufferSize() const -> size_t
{
  auto * dev = static_cast<MTL::Device *>(metalDevice);
  return dev ? dev->maxBufferLength() : 0;
}

auto
MetalDevice::getMaximumWorkGroupSize() const -> size_t
{
  auto * dev = static_cast<MTL::Device *>(metalDevice);
  return dev ? dev->maxThreadsPerThreadgroup().width : 0;
}

auto
MetalDevice::getLocalMemorySize() const -> size_t
{
  return 32768;
}

auto
MetalDevice::getMetalDevice() const -> void *
{
  return metalDevice;
}

auto
MetalDevice::getMetalCommandQueue() const -> void *
{
  return metalQueue;
}

auto
MetalDevice::getMetalDeviceIndex() const -> int
{
  return metalDeviceIndex;
}

#endif // USE_METAL

} // namespace cle
