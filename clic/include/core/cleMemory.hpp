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
AllocateBufferMemory(const ProcessorPointer & device,
                     const ShapeArray &       shape,
                     const DataType &         type = DataType::FLOAT32) -> Image;

auto
AllocateImageMemory(const ProcessorPointer & device,
                    const ShapeArray &       shape,
                    const DataType &         type = DataType::FLOAT32) -> Image;

auto
AllocateBufferMemory(const Image & image) -> Image;

auto
AllocateImageMemory(const Image & image) -> Image;

template <class type>
auto
WriteBufferObject(const Image & image, const type * array, const size_t & array_size) -> void
{
  if (image.Ndim() == 1)
  {
    Backend::EnqueueWriteToBuffer(
      image.GetDevice()->QueuePtr(), image.Get(), true, 0, array_size * DataTypeToSizeOf(image.GetDataType()), array);
    return;
  }
  Backend::EnqueueWriteToBufferRect(
    image.GetDevice()->QueuePtr(), image.Get(), true, image.Origin(), image.Origin(), image.Shape(), array);
}

template <class type>
auto
WriteImageObject(const Image & image, const type * array) -> void
{
  Backend::EnqueueWriteToImage(image.GetDevice()->QueuePtr(), image.Get(), true, image.Origin(), image.Shape(), array);
}

template <class type>
auto
ReadBufferObject(const Image & image, const type * array, const size_t & array_size) -> void
{
  if (image.Ndim() == 1)
  {
    Backend::EnqueueReadFromBuffer(image.GetDevice()->QueuePtr(),
                                   image.Get(),
                                   true,
                                   0,
                                   array_size * DataTypeToSizeOf(image.GetDataType()),
                                   (void *)array);
    return;
  }
  Backend::EnqueueReadFromBufferRect(
    image.GetDevice()->QueuePtr(), image.Get(), true, image.Origin(), image.Origin(), image.Shape(), (void *)array);
}

template <class type>
auto
ReadImageObject(const Image & image, const type * array) -> void
{
  Backend::EnqueueReadFromImage(
    image.GetDevice()->QueuePtr(), image.Get(), true, image.Origin(), image.Shape(), (void *)array);
}

auto
AllocateMemory(const ProcessorPointer & device,
               const ShapeArray &       shape = { 1, 1, 1 },
               const DataType &         bit_type = DataType::FLOAT32,
               const MemoryType &       object_type = MemoryType::BUFFER) -> Image;

auto
AllocateMemory(const Image & image) -> Image;

template <class type>
auto
WriteObject(const Image & image, const type * array, const size_t & array_size) -> void
{
  if (!IsCompatible<type>())
  {
    throw(std::runtime_error(
      "Error writing image to device: unsupported data type used : " + DataTypeToString(TypeToDataType<type>()) + "."));
  }
  if (image.GetNumberOfElements() != array_size)
  {
    throw(
      std::runtime_error("Error writing image to device: Image and host array are not of the same size. Image size: " +
                         std::to_string(image.GetNumberOfElements()) + " Array size: " + std::to_string(array_size)));
  }
  if (image.GetDataType() != TypeToDataType<type>())
  {
    throw(std::runtime_error(
      "Error writing image to device: Image and host array are not of the same data type. Image data type: " +
      DataTypeToString(image.GetDataType()) + " Array data type: " + DataTypeToString(TypeToDataType<type>())));
  }
  if (image.GetMemoryType() == MemoryType::BUFFER)
  {
    WriteBufferObject(image, array, array_size);
  }
  else
  {
    WriteImageObject(image, array);
  }
}

template <class type>
auto
WriteObject(const Image & image, const std::vector<type> & array) -> void
{
  WriteObject(image, array.data(), array.size());
}

template <class type>
auto
ReadObject(const Image & image, const type * array, const size_t & array_size) -> void
{
  if (!IsCompatible<type>())
  {
    throw(std::runtime_error("Error reading image to device: unsupported data type used."));
  }
  if (image.GetNumberOfElements() != array_size)
  {
    throw(
      std::runtime_error("Error reading image to host: Image and host array are not of the same size. Image size: " +
                         std::to_string(image.GetNumberOfElements()) + " Array size: " + std::to_string(array_size)));
  }
  if (image.GetDataType() != TypeToDataType<type>())
  {
    throw(std::runtime_error(
      "Error reading image to host: Image and host array are not of the same data type. Image data type: " +
      DataTypeToString(image.GetDataType()) + " Array data type: " + DataTypeToString(TypeToDataType<type>())));
  }
  if (image.GetMemoryType() == MemoryType::BUFFER)
  {
    ReadBufferObject(image, array, array_size);
  }
  else
  {
    ReadImageObject(image, array);
  }
}

template <class type>
auto
ReadObject(const Image & image, const std::vector<type> & array) -> void
{
  ReadObject(image, array.data(), array.size());
}

template <class type>
auto
ReadObject(const Image & image, const type & value) -> void
{
  ReadObject(image, &value, 1);
}

} // namespace cle::Memory

#endif // __CORE_CLEMEMORY_HPP
