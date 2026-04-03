
#include "backend.hpp"
#include "translator.hpp"
#include "cle_preamble_metal.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <numeric>
#include <string>
#include <vector>

#if USE_METAL

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

#endif

namespace cle
{

#if USE_METAL

[[nodiscard]] static auto
castDevice(const Device::Pointer & device) -> std::shared_ptr<const MetalDevice>
{
  auto metal_device = std::dynamic_pointer_cast<const MetalDevice>(device);
  if (!metal_device)
  {
    throw std::runtime_error("Error: Failed to cast device to MetalDevice");
  }
  return metal_device;
}

[[nodiscard]] static auto
castDeviceMutable(const Device::Pointer & device) -> std::shared_ptr<MetalDevice>
{
  auto metal_device = std::dynamic_pointer_cast<MetalDevice>(device);
  if (!metal_device)
  {
    throw std::runtime_error("Error: Failed to cast device to MetalDevice");
  }
  return metal_device;
}

MetalBackend::MetalBackend()
{
}

auto
MetalBackend::getType() const -> Backend::Type
{
  return Backend::Type::METAL;
}

auto
MetalBackend::getDevices(const std::string & type) const -> std::vector<Device::Pointer>
{
  if (type == "cpu")
  {
    return {};
  }

  NS::Array * devices = MTL::CopyAllDevices();
  if (!devices || devices->count() == 0)
  {
    std::cerr << "Warning: No Metal devices found." << std::endl;
    if (devices)
    {
      devices->release();
    }
    return {};
  }

  std::vector<Device::Pointer> result;
  result.reserve(devices->count());
  for (NS::UInteger i = 0; i < devices->count(); ++i)
  {
    result.push_back(std::make_shared<MetalDevice>(static_cast<int>(i)));
  }
  devices->release();
  return result;
}

auto
MetalBackend::getDevice(const std::string & name, const std::string & type) const -> Device::Pointer
{
  auto       devices = getDevices(type);
  const auto lower_name = to_lower(name);

  auto it = std::find_if(devices.begin(), devices.end(), [&lower_name](const Device::Pointer & dev) {
    return dev->getName(true).find(lower_name) != std::string::npos;
  });

  if (it != devices.end())
  {
    return *it;
  }
  if (!devices.empty())
  {
    return devices.back();
  }

  std::cerr << "Warning: No Metal compatible devices found." << std::endl;
  return nullptr;
}

auto
MetalBackend::getDeviceFromIndex(size_t index, const std::string & type) const -> Device::Pointer
{
  auto devices = getDevices(type);

  if (index < devices.size())
  {
    return devices[index];
  }
  if (!devices.empty())
  {
    return devices.back();
  }

  std::cerr << "Warning: No Metal compatible devices found." << std::endl;
  return nullptr;
}

auto
MetalBackend::getDevicesList(const std::string & type) const -> std::vector<std::string>
{
  auto                     devices = getDevices(type);
  std::vector<std::string> names;
  names.reserve(devices.size());
  for (const auto & dev : devices)
  {
    names.emplace_back(dev->getName());
  }
  return names;
}

auto
MetalBackend::allocateMemory(const Device::Pointer &       device,
                             const std::array<size_t, 3> & region,
                             const dType &                 dtype,
                             const mType &                 mtype,
                             std::shared_ptr<void> &       data_ptr) const -> void
{
  const size_t size = region[0] * region[1] * region[2] * toBytes(dtype);

  auto metal_device = castDevice(device);
  auto * dev = static_cast<MTL::Device *>(metal_device->getMetalDevice());

  MTL::Buffer * buffer = dev->newBuffer(size, MTL::ResourceStorageModeShared);
  if (!buffer)
  {
    throw std::runtime_error("Error: Failed to allocate Metal buffer of size " + std::to_string(size));
  }

  data_ptr = std::shared_ptr<void>(buffer, [](void * p) {
    static_cast<MTL::Buffer *>(p)->release();
  });
}

auto
MetalBackend::freeMemory(const Device::Pointer & device, const mType & mtype, const std::shared_ptr<void> & data_ptr) const -> void
{
}

auto
MetalBackend::getRefCount(void * data_ptr) const -> int
{
  return 1;
}

auto
MetalBackend::syncToStream(const Device::Pointer & device, int64_t consumer_stream) const -> void
{
  auto metal_device = castDevice(device);
  auto * queue = static_cast<MTL::CommandQueue *>(metal_device->getMetalCommandQueue());
  auto * cmdBuf = queue->commandBuffer();
  cmdBuf->commit();
  cmdBuf->waitUntilCompleted();
}

static auto
computeLinearIndex(const std::array<size_t, 3> & origin,
                   const std::array<size_t, 3> & shape,
                   size_t                        elem_size) -> size_t
{
  return (origin[2] * shape[1] * shape[0] + origin[1] * shape[0] + origin[0]) * elem_size;
}

auto
MetalBackend::writeBuffer(const Device::Pointer &       device,
                          const std::shared_ptr<void> & buffer_ptr,
                          std::array<size_t, 3> &       buffer_shape,
                          std::array<size_t, 3> &       buffer_origin,
                          std::array<size_t, 3> &       region,
                          const void *                  host_ptr) const -> void
{
  const size_t row_bytes = region[0];
  auto *       dst_base = static_cast<uint8_t *>(static_cast<MTL::Buffer *>(buffer_ptr.get())->contents());
  const auto * src_base = static_cast<const uint8_t *>(host_ptr);

  for (size_t z = 0; z < region[2]; ++z)
  {
    for (size_t y = 0; y < region[1]; ++y)
    {
      const size_t dst_offset = (buffer_origin[2] + z) * buffer_shape[1] * buffer_shape[0]
                              + (buffer_origin[1] + y) * buffer_shape[0]
                              + buffer_origin[0];
      const size_t src_offset = z * region[1] * region[0] + y * region[0];
      std::memcpy(dst_base + dst_offset, src_base + src_offset, row_bytes);
    }
  }
}

auto
MetalBackend::writeMemory(const Device::Pointer &       device,
                          const std::shared_ptr<void> & buffer_ptr,
                          std::array<size_t, 3> &       buffer_shape,
                          std::array<size_t, 3> &       buffer_origin,
                          std::array<size_t, 3> &       region,
                          const dType &                 dtype,
                          const mType &                 mtype,
                          const void *                  host_ptr) const -> void
{
  const size_t elem_size = toBytes(dtype);
  buffer_shape[0] *= elem_size;
  buffer_origin[0] *= elem_size;
  region[0] *= elem_size;

  writeBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
}

auto
MetalBackend::readBuffer(const Device::Pointer &       device,
                         const std::shared_ptr<void> & buffer_ptr,
                         std::array<size_t, 3> &       buffer_shape,
                         std::array<size_t, 3> &       buffer_origin,
                         std::array<size_t, 3> &       region,
                         void *                        host_ptr) const -> void
{
  const size_t row_bytes = region[0];
  const auto * src_base = static_cast<const uint8_t *>(static_cast<MTL::Buffer *>(buffer_ptr.get())->contents());
  auto *       dst_base = static_cast<uint8_t *>(host_ptr);

  for (size_t z = 0; z < region[2]; ++z)
  {
    for (size_t y = 0; y < region[1]; ++y)
    {
      const size_t src_offset = (buffer_origin[2] + z) * buffer_shape[1] * buffer_shape[0]
                              + (buffer_origin[1] + y) * buffer_shape[0]
                              + buffer_origin[0];
      const size_t dst_offset = z * region[1] * region[0] + y * region[0];
      std::memcpy(dst_base + dst_offset, src_base + src_offset, row_bytes);
    }
  }
}

auto
MetalBackend::readMemory(const Device::Pointer &       device,
                         const std::shared_ptr<void> & buffer_ptr,
                         std::array<size_t, 3> &       buffer_shape,
                         std::array<size_t, 3> &       buffer_origin,
                         std::array<size_t, 3> &       region,
                         const dType &                 dtype,
                         const mType &                 mtype,
                         void *                        host_ptr) const -> void
{
  const size_t elem_size = toBytes(dtype);
  buffer_shape[0] *= elem_size;
  buffer_origin[0] *= elem_size;
  region[0] *= elem_size;

  readBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
}

auto
MetalBackend::copyMemoryBufferToBuffer(const Device::Pointer &       device,
                                       const std::shared_ptr<void> & src_ptr,
                                       std::array<size_t, 3> &       src_origin,
                                       std::array<size_t, 3> &       src_shape,
                                       const std::shared_ptr<void> & dst_ptr,
                                       std::array<size_t, 3> &       dst_origin,
                                       std::array<size_t, 3> &       dst_shape,
                                       std::array<size_t, 3> &       region,
                                       const size_t &                bytes) const -> void
{
  region[0] *= bytes;
  src_origin[0] *= bytes;
  src_shape[0] *= bytes;
  dst_origin[0] *= bytes;
  dst_shape[0] *= bytes;

  const size_t row_bytes = region[0];
  const auto * src_base = static_cast<const uint8_t *>(static_cast<MTL::Buffer *>(src_ptr.get())->contents());
  auto *       dst_base = static_cast<uint8_t *>(static_cast<MTL::Buffer *>(dst_ptr.get())->contents());

  for (size_t z = 0; z < region[2]; ++z)
  {
    for (size_t y = 0; y < region[1]; ++y)
    {
      const size_t s_off = (src_origin[2] + z) * src_shape[1] * src_shape[0]
                         + (src_origin[1] + y) * src_shape[0]
                         + src_origin[0];
      const size_t d_off = (dst_origin[2] + z) * dst_shape[1] * dst_shape[0]
                         + (dst_origin[1] + y) * dst_shape[0]
                         + dst_origin[0];
      std::memcpy(dst_base + d_off, src_base + s_off, row_bytes);
    }
  }
}

auto
MetalBackend::copyMemoryImageToBuffer(const Device::Pointer &       device,
                                      const std::shared_ptr<void> & src_ptr,
                                      std::array<size_t, 3> &       src_origin,
                                      std::array<size_t, 3> &       src_shape,
                                      const std::shared_ptr<void> & dst_ptr,
                                      std::array<size_t, 3> &       dst_origin,
                                      std::array<size_t, 3> &       dst_shape,
                                      std::array<size_t, 3> &       region,
                                      const size_t &                bytes) const -> void
{
  copyMemoryBufferToBuffer(device, src_ptr, src_origin, src_shape, dst_ptr, dst_origin, dst_shape, region, bytes);
}

auto
MetalBackend::copyMemoryBufferToImage(const Device::Pointer &       device,
                                      const std::shared_ptr<void> & src_ptr,
                                      std::array<size_t, 3> &       src_origin,
                                      std::array<size_t, 3> &       src_shape,
                                      const std::shared_ptr<void> & dst_ptr,
                                      std::array<size_t, 3> &       dst_origin,
                                      std::array<size_t, 3> &       dst_shape,
                                      std::array<size_t, 3> &       region,
                                      const size_t &                bytes) const -> void
{
  copyMemoryBufferToBuffer(device, src_ptr, src_origin, src_shape, dst_ptr, dst_origin, dst_shape, region, bytes);
}

auto
MetalBackend::copyMemoryImageToImage(const Device::Pointer &       device,
                                     const std::shared_ptr<void> & src_ptr,
                                     std::array<size_t, 3> &       src_origin,
                                     std::array<size_t, 3> &       src_shape,
                                     const std::shared_ptr<void> & dst_ptr,
                                     std::array<size_t, 3> &       dst_origin,
                                     std::array<size_t, 3> &       dst_shape,
                                     std::array<size_t, 3> &       region,
                                     const size_t &                bytes) const -> void
{
  copyMemoryBufferToBuffer(device, src_ptr, src_origin, src_shape, dst_ptr, dst_origin, dst_shape, region, bytes);
}

auto
MetalBackend::setBuffer(const Device::Pointer &       device,
                        const std::shared_ptr<void> & buffer_ptr,
                        std::array<size_t, 3> &       buffer_shape,
                        std::array<size_t, 3> &       buffer_origin,
                        std::array<size_t, 3> &       region,
                        const dType &                 dtype,
                        const float &                 value) const -> void
{
  const size_t elem_size = toBytes(dtype);
  const size_t count = region[0] * region[1] * region[2];
  auto *       dst = static_cast<uint8_t *>(static_cast<MTL::Buffer *>(buffer_ptr.get())->contents());
  const size_t offset = (buffer_origin[2] * buffer_shape[1] * buffer_shape[0]
                       + buffer_origin[1] * buffer_shape[0]
                       + buffer_origin[0]) * elem_size;

  switch (dtype)
  {
    case dType::FLOAT: {
      auto * p = reinterpret_cast<float *>(dst + offset);
      auto   v = static_cast<float>(value);
      for (size_t i = 0; i < count; ++i) p[i] = v;
      break;
    }
    case dType::INT32: {
      auto * p = reinterpret_cast<int32_t *>(dst + offset);
      auto   v = static_cast<int32_t>(value);
      for (size_t i = 0; i < count; ++i) p[i] = v;
      break;
    }
    case dType::UINT32: {
      auto * p = reinterpret_cast<uint32_t *>(dst + offset);
      auto   v = static_cast<uint32_t>(value);
      for (size_t i = 0; i < count; ++i) p[i] = v;
      break;
    }
    case dType::INT16: {
      auto * p = reinterpret_cast<int16_t *>(dst + offset);
      auto   v = static_cast<int16_t>(value);
      for (size_t i = 0; i < count; ++i) p[i] = v;
      break;
    }
    case dType::UINT16: {
      auto * p = reinterpret_cast<uint16_t *>(dst + offset);
      auto   v = static_cast<uint16_t>(value);
      for (size_t i = 0; i < count; ++i) p[i] = v;
      break;
    }
    case dType::INT8: {
      auto * p = reinterpret_cast<int8_t *>(dst + offset);
      auto   v = static_cast<int8_t>(value);
      for (size_t i = 0; i < count; ++i) p[i] = v;
      break;
    }
    case dType::UINT8: {
      auto * p = reinterpret_cast<uint8_t *>(dst + offset);
      auto   v = static_cast<uint8_t>(value);
      for (size_t i = 0; i < count; ++i) p[i] = v;
      break;
    }
    default:
      throw std::runtime_error("Error: Unsupported data type for Metal setMemory");
  }
}

auto
MetalBackend::setMemory(const Device::Pointer &       device,
                        const std::shared_ptr<void> & buffer_ptr,
                        std::array<size_t, 3> &       buffer_shape,
                        std::array<size_t, 3> &       buffer_origin,
                        std::array<size_t, 3> &       region,
                        const dType &                 dtype,
                        const mType &                 mtype,
                        const float &                 value) const -> void
{
  setBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, dtype, value);
}

auto
MetalBackend::buildKernel(const Device::Pointer & device,
                          const std::string &     kernel_source,
                          const std::string &     kernel_name,
                          std::shared_ptr<void> & kernel) const -> void
{
  auto metal_device = castDevice(device);
  auto * dev = static_cast<MTL::Device *>(metal_device->getMetalDevice());

  const auto source_hash = DiskCache::hash(kernel_source);
  const auto device_hash = DiskCache::hash(metal_device->getInfo());
  const auto cache_key = device_hash + "_" + source_hash + "_" + kernel_name;

  auto cached = device->getProgramFromCache(cache_key);
  if (cached != nullptr)
  {
    *reinterpret_cast<MTL::ComputePipelineState **>(kernel.get()) =
      *reinterpret_cast<MTL::ComputePipelineState **>(cached.get());
    return;
  }

  // The input source is: defines + metal preamble + OpenCL-like kernel body.
  // Translate only the kernel body to preserve the already-valid Metal preamble.
  const std::string preamble = getPreamble();
  const auto        preamble_pos = kernel_source.find(preamble);

  std::string metal_source;
  if (preamble_pos != std::string::npos)
  {
    const std::string       before = kernel_source.substr(0, preamble_pos);
    const std::string       after = kernel_source.substr(preamble_pos + preamble.length());
    OpenCLToMetalTranslator translator;
    metal_source = translator.translate(before) + preamble + translator.translate(after);
  }
  else
  {
    OpenCLToMetalTranslator translator;
    metal_source = translator.translate(kernel_source);
  }

  std::cout << "\n[CLIc][Metal] Translated kernel source before build ('" << kernel_name << "'):\n"
            << metal_source << "\n"
            << "[CLIc][Metal] End translated kernel source\n";

  NS::Error * error = nullptr;
  auto        sourceStr = NS::String::string(metal_source.c_str(), NS::UTF8StringEncoding);
  auto *      library = dev->newLibrary(sourceStr, nullptr, &error);
  if (!library)
  {
    std::string errMsg = error ? error->localizedDescription()->utf8String() : "Unknown error";
    throw std::runtime_error("Error: Failed to compile Metal kernel library: " + errMsg);
  }

  auto   funcName = NS::String::string(kernel_name.c_str(), NS::UTF8StringEncoding);
  auto * function = library->newFunction(funcName);
  if (!function)
  {
    library->release();
    throw std::runtime_error("Error: Failed to find function '" + kernel_name + "' in Metal library");
  }

  auto * pipelineState = dev->newComputePipelineState(function, &error);
  function->release();
  library->release();

  if (!pipelineState)
  {
    std::string errMsg = error ? error->localizedDescription()->utf8String() : "Unknown error";
    throw std::runtime_error("Error: Failed to create Metal compute pipeline state: " + errMsg);
  }

  auto pipeline_ptr = std::shared_ptr<void>(
    new MTL::ComputePipelineState *(pipelineState),
    [](void * p) {
      auto * pp = reinterpret_cast<MTL::ComputePipelineState **>(p);
      if (*pp)
      {
        (*pp)->release();
      }
      delete pp;
    });

  device->addProgramToCache(cache_key, pipeline_ptr);
  *reinterpret_cast<MTL::ComputePipelineState **>(kernel.get()) = pipelineState;
}

auto
MetalBackend::executeKernel(const Device::Pointer &                    device,
                            const std::string &                        kernel_source,
                            const std::string &                        kernel_name,
                            const std::array<size_t, 3> &              global_size,
                            const std::array<size_t, 3> &              local_size,
                            const std::vector<std::shared_ptr<void>> & args,
                            const std::vector<size_t> &                sizes) const -> void
{
  std::cout << "\n[CLIc][Metal] Kernel source before execute ('" << kernel_name << "'):\n"
            << kernel_source << "\n"
            << "[CLIc][Metal] End kernel source before execute\n";

  auto metal_device = castDevice(device);
  auto * queue = static_cast<MTL::CommandQueue *>(metal_device->getMetalCommandQueue());
  auto * dev = static_cast<MTL::Device *>(metal_device->getMetalDevice());

  MTL::ComputePipelineState * pipelineState = nullptr;
  std::shared_ptr<void>       kernel_holder(reinterpret_cast<void *>(&pipelineState), [](void *) {});
  buildKernel(std::const_pointer_cast<Device>(device), kernel_source, kernel_name, kernel_holder);

  auto * cmdBuf = queue->commandBuffer();
  if (!cmdBuf)
  {
    throw std::runtime_error("Error: Failed to create Metal command buffer");
  }

  auto * encoder = cmdBuf->computeCommandEncoder();
  if (!encoder)
  {
    throw std::runtime_error("Error: Failed to create Metal compute command encoder");
  }

  encoder->setComputePipelineState(pipelineState);

  for (size_t i = 0; i < args.size(); ++i)
  {
    if (sizes[i] == GPU_MEM_PTR_SIZE)
    {
      auto * mtlBuf = static_cast<MTL::Buffer *>(args[i].get());
      encoder->setBuffer(mtlBuf, 0, i);
    }
    else
    {
      encoder->setBytes(args[i].get(), sizes[i], i);
    }
  }

  const bool use_local = (local_size[0] > 0 && local_size[1] > 0 && local_size[2] > 0);

  MTL::Size grid = MTL::Size::Make(global_size[0], global_size[1], global_size[2]);

  MTL::Size threads;
  if (use_local)
  {
    threads = MTL::Size::Make(local_size[0], local_size[1], local_size[2]);
  }
  else
  {
    NS::UInteger w = pipelineState->threadExecutionWidth();
    NS::UInteger h = pipelineState->maxTotalThreadsPerThreadgroup() / w;
    threads = MTL::Size::Make(w, h, 1);
  }

  encoder->dispatchThreads(grid, threads);
  encoder->endEncoding();
  cmdBuf->commit();
  cmdBuf->waitUntilCompleted();
}

auto
MetalBackend::getPreamble() const -> std::string
{
  return kernel::preamble_metal;
}

#endif // USE_METAL

} // namespace cle
