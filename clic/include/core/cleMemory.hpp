#ifndef __CORE_CLEMEMORY_HPP
#define __CORE_CLEMEMORY_HPP

#include <iostream>

#include "cleBackend.hpp"
#include "cleImage.hpp"
#include "cleProcessor.hpp"
#include "cleTypes.hpp"
#include "cleUtils.hpp"

#include <cassert>
#include <type_traits>

namespace cle::Memory
{

using ProcessorPointer = std::shared_ptr<Processor>;
using ShapeArray = std::array<size_t, 3>;
using BitType = cl_channel_type;
// using MemType = cl_mem_object_type;

auto
AllocateBufferObject(const ProcessorPointer & device, const ShapeArray & shape, const BitType & type = CL_FLOAT)
  -> Image;

auto
AllocateImageObject(const ProcessorPointer & device, const ShapeArray & shape, const BitType & type = CL_FLOAT)
  -> Image;

auto
AllocateBufferObject(const Image & object) -> Image;

auto
AllocateImageObject(const Image & object) -> Image;

template <class type>
auto
WriteBufferObject(const Image & object, const std::vector<type> & array) -> void
{
  size_t byte_length = array.size() * sizeof(type);
  Backend::EnqueueWriteToBuffer(object.Device()->Queue(), object.Get(), true, 0, byte_length, array.data());
}

template <class type>
auto
WriteImageObject(const Image & object, const std::vector<type> & array) -> void
{
  Backend::EnqueueWriteToImage(
    object.Device()->Queue(), object.Get(), CL_TRUE, object.Origin(), object.Shape(), array.data());
}

template <class type>
auto
ReadBufferObject(const Image & object, const std::vector<type> & array) -> void
{
  if (sizeof(type) != object.BitType().Bytes())
  {
    throw(std::runtime_error("not the same type!"));
  }
  size_t byte_length = array.size() * object.BitType().Bytes();
  Backend::EnqueueReadFromBuffer(object.Device()->Queue(), object.Get(), true, 0, byte_length, (void *)(array.data()));
}

template <class type>
auto
ReadImageObject(const Image & object, const std::vector<type> & array) -> void
{
  if (sizeof(type) != object.BitType().Bytes())
  {
    throw(std::runtime_error("not the same type!"));
  }
  Backend::EnqueueReadFromImage(
    object.Device()->Queue(), object.Get(), true, object.Origin(), object.Shape(), (void *)(array.data()));
}

auto
AllocateObject(const ProcessorPointer & device,
               const ShapeArray &       shape = { 1, 1, 1 },
               const BitType &          type = CL_FLOAT,
               const MemoryType &       object = BUFFER) -> Image;

auto
AllocateObject(const Image & object) -> Image;

template <class type>
auto
WriteObject(const Image & object, const std::vector<type> & array) -> void
{
  if (object.IsBuffer())
  {
    WriteBufferObject(object, array);
  }
  else
  {
    WriteImageObject(object, array);
  }
}

template <class type = float>
auto
ReadObject(const Image & object) -> std::vector<type>
{
  std::vector<type> array(object.Shape()[0] * object.Shape()[1] * object.Shape()[2]);
  if (object.IsBuffer())
  {
    ReadBufferObject(object, array);
  }
  else
  {
    ReadImageObject(object, array);
  }
  return array;
}

} // namespace cle::Memory

#endif // __CORE_CLEMEMORY_HPP
