#include "cleMemory.hpp"

#include "cleBackend.hpp"
#include "cleImage.hpp"
#include "cleProcessor.hpp"
#include "cleTypes.hpp"

#include <iostream>

namespace cle::Memory
{

auto
AllocateBufferMemory(const ProcessorPointer & device, const ShapeArray & shape, const DataType & type) -> Image
{
  ChannelType       channels_type = INTENSITY;
  MemAllocationType mem_alloc = DEFAULT;
  HostAccessType    host_access = HOST_READ_WRITE;
  KernelAccessType  kernel_access = KERNEL_READ_WRITE;

  size_t byte_length = shape[0] * shape[1] * shape[2] * DataTypeToSizeOf(type);
  auto   mem_ptr = Backend::GetBufferPointer(device->Context(), mem_alloc, host_access, kernel_access, byte_length);

  ObjectType object_type = static_cast<ObjectType>(Backend::GetObjectType(mem_ptr));

  return Image(device, mem_ptr, shape, type, object_type, channels_type);
}

auto
AllocateImageMemory(const ProcessorPointer & device, const ShapeArray & shape, const DataType & type) -> Image
{
  ChannelType       channels_type = INTENSITY;
  MemAllocationType mem_alloc = DEFAULT;
  HostAccessType    host_access = HOST_READ_WRITE;
  KernelAccessType  kernel_access = KERNEL_READ_WRITE;

  auto mem_ptr =
    Backend::GetImagePointer(device->Context(), mem_alloc, host_access, kernel_access, channels_type, type, shape);

  ObjectType object_type = static_cast<ObjectType>(Backend::GetObjectType(mem_ptr));

  return Image(device, mem_ptr, shape, type, object_type, channels_type);
}

auto
AllocateBufferMemory(const Image & image) -> Image
{
  return AllocateBufferMemory(image.Device(), image.Shape(), image.Data());
}

auto
AllocateImageMemory(const Image & image) -> Image
{
  return AllocateImageMemory(image.Device(), image.Shape(), image.Data());
}

auto
AllocateMemory(const ProcessorPointer & device,
               const ShapeArray &       shape,
               const DataType &         bit_type,
               const ObjectType &       object_type) -> Image
{
  if (object_type == BUFFER)
  {
    std::cout << "we allocate buffer" << std::endl;
    return AllocateBufferMemory(device, shape, bit_type);
  }
  std::cout << "we allocate image" << std::endl;
  return AllocateImageMemory(device, shape, bit_type);
}

auto
AllocateMemory(const Image & image) -> Image
{
  return AllocateMemory(image.Device(), image.Shape(), image.Data(), image.Object());
}

} // namespace cle::Memory