#ifndef __CORE_CLETYPES_HPP
#define __CORE_CLETYPES_HPP

#include "clic.hpp"
#include <algorithm>
#include <iostream>
namespace cle
{

enum MemoryType
{
  IMAGE = CL_MEM_OBJECT_IMAGE1D,
  BUFFER = CL_MEM_OBJECT_BUFFER,
  IMAGE1D = CL_MEM_OBJECT_IMAGE1D,
  IMAGE2D = CL_MEM_OBJECT_IMAGE2D,
  IMAGE3D = CL_MEM_OBJECT_IMAGE3D,
  SCALAR = 0
};

enum ChannelType
{
  INTENSITY = CL_INTENSITY,
  RGBA = CL_RGBA
};

enum DataType
{
  FLOAT32 = CL_FLOAT,
  INT8 = CL_SIGNED_INT8,
  INT16 = CL_SIGNED_INT16,
  INT32 = CL_SIGNED_INT32,
  INT64 = 0x10E1, // 4321 CL_SIGNED_INT64
  UINT8 = CL_UNSIGNED_INT8,
  UINT16 = CL_UNSIGNED_INT16,
  UINT32 = CL_UNSIGNED_INT32,
  UINT64 = 0x10E2, // 4322 CL_UNSIGNED_INT64
};

enum DeviceType
{
  ALL = CL_DEVICE_TYPE_ALL,
  GPU = CL_DEVICE_TYPE_GPU,
  CPU = CL_DEVICE_TYPE_CPU,
};

enum MemAllocationType
{
  USE_HOST = CL_MEM_USE_HOST_PTR,
  ALLOC_HOST = CL_MEM_ALLOC_HOST_PTR,
  COPY_HOST = CL_MEM_COPY_HOST_PTR,
  BEST = CL_MEM_ALLOC_HOST_PTR,
  NONE = 0
};

enum HostAccessType
{
  HOST_READ_ONLY = CL_MEM_HOST_READ_ONLY,
  HOST_WRITE_ONLY = CL_MEM_HOST_WRITE_ONLY,
  HOST_READ_WRITE = 0,
  HOST_NO_ACCESS = CL_MEM_HOST_NO_ACCESS
};

enum KernelAccessType
{
  KERNEL_READ_ONLY = CL_MEM_READ_ONLY,
  KERNEL_WRITE_ONLY = CL_MEM_WRITE_ONLY,
  KERNEL_READ_WRITE = CL_MEM_READ_WRITE
};

enum BuildStatus
{
  SUCCESS = CL_BUILD_SUCCESS,
  FAIL = CL_BUILD_ERROR
};

template <class T>
inline auto
TypeToDataType() -> DataType
{
  static_assert(std::is_fundamental_v<T>, "Template to cast can only be of native type");
  if constexpr (std::is_same_v<T, float>)
  {
    return DataType::FLOAT32;
  }
  if constexpr (std::is_same_v<T, int64_t>)
  {
    return DataType::INT64;
  }
  if constexpr (std::is_same_v<T, uint64_t>)
  {
    return DataType::UINT64;
  }
  if constexpr (std::is_same_v<T, int32_t>)
  {
    return DataType::INT32;
  }
  if constexpr (std::is_same_v<T, uint32_t>)
  {
    return DataType::UINT32;
  }
  if constexpr (std::is_same_v<T, int16_t>)
  {
    return DataType::INT16;
  }
  if constexpr (std::is_same_v<T, uint16_t>)
  {
    return DataType::UINT16;
  }
  if constexpr (std::is_same_v<T, int8_t>)
  {
    return DataType::INT8;
  }
  if constexpr (std::is_same_v<T, uint8_t>)
  {
    return DataType::UINT8;
  }
  throw std::invalid_argument("Unknown template type to cast in data type.");
}

inline auto
DataTypeToSizeOf(const DataType & type) -> size_t
{
  switch (type)
  {
    case DataType::FLOAT32:
      return sizeof(float);
    case DataType::INT8:
      return sizeof(int8_t);
    case DataType::INT16:
      return sizeof(int16_t);
    case DataType::INT32:
      return sizeof(int32_t);
    case DataType::INT64:
      return sizeof(int64_t);
    case DataType::UINT8:
      return sizeof(uint8_t);
    case DataType::UINT16:
      return sizeof(uint16_t);
    case DataType::UINT32:
      return sizeof(uint32_t);
    case DataType::UINT64:
      return sizeof(uint64_t);
    default:
      throw std::invalid_argument("Unknown data type provided to cast in bytes size.");
  }
}

inline auto
DataTypeToString(const DataType & type, const bool & use_abbreviation = false) -> std::string
{
  switch (type)
  {
    case DataType::FLOAT32:
      return use_abbreviation ? "f" : "float";
    case DataType::INT64:
      return use_abbreviation ? "l" : "long";
    case DataType::UINT64:
      return use_abbreviation ? "ul" : "ulong";
    case DataType::INT32:
      return use_abbreviation ? "i" : "int";
    case DataType::UINT32:
      return use_abbreviation ? "ui" : "uint";
    case DataType::INT16:
      return use_abbreviation ? "s" : "short";
    case DataType::UINT16:
      return use_abbreviation ? "us" : "ushort";
    case DataType::INT8:
      return use_abbreviation ? "c" : "char";
    case DataType::UINT8:
      return use_abbreviation ? "uc" : "uchar";
    default:
      throw std::invalid_argument("Unknown data type provided to cast in string.");
  }
}


inline auto
MemoryTypeToString(const MemoryType & type) -> std::string
{
  switch (type)
  {
    case MemoryType::BUFFER:
      return "Buffer";
    case MemoryType::IMAGE1D:
      return "Image1D";
    case MemoryType::IMAGE2D:
      return "Image2D";
    case MemoryType::IMAGE3D:
      return "Image3D";
    case MemoryType::SCALAR:
      return "Scalar";
    default:
      throw std::invalid_argument("Unknown memory type provided to cast in string.");
  }
}

inline auto
StringToDeviceType(const std::string & type) -> DeviceType
{
  std::string lowercase_type;
  std::transform(type.begin(), type.end(), std::back_inserter(lowercase_type), ::tolower);

  if (lowercase_type == "gpu")
  {
    return DeviceType::GPU;
  }
  if (lowercase_type == "cpu")
  {
    return DeviceType::CPU;
  }
  if (lowercase_type == "all")
  {
    return DeviceType::ALL;
  }
  throw std::invalid_argument("Unknown device type provided to cast in device type.");
}

inline auto
DeviceTypeToString(const DeviceType & type) -> std::string
{
  switch (type)
  {
    case DeviceType::GPU:
      return "gpu";
    case DeviceType::CPU:
      return "cpu";
    case DeviceType::ALL:
      return "all";
    default:
      throw std::invalid_argument("Unknown device type provided to cast in string.");
  }
}

inline auto
operator<<(std::ostream & out, DataType & value) -> std::ostream &
{
  out << DataTypeToString(value);
  return out;
}

inline auto
operator<<(std::ostream & out, MemoryType & value) -> std::ostream &
{
  out << MemoryTypeToString(value);
  return out;
}

inline auto
IsImageCompatible(const DataType & type) -> bool
{
  return (type != DataType::INT64 && type != DataType::UINT64);
}

template <class T>
inline auto
IsCompatible() -> bool
{
  return (std::is_fundamental<T>::value && !std::is_same<T, bool>::value && !std::is_same<T, double>::value);
}

} // namespace cle

#endif // __CORE_CLETYPES_HPP
