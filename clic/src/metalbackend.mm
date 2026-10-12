
#include "backend.hpp"
#include "backend_common.hpp"
#include "translator.hpp"
#include "cle_preamble_metal.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <string>
#include <vector>

#if USE_METAL

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

#endif

namespace cle
{

#if USE_METAL

// Note on memory management: metal-cpp getters such as commandBuffer(), computeCommandEncoder() or
// NS::String::string() return autoreleased objects. The host application (C++ or Python) has no
// autorelease pool in place, so each entry point creating such objects drains its own pool.

[[nodiscard]] static auto
castDevice(const Device::Pointer & device) -> std::shared_ptr<const MetalDevice>
{
  return detail::castDevice<MetalDevice>(device, "MetalDevice");
}

[[nodiscard]] static auto
contentsOf(const std::shared_ptr<void> & buffer_ptr) -> uint8_t *
{
  return static_cast<uint8_t *>(static_cast<MTL::Buffer *>(buffer_ptr.get())->contents());
}

[[nodiscard]] static auto
errorMessage(const NS::Error * error) -> std::string
{
  return (error && error->localizedDescription()) ? error->localizedDescription()->utf8String() : "Unknown error";
}

// ============================================================================
// Rectangular copy helpers (shapes, origins and regions with x in bytes)
// ============================================================================

/**
 * @brief Copy a rectangular region between two host-visible memory areas.
 *
 * Contiguous regions are copied in chunks of 4 KB: measured on Apple Silicon, single large memcpy calls into
 * or out of shared Metal buffers are ~35% slower than 4 KB copies when host writes and reads alternate
 * (chunks of 16 KB and more show the same slowdown; 1-4 KB chunks match the row-by-row copy).
 */
static auto
copyRect(uint8_t *                     dst,
         const std::array<size_t, 3> & dst_shape,
         const std::array<size_t, 3> & dst_origin,
         const uint8_t *               src,
         const std::array<size_t, 3> & src_shape,
         const std::array<size_t, 3> & src_origin,
         const std::array<size_t, 3> & region) -> void
{
  if (detail::isContiguous(src_shape, region) && detail::isContiguous(dst_shape, region))
  {
    constexpr size_t chunk = 4096;
    const size_t     total = detail::elementCount(region);
    dst += detail::linearOffset(dst_shape, dst_origin);
    src += detail::linearOffset(src_shape, src_origin);
    for (size_t offset = 0; offset < total; offset += chunk)
    {
      std::memcpy(dst + offset, src + offset, std::min(chunk, total - offset));
    }
    return;
  }
  for (size_t z = 0; z < region[2]; ++z)
  {
    for (size_t y = 0; y < region[1]; ++y)
    {
      std::memcpy(dst + detail::linearOffset(dst_shape, dst_origin, y, z),
                  src + detail::linearOffset(src_shape, src_origin, y, z),
                  region[0]);
    }
  }
}

// ============================================================================
// Kernel build helper
// ============================================================================

/**
 * @brief Compile Metal Shading Language source into a compute pipeline state for the given kernel.
 *        The returned object releases the pipeline state when its last reference goes away.
 */
[[nodiscard]] static auto
createPipeline(MTL::Device * dev, const std::string & metal_source, const std::string & kernel_name) -> std::shared_ptr<void>
{
  @autoreleasepool
  {
    NS::Error * error = nullptr;
    auto *      library = dev->newLibrary(NS::String::string(metal_source.c_str(), NS::UTF8StringEncoding), nullptr, &error);
    if (!library)
    {
      throw std::runtime_error("Error: Failed to compile Metal kernel library: " + errorMessage(error));
    }

    auto * function = library->newFunction(NS::String::string(kernel_name.c_str(), NS::UTF8StringEncoding));
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
      throw std::runtime_error("Error: Failed to create Metal compute pipeline state: " + errorMessage(error));
    }

    return std::shared_ptr<void>(new MTL::ComputePipelineState *(pipelineState), [](void * p) {
      auto * pp = reinterpret_cast<MTL::ComputePipelineState **>(p);
      if (*pp)
      {
        (*pp)->release();
      }
      delete pp;
    });
  }
}

// ============================================================================

MetalBackend::MetalBackend() {}

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

  // Devices are created once (each one owns a command queue and a kernel cache)
  if (device_list_.empty())
  {
    @autoreleasepool
    {
      NS::Array *  devices = MTL::CopyAllDevices();
      const size_t count = devices ? devices->count() : 0;
      if (devices)
      {
        devices->release();
      }
      device_list_.reserve(count);
      for (size_t i = 0; i < count; ++i)
      {
        try
        {
          device_list_.push_back(std::make_shared<MetalDevice>(static_cast<int>(i)));
        }
        catch (const std::exception & e)
        {
          std::cerr << "Warning: Skipping Metal device " << i << ": " << e.what() << std::endl;
        }
      }
    }
  }

  if (device_list_.empty())
  {
    std::cerr << "Warning: No Metal devices found." << std::endl;
  }
  return device_list_;
}

auto
MetalBackend::getDevice(const std::string & name, const std::string & type) const -> Device::Pointer
{
  const auto devices = getDevices(type);
  if (devices.empty())
  {
    std::cerr << "Warning: No Metal compatible devices found." << std::endl;
    return nullptr;
  }
  const auto device = detail::findDeviceByName(devices, name);
  return device ? device : devices.back();
}

auto
MetalBackend::getDeviceFromIndex(size_t index, const std::string & type) const -> Device::Pointer
{
  const auto devices = getDevices(type);
  if (devices.empty())
  {
    std::cerr << "Warning: No Metal compatible devices found." << std::endl;
    return nullptr;
  }
  return detail::deviceAt(devices, index);
}

auto
MetalBackend::getDevicesList(const std::string & type) const -> std::vector<std::string>
{
  return detail::deviceNames(getDevices(type));
}

auto
MetalBackend::allocateMemory(const Device::Pointer &       device,
                             const std::array<size_t, 3> & region,
                             const dType &                 dtype,
                             const mType &                 mtype,
                             std::shared_ptr<void> &       data_ptr) const -> void
{
  const size_t size = detail::elementCount(region) * toBytes(dtype);

  auto   metal_device = castDevice(device);
  auto * dev = static_cast<MTL::Device *>(metal_device->getMetalDevice());

  MTL::Buffer * buffer = dev->newBuffer(size, MTL::ResourceStorageModeShared);
  if (!buffer)
  {
    throw std::runtime_error("Error: Failed to allocate Metal buffer of size " + std::to_string(size));
  }

  // Command buffers retain the buffers they use, so a buffer released while still in use by the GPU
  // is only destroyed once that work has completed.
  data_ptr = std::shared_ptr<void>(buffer, [](void * p) { static_cast<MTL::Buffer *>(p)->release(); });
}

auto
MetalBackend::freeMemory(const Device::Pointer &, const mType &, const std::shared_ptr<void> &) const -> void
{
  // Memory is released by the custom deleter of the shared_ptr returned by allocateMemory.
}

auto
MetalBackend::getRefCount(void * data_ptr) const -> int
{
  return 1;
}

auto
MetalBackend::syncToStream(const Device::Pointer & device, int64_t consumer_stream) const -> void
{
  castDevice(device)->waitForCompletion();
}

static auto
writeBuffer(const Device::Pointer &       device,
            const std::shared_ptr<void> & buffer_ptr,
            std::array<size_t, 3> &       buffer_shape,
            std::array<size_t, 3> &       buffer_origin,
            std::array<size_t, 3> &       region,
            const void *                  host_ptr) -> void
{
  // the GPU may still be reading or writing the buffer
  castDevice(device)->waitForCompletion();
  copyRect(contentsOf(buffer_ptr), buffer_shape, buffer_origin, static_cast<const uint8_t *>(host_ptr), region, { 0, 0, 0 }, region);
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
  detail::scaleToBytes(buffer_shape, buffer_origin, region, toBytes(dtype));
  writeBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
}

static auto
readBuffer(const Device::Pointer &       device,
           const std::shared_ptr<void> & buffer_ptr,
           std::array<size_t, 3> &       buffer_shape,
           std::array<size_t, 3> &       buffer_origin,
           std::array<size_t, 3> &       region,
           void *                        host_ptr) -> void
{
  // the GPU may still be writing the buffer
  castDevice(device)->waitForCompletion();
  copyRect(static_cast<uint8_t *>(host_ptr), region, { 0, 0, 0 }, contentsOf(buffer_ptr), buffer_shape, buffer_origin, region);
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
  detail::scaleToBytes(buffer_shape, buffer_origin, region, toBytes(dtype));
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
  detail::scaleToBytes(src_shape, src_origin, region, bytes);
  detail::scaleToBytes(dst_shape, dst_origin, bytes);

  const auto metal_device = castDevice(device);
  const bool contiguous = detail::isContiguous(src_shape, region) && detail::isContiguous(dst_shape, region);
  if (contiguous && !metal_device->getWaitToFinish())
  {
    // Asynchronous mode: contiguous copy done by the GPU, queued after the pending work without waiting for it.
    // In synchronous mode a host copy is cheaper than submitting and awaiting a blit (unified memory).
    @autoreleasepool
    {
      auto * queue = static_cast<MTL::CommandQueue *>(metal_device->getMetalCommandQueue());
      auto * cmdBuf = queue->commandBuffer();
      auto * blit = cmdBuf ? cmdBuf->blitCommandEncoder() : nullptr;
      if (!blit)
      {
        throw std::runtime_error("Error: Failed to create Metal blit command encoder");
      }
      blit->copyFromBuffer(static_cast<MTL::Buffer *>(src_ptr.get()),
                           detail::linearOffset(src_shape, src_origin),
                           static_cast<MTL::Buffer *>(dst_ptr.get()),
                           detail::linearOffset(dst_shape, dst_origin),
                           detail::elementCount(region));
      blit->endEncoding();
      cmdBuf->commit();
      metal_device->addPendingCommandBuffer(cmdBuf);
    }
    metal_device->finish();
    return;
  }

  // Host copy, once the pending work has completed
  metal_device->waitForCompletion();
  copyRect(contentsOf(dst_ptr), dst_shape, dst_origin, contentsOf(src_ptr), src_shape, src_origin, region);
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

static auto
setBuffer(const Device::Pointer &       device,
          const std::shared_ptr<void> & buffer_ptr,
          std::array<size_t, 3> &       buffer_shape,
          std::array<size_t, 3> &       buffer_origin,
          std::array<size_t, 3> &       region,
          const dType &                 dtype,
          const float &                 value) -> void
{
  // the GPU may still be using the buffer
  castDevice(device)->waitForCompletion();

  uint8_t * base = contentsOf(buffer_ptr);
  detail::visitDType(dtype, [&](auto tag) {
    using T = decltype(tag);
    const auto cval = static_cast<T>(value);
    const auto row_ptr = [&](size_t y, size_t z) {
      return reinterpret_cast<T *>(base) + detail::linearOffset(buffer_shape, buffer_origin, y, z);
    };
    if (detail::isContiguous(buffer_shape, region))
    {
      std::fill_n(row_ptr(0, 0), detail::elementCount(region), cval);
      return;
    }
    for (size_t z = 0; z < region[2]; ++z)
    {
      for (size_t y = 0; y < region[1]; ++y)
      {
        std::fill_n(row_ptr(y, z), region[0], cval);
      }
    }
  });
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
                          std::shared_ptr<void> & kernel,
                          const std::string &     cache_key) const -> void
{
  auto   metal_device = castDevice(device);
  auto * dev = static_cast<MTL::Device *>(metal_device->getMetalDevice());

  // The pipeline state is cached in memory; the translated MSL source on disk (avoids the translation).
  auto pipeline_holder = detail::cachedBuild(
    device,
    kernel_source,
    kernel_name,
    "msl",
    cache_key,
    [&](const std::string & metal_source) { return createPipeline(dev, metal_source, kernel_name); },
    [&]() {
      // The input source is: defines + metal preamble + OpenCL-like kernel body.
      // Translate around the preamble to preserve the already-valid Metal code.
      const OpenCLToMetalTranslator translator;
      const auto                    translate = [&](const std::string & code) { return translator.translate(code); };
      auto metal_source = detail::translateAroundPreamble(kernel_source, getPreamble(), translate, translate);
      // Kernels without the CLIc preamble (native_execute) still need the Metal standard library: translated code
      // uses its unqualified names (atomic_int, memory_order_relaxed, math functions, ...)
      if (metal_source.find("<metal_stdlib>") == std::string::npos)
      {
        metal_source = "#include <metal_stdlib>\nusing namespace metal;\n" + metal_source;
      }
      return std::make_pair(createPipeline(dev, metal_source, kernel_name), metal_source);
    });

  // kernel either points to caller-provided MTL::ComputePipelineState * storage, or receives the cached holder
  if (kernel != nullptr)
  {
    *reinterpret_cast<MTL::ComputePipelineState **>(kernel.get()) = *reinterpret_cast<MTL::ComputePipelineState **>(pipeline_holder.get());
  }
  else
  {
    kernel = pipeline_holder;
  }
}

auto
MetalBackend::executeKernel(const Device::Pointer &                    device,
                            const std::string &                        kernel_source,
                            const std::string &                        kernel_name,
                            const std::array<size_t, 3> &              global_size,
                            const std::array<size_t, 3> &              local_size,
                            const std::vector<std::shared_ptr<void>> & args,
                            const std::vector<size_t> &                sizes,
                            const std::string &                        cache_key) const -> void
{
  auto   metal_device = castDevice(device);
  auto * queue = static_cast<MTL::CommandQueue *>(metal_device->getMetalCommandQueue());

  // the holder keeps the pipeline state alive while it is encoded
  std::shared_ptr<void> pipeline_holder;
  buildKernel(device, kernel_source, kernel_name, pipeline_holder, cache_key);
  auto * pipelineState = *reinterpret_cast<MTL::ComputePipelineState **>(pipeline_holder.get());

  @autoreleasepool
  {
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
        encoder->setBuffer(static_cast<MTL::Buffer *>(args[i].get()), 0, i);
      }
      else
      {
        encoder->setBytes(args[i].get(), sizes[i], i);
      }
    }

    // Threadgroup shaped after the grid, within the limit of the pipeline (which can be lower than the
    // device limit for kernels using many registers)
    const bool use_local = (local_size[0] > 0 && local_size[1] > 0 && local_size[2] > 0);
    const auto block = use_local ? local_size : detail::computeBlockSize(global_size, pipelineState->maxTotalThreadsPerThreadgroup());
    encoder->dispatchThreads(MTL::Size::Make(global_size[0], global_size[1], global_size[2]), MTL::Size::Make(block[0], block[1], block[2]));
    encoder->endEncoding();

    // Submitted without waiting: the host waits for completion only when it needs to (host access to
    // buffers, finish() or setWaitToFinish(true))
    cmdBuf->commit();
    metal_device->addPendingCommandBuffer(cmdBuf);
  }
  metal_device->finish();
}

auto
MetalBackend::getPreamble() const -> std::string
{
  return kernel::preamble_metal;
}

#endif // USE_METAL

} // namespace cle
