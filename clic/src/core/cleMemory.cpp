#include "cleMemory.hpp"

#include <iostream>

#include "cleBackend.hpp"
#include "cleImage.hpp"
#include "cleProcessor.hpp"
#include "cleTypes.hpp"
#include "cleUtils.hpp"

namespace cle::Memory
{

auto
AllocateBufferObject(const ProcessorPointer & device, const ShapeArray & shape, const BitType & type) -> Image
{
  DataType         data_type = { type };
  ChannelsType     channels_type = { CL_INTENSITY };
  MemAllocType     mem_alloc = { 0 };
  HostAccessType   host_access = { 0 };
  KernelAccessType kernel_access = { CL_MEM_READ_WRITE };

  size_t byte_length = shape[0] * shape[1] * shape[2] * data_type.Bytes();
  auto   object = Backend::GetBufferPointer(device->Context(), mem_alloc, host_access, kernel_access, byte_length);

  // ObjectType object_type = { Backend::GetMemoryType(object) };

  return Image(device, object, shape, data_type, BUFFER, channels_type);
}

auto
AllocateImageObject(const ProcessorPointer & device, const ShapeArray & shape, const BitType & type) -> Image
{
  DataType         data_type = { type };
  ChannelsType     channels_type = { CL_INTENSITY };
  MemAllocType     mem_alloc = { 0 };
  HostAccessType   host_access = { 0 };
  KernelAccessType kernel_access = { CL_MEM_READ_WRITE };

  auto object =
    Backend::GetImagePointer(device->Context(), mem_alloc, host_access, kernel_access, channels_type, data_type, shape);

  // ObjectType object_type = { Backend::GetMemoryType(object) };

  return Image(device, object, shape, data_type, IMAGE, channels_type);
}

auto
AllocateBufferObject(const Image & object) -> Image
{
  return AllocateBufferObject(object.Device(), object.Shape(), object.BitType().Get());
}

auto
AllocateImageObject(const Image & object) -> Image
{
  return AllocateImageObject(object.Device(), object.Shape(), object.BitType().Get());
}

auto
AllocateObject(const ProcessorPointer & device,
               const ShapeArray &       shape,
               const BitType &          type,
               const MemoryType &       object) -> Image
{
  if (object == BUFFER)
  {
    return AllocateBufferObject(device, shape, type);
  }
  return AllocateImageObject(device, shape, type);
}

auto
AllocateObject(const Image & object) -> Image
{
  return AllocateObject(object.Device(), object.Shape(), object.BitType().Get(), object.Memory());
}

} // namespace cle::Memory