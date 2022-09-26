#ifndef __CORE_CLETYPES_HPP
#define __CORE_CLETYPES_HPP

#include "clic.hpp"
namespace cle
{

enum ObjectType
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
  INTENSITY = CL_R,
  RGBA = CL_RGBA
};

enum DataType
{
  FLOAT = CL_FLOAT,
  INT8 = CL_SIGNED_INT8,
  INT16 = CL_SIGNED_INT16,
  INT32 = CL_SIGNED_INT32,
  UINT8 = CL_UNSIGNED_INT8,
  UINT16 = CL_UNSIGNED_INT16,
  UINT32 = CL_UNSIGNED_INT32
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
  DEFAULT = 0
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

inline auto
ObjectTypeToString(const ObjectType & type) -> std::string
{
  std::string res;
  switch (type)
  {
    case BUFFER:
      res = "buffer";
      break;
    case SCALAR:
      res = "scalar";
      break;
    case IMAGE1D:
      res = "image1d";
      break;
    case IMAGE2D:
      res = "image2d";
      break;
    case IMAGE3D:
      res = "image3d";
      break;
    default:
      res = "unknown";
  }
  return res;
}


inline auto
DataTypeToString(const DataType & type, const bool & short_version = false) -> std::string
{
  std::string res;
  switch (type)
  {
    case CL_SIGNED_INT8:
      res = (short_version) ? "c" : "char";
      break;
    case CL_SIGNED_INT16:
      res = (short_version) ? "s" : "short";
      break;
    case CL_SIGNED_INT32:
      res = (short_version) ? "i" : "int";
      break;
    case CL_UNSIGNED_INT8:
      res = (short_version) ? "uc" : "uchar";
      break;
    case CL_UNSIGNED_INT16:
      res = (short_version) ? "us" : "ushort";
      break;
    case CL_UNSIGNED_INT32:
      res = (short_version) ? "ui" : "uint";
      break;
    case CL_FLOAT:
      res = (short_version) ? "f" : "float";
      break;
    default:
      res = "unknown";
  }
  return res;
}

template <class T>
inline auto
TypeToDataType() -> DataType
{
  static_assert(std::is_fundamental<T>::value, "Object can only be of native type");
  if (std::is_same<T, float>::value)
  {
    return FLOAT;
  }
  if (std::is_same<T, int>::value)
  {
    return INT32;
  }
  if (std::is_same<T, unsigned int>::value)
  {
    return UINT32;
  }
  if (std::is_same<T, char>::value)
  {
    return INT8;
  }
  if (std::is_same<T, unsigned char>::value)
  {
    return UINT8;
  }
  if (std::is_same<T, short>::value)
  {
    return INT16;
  }
  if (std::is_same<T, unsigned short>::value)
  {
    return UINT16;
  }
  return FLOAT;
}

inline auto
DataTypeToSizeOf(const DataType & type) -> size_t
{
  size_t res;
  switch (type)
  {
    case CL_SIGNED_INT8:
      res = sizeof(int8_t);
      break;
    case CL_SIGNED_INT16:
      res = sizeof(int16_t);
      break;
    case CL_SIGNED_INT32:
      res = sizeof(int32_t);
      break;
    case CL_UNSIGNED_INT8:
      res = sizeof(uint8_t);
      break;
    case CL_UNSIGNED_INT16:
      res = sizeof(uint16_t);
      break;
    case CL_UNSIGNED_INT32:
      res = sizeof(uint32_t);
      break;
    case CL_FLOAT:
      res = sizeof(float);
      break;
    default:
      res = sizeof(float);
  }
  return res;
}

} // namespace cle

#endif // __CORE_CLETYPES_HPP
