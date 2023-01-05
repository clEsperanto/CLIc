#ifndef __CORE_CLETYPES_HPP
#define __CORE_CLETYPES_HPP

#include "clic.hpp"
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
  FLOAT64 = 0x10E3, // 4323 CL_DOUBLE
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
  static_assert(std::is_fundamental<T>::value, "Object can only be of native type");
  if (std::is_same<T, double>::value)
  {
    return DataType::FLOAT64;
  }
  if (std::is_same<T, float>::value)
  {
    return DataType::FLOAT32;
  }
  if (std::is_same<T, int64_t>::value)
  {
    return DataType::INT64;
  }
  if (std::is_same<T, uint64_t>::value)
  {
    return DataType::UINT64;
  }
  if (std::is_same<T, int32_t>::value)
  {
    return DataType::INT32;
  }
  if (std::is_same<T, uint32_t>::value)
  {
    return DataType::UINT32;
  }
  if (std::is_same<T, int8_t>::value)
  {
    return DataType::INT8;
  }
  if (std::is_same<T, uint8_t>::value)
  {
    return DataType::UINT8;
  }
  if (std::is_same<T, int16_t>::value)
  {
    return DataType::INT16;
  }
  if (std::is_same<T, uint16_t>::value)
  {
    return DataType::UINT16;
  }
  throw(std::runtime_error("Unknown template type to cast in data type."));
}

inline auto
DataTypeToSizeOf(const DataType & type) -> size_t
{
  size_t res;
  switch (type)
  {
    case DataType::FLOAT64:
      res = sizeof(double);
      break;
    case DataType::FLOAT32:
      res = sizeof(float);
      break;
    case DataType::INT8:
      res = sizeof(int8_t);
      break;
    case DataType::INT16:
      res = sizeof(int16_t);
      break;
    case DataType::INT32:
      res = sizeof(int32_t);
      break;
    case DataType::INT64:
      res = sizeof(int64_t);
      break;
    case DataType::UINT8:
      res = sizeof(uint8_t);
      break;
    case DataType::UINT16:
      res = sizeof(uint16_t);
      break;
    case DataType::UINT32:
      res = sizeof(uint32_t);
      break;
    case DataType::UINT64:
      res = sizeof(uint64_t);
      break;
    default:
      throw(std::runtime_error("Unknown data type provided to cast in bytes size."));
  }
  return res;
}

inline auto
DataTypeToString(const DataType & type, const bool & use_abreviation = false) -> std::string
{
  std::string res;
  switch (type)
  {
    case DataType::FLOAT64:
      res = (use_abreviation) ? "d" : "double";
      break;
    case DataType::FLOAT32:
      res = (use_abreviation) ? "f" : "float";
      break;
    case DataType::INT64:
      res = (use_abreviation) ? "l" : "long";
      break;
    case DataType::UINT64:
      res = (use_abreviation) ? "ul" : "ulong";
      break;
    case DataType::INT32:
      res = (use_abreviation) ? "i" : "int";
      break;
    case DataType::UINT32:
      res = (use_abreviation) ? "ui" : "uint";
      break;
    case DataType::INT8:
      res = (use_abreviation) ? "c" : "char";
      break;
    case DataType::UINT8:
      res = (use_abreviation) ? "uc" : "uchar";
      break;
    case DataType::INT16:
      res = (use_abreviation) ? "s" : "short";
      break;
    case DataType::UINT16:
      res = (use_abreviation) ? "us" : "ushort";
      break;
    default:
      throw(std::runtime_error("Unknown data type provided to cast in string."));
  }
  return res;
}

inline auto
MemoryTypeToString(const MemoryType & type) -> std::string
{
  std::string res;
  switch (type)
  {
    case MemoryType::BUFFER:
      res = "Buffer";
      break;
    case MemoryType::IMAGE1D:
      res = "Image1D";
      break;
    case MemoryType::IMAGE2D:
      res = "Image2D";
      break;
    case MemoryType::IMAGE3D:
      res = "Image3D";
      break;
    case MemoryType::SCALAR:
      res = "Scalar";
      break;
    default:
      throw(std::runtime_error("Unknown memory type provided to cast in string."));
  }
  return res;
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
