
#include "device.hpp"
#include "utils.hpp"

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

static auto
castDevice(MTL::Device * device) -> MTL::Device *
{
  if (!device)
  {
    throw std::runtime_error("Error: Metal device pointer is null");
  }
  return device;
}

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

  NS::Array * devices = MTL::CopyAllDevices();
  if (!devices || devices->count() == 0)
  {
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

auto
MetalDevice::finalize() -> void
{
  if (!isInitialized())
  {
    return;
  }

  initialized = false;

  waitFinish = true;
  finish();

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
  if (!isInitialized())
  {
    return;
  }
  if (waitFinish)
  {
    auto * queue = static_cast<MTL::CommandQueue *>(metalQueue);
    auto * cmdBuf = queue->commandBuffer();
    cmdBuf->commit();
    cmdBuf->waitUntilCompleted();
  }
}

auto
MetalDevice::setWaitToFinish(bool flag) -> void
{
  waitFinish = flag;
}

auto
MetalDevice::isInitialized() const -> bool
{
  return initialized;
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
  std::string name = dev->name()->utf8String();
  return lowercase ? to_lower(name) : name;
}

auto
MetalDevice::getInfo() const -> std::string
{
  std::ostringstream os;
  os << "Metal Device: " << getName() << " (index " << metalDeviceIndex << ")";
  return os.str();
}

auto
MetalDevice::getInfoExtended() const -> std::string
{
  auto * dev = static_cast<MTL::Device *>(metalDevice);
  std::ostringstream os;
  os << "Metal Device Extended Info\n";
  os << std::left << std::setw(30) << "\tName" << getName() << "\n";
  os << std::left << std::setw(30) << "\tDevice Index" << metalDeviceIndex << "\n";
  if (dev)
  {
    os << std::left << std::setw(30) << "\tMax Buffer Length" << dev->maxBufferLength() << "\n";
    os << std::left << std::setw(30) << "\tMax Threads Per Threadgroup" << dev->maxThreadsPerThreadgroup().width << "\n";
    os << std::left << std::setw(30) << "\tHas Unified Memory" << (dev->hasUnifiedMemory() ? "Yes" : "No") << "\n";
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
  NS::Array * devices = MTL::CopyAllDevices();
  size_t count = devices ? devices->count() : 0;
  if (devices)
  {
    devices->release();
  }
  return count;
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
MetalDevice::getProgramFromCache(const std::string & key) const -> std::shared_ptr<void>
{
  return cache->get(key);
}

auto
MetalDevice::addProgramToCache(const std::string & key, std::shared_ptr<void> program) -> void
{
  cache->put(key, std::move(program));
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
