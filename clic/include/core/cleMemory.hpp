#ifndef __CORE_CLEMEMORY_HPP
#define __CORE_CLEMEMORY_HPP

#include <iostream>

#include "cleBackend.hpp"
#include "cleImage.hpp"
#include "cleProcessor.hpp"
#include "cleTypes.hpp"


#include <cassert>
#include <type_traits>

namespace cle::Memory
{

using ProcessorPointer = std::shared_ptr<Processor>;
using ShapeArray = std::array<size_t, 3>;

auto
AllocateBufferMemory(const ProcessorPointer & device, const ShapeArray & shape, const DataType & type = FLOAT) -> Image;

auto
AllocateImageMemory(const ProcessorPointer & device, const ShapeArray & shape, const DataType & type = FLOAT) -> Image;

auto
AllocateBufferMemory(const Image & image) -> Image;

auto
AllocateImageMemory(const Image & image) -> Image;

// template <class type>
// auto
// WriteBufferObject(const Image & image, const std::vector<type> & array) -> void
// {
//   size_t byte_length = array.size() * sizeof(type);
//   if (image.Ndim() == 1)
//   {
//     Backend::EnqueueWriteToBuffer(image.GetDevice()->QueuePtr(), image.Get(), true, 0, byte_length, array.data());
//     return;
//   }
//   Backend::EnqueueWriteToBufferRect(
//     image.GetDevice()->QueuePtr(), image.Get(), true, image.Origin(), image.Origin(), image.Shape(), array.data());

//   //** this may be only interesting if the buffer is very large and if both map and unmap are done
//   //** in the same scope
//   // auto ptr = Backend::EnqueueMapBuffer(image.GetDevice()->QueuePtr(), image.Get(), true, 0, byte_length);
//   // std::memcpy(ptr, array.data(), byte_length);
//   // Backend::EnqueueUnmapMemObject(image.GetDevice()->QueuePtr(), image.Get(), ptr);
//   // return;
// }

template <class type>
auto
WriteBufferObject(const Image & image, const type * array, const size_t & array_byte_size) -> void
{
  if (image.Ndim() == 1)
  {
    Backend::EnqueueWriteToBuffer(image.GetDevice()->QueuePtr(), image.Get(), true, 0, array_byte_size, array);
    return;
  }
  Backend::EnqueueWriteToBufferRect(
    image.GetDevice()->QueuePtr(), image.Get(), true, image.Origin(), image.Origin(), image.Shape(), array);

  //** this may be only interesting if the buffer is very large and if both map and unmap are done
  //** in the same scope
  // auto ptr = Backend::EnqueueMapBuffer(image.GetDevice()->QueuePtr(), image.Get(), true, 0, byte_length);
  // std::memcpy(ptr, array.data(), byte_length);
  // Backend::EnqueueUnmapMemObject(image.GetDevice()->QueuePtr(), image.Get(), ptr);
  // return;
}

// template <class type>
// auto
// WriteImageObject(const Image & image, const std::vector<type> & array) -> void
// {
//   Backend::EnqueueWriteToImage(
//     image.GetDevice()->QueuePtr(), image.Get(), CL_TRUE, image.Origin(), image.Shape(), array.data());
// }

template <class type>
auto
WriteImageObject(const Image & image, const type * array) -> void
{
  Backend::EnqueueWriteToImage(
    image.GetDevice()->QueuePtr(), image.Get(), CL_TRUE, image.Origin(), image.Shape(), array);
}

template <class type>
auto
ReadBufferObject(const Image & image, const std::vector<type> & array) -> void
{
  if (sizeof(type) != image.GetSizeOfElements())
  {
    throw(std::runtime_error("Error: Buffer and host array are not of the same data type."));
  }
  size_t byte_length = array.size() * DataTypeToSizeOf(image.GetDataType());
  if (image.Ndim() == 1)
  {
    Backend::EnqueueReadFromBuffer(
      image.GetDevice()->QueuePtr(), image.Get(), true, 0, byte_length, (void *)(array.data()));
    return;
  }
  Backend::EnqueueReadFromBufferRect(image.GetDevice()->QueuePtr(),
                                     image.Get(),
                                     true,
                                     image.Origin(),
                                     image.Origin(),
                                     image.Shape(),
                                     (void *)(array.data()));

  //** this may be only interesting if the buffer is very large and if both map and unmap are done
  //** in the same scope
  //  auto ptr = Backend::EnqueueMapBuffer(image.GetDevice()->QueuePtr(), image.Get(), true, 0, byte_length);
  //  std::memcpy((void *)(array.data()), ptr, byte_length);
  //  Backend::EnqueueUnmapMemObject(image.GetDevice()->QueuePtr(), image.Get(), ptr);
  //  return;
}

template <class type>
auto
ReadImageObject(const Image & image, const std::vector<type> & array) -> void
{
  if (sizeof(type) != image.GetSizeOfElements())
  {
    throw(std::runtime_error("Error: Image and host array are not of the same type."));
  }
  Backend::EnqueueReadFromImage(
    image.GetDevice()->QueuePtr(), image.Get(), true, image.Origin(), image.Shape(), (void *)(array.data()));
}

auto
AllocateMemory(const ProcessorPointer & device,
               const ShapeArray &       shape = { 1, 1, 1 },
               const DataType &         bit_type = FLOAT,
               const MemoryType &       object_type = BUFFER) -> Image;

auto
AllocateMemory(const Image & image) -> Image;

// template <class type>
// auto
// WriteObject(const Image & image, const std::vector<type> & array) -> void
// {
//   if (image.GetMemoryType() == BUFFER)
//   {
//     WriteBufferObject(image, array);
//   }
//   else
//   {
//     WriteImageObject(image, array);
//   }
// }

template <class type>
auto
WriteObject(const Image & image, const type * array, const size_t & array_bytes_size) -> void
{
  if (image.GetMemoryType() == BUFFER)
  {
    WriteBufferObject(image, array, array_bytes_size);
  }
  else
  {
    WriteImageObject(image, array);
  }
}

template <class type = float>
auto
ReadObject(const Image & image) -> std::vector<type>
{
  std::vector<type> array(image.Shape()[0] * image.Shape()[1] * image.Shape()[2]);
  if (image.GetMemoryType() == BUFFER)
  {
    ReadBufferObject(image, array);
  }
  else
  {
    ReadImageObject(image, array);
  }
  return array;
}

} // namespace cle::Memory

#endif // __CORE_CLEMEMORY_HPP
