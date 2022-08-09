#ifndef __CORE_CLEUTILS_HPP
#define __CORE_CLEUTILS_HPP

#include "clic.hpp"
#include <string>

#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace cle
{

// auto
// Sigma2KernelSize (const std::array<float, 3> &sigmas) -> std::array<size_t, 3>
// {
//     const float factor = 8.0;
//     const float rounding = 5.0;
//     std::array<size_t, 3> kernel_size{};
//     auto it_size (kernel_size.begin ());
//     auto size_end (kernel_size.end ());
//     auto it_sigma (sigmas.begin ());
//     auto sigma_end (sigmas.end ());
//     for (; (it_sigma != sigma_end) && (it_size != size_end); ++it_sigma, ++it_size)
//         {
//             *it_size = static_cast<size_t> (*it_sigma * factor + rounding);
//             if (*it_size % 2 == 0)
//                 {
//                     *it_size += 1;
//                 }
//         }
//     return kernel_size;
// }


struct DeviceType
{
  [[nodiscard]] auto
  IsCPU() const -> bool
  {
    return type == CL_DEVICE_TYPE_CPU;
  }
  [[nodiscard]] auto
  IsGPU() const -> bool
  {
    return type == CL_DEVICE_TYPE_GPU;
  }
  void
  UseAny()
  {
    type = CL_DEVICE_TYPE_ALL;
  }
  void
  UseCPU()
  {
    type = CL_DEVICE_TYPE_CPU;
  }
  void
  UseGPU()
  {
    type = CL_DEVICE_TYPE_GPU;
  }
  void
  Set(cl_device_type t)
  {
    type = t;
  }
  [[nodiscard]] auto
  Get() const -> cl_device_type
  {
    return type;
  }

  cl_device_type type = CL_DEVICE_TYPE_ALL;
};

struct MemAllocType
{
  void
  UseNone()
  {
    type = 0;
  }
  void
  UseHostPointer()
  {
    type = CL_MEM_USE_HOST_PTR;
  }
  void
  AllocateHostPointer()
  {
    type = CL_MEM_ALLOC_HOST_PTR;
  }
  void
  UseBest()
  {
    type = CL_MEM_USE_HOST_PTR;
  }
  [[nodiscard]] auto
  Get() const -> cl_mem_flags
  {
    return type;
  }
  void
  Set(cl_mem_flags t)
  {
    type = t;
  }

  cl_mem_flags type = 0;
};

struct HostAccessType
{
  void
  ReadOnly()
  {
    type = CL_MEM_HOST_READ_ONLY;
  }
  void
  WriteOnly()
  {
    type = CL_MEM_HOST_WRITE_ONLY;
  }
  void
  ReadWrite()
  {
    type = 0;
  }
  void
  NoAccess()
  {
    type = CL_MEM_HOST_NO_ACCESS;
  }
  [[nodiscard]] auto
  Get() const -> cl_mem_flags
  {
    return type;
  }
  void
  Set(cl_mem_flags t)
  {
    type = t;
  }

  cl_mem_flags type = 0;
};

struct KernelAccessType
{
  void
  ReadOnly()
  {
    type = CL_MEM_READ_ONLY;
  }
  void
  WriteOnly()
  {
    type = CL_MEM_WRITE_ONLY;
  }
  void
  ReadWrite()
  {
    type = CL_MEM_READ_WRITE;
  }
  [[nodiscard]] auto
  Get() const -> cl_mem_flags
  {
    return type;
  }
  void
  Set(cl_mem_flags t)
  {
    type = t;
  }

  cl_mem_flags type = CL_MEM_READ_WRITE;
};

struct ObjectType
{
  [[nodiscard]] auto
  IsImage() const -> bool
  {
    return type != CL_MEM_OBJECT_BUFFER;
  }
  [[nodiscard]] auto
  IsBuffer() const -> bool
  {
    return type == CL_MEM_OBJECT_BUFFER;
  }
  void
  Set(cl_mem_object_type t)
  {
    type = t;
  }
  [[nodiscard]] auto
  Str() const -> std::string
  {
    switch (type)
    {
      case CL_MEM_OBJECT_BUFFER:
        return "buffer";
      case CL_MEM_OBJECT_IMAGE1D:
        return "image1d";
      case CL_MEM_OBJECT_IMAGE2D:
        return "image2d";
      case CL_MEM_OBJECT_IMAGE3D:
        return "image3d";
      default:
        return "unknown";
    }
  }
  [[nodiscard]] auto
  Get() const -> cl_mem_object_type
  {
    return type;
  };

  cl_mem_object_type type = CL_MEM_OBJECT_BUFFER;
};

struct ChannelsType
{
  void
  Set(int nb_channels)
  {
    switch (nb_channels)
    {
      case 1:
        type = CL_R;
      case 3:
        type = CL_RGB;
      case 4:
        type = CL_RGBA;
      default:
        type = CL_R;
    }
  }
  [[nodiscard]] auto
  GetNbChannels() const -> int
  {
    switch (type)
    {
      case CL_R:
        return 1;
      case CL_RGB:
        return 3;
      case CL_RGBA:
        return 4;
      default:
        return 0;
    }
  }
  [[nodiscard]] auto
  Get() const -> cl_channel_order
  {
    return type;
  }

  cl_channel_order type = CL_R;
};

struct DataType
{
  template <class T>
  void
  Set()
  {
    static_assert(std::is_fundamental<T>::value, "Object can only be of native type");
    if (std::is_same<T, float>::value)
    {
      type = CL_FLOAT;
      return;
    }
    if (std::is_same<T, int>::value)
    {
      type = CL_SIGNED_INT32;
      return;
    }
    if (std::is_same<T, unsigned int>::value)
    {
      type = CL_UNSIGNED_INT32;
      return;
    }
    if (std::is_same<T, char>::value)
    {
      type = CL_SIGNED_INT8;
      return;
    }
    if (std::is_same<T, unsigned char>::value)
    {
      type = CL_UNSIGNED_INT8;
      return;
    }
    if (std::is_same<T, short>::value)
    {
      type = CL_SIGNED_INT16;
      return;
    }
    if (std::is_same<T, unsigned short>::value)
    {
      type = CL_UNSIGNED_INT16;
      return;
    }
    type = CL_FLOAT;
  }
  void
  Set(cl_channel_type t)
  {
    type = t;
  }
  [[nodiscard]] auto
  Str() const -> std::string
  {
    switch (type)
    {
      case CL_SIGNED_INT8:
        return "char";
      case CL_SIGNED_INT16:
        return "short";
      case CL_SIGNED_INT32:
        return "int";
      case CL_UNSIGNED_INT8:
        return "uchar";
      case CL_UNSIGNED_INT16:
        return "ushort";
      case CL_UNSIGNED_INT32:
        return "uint";
      case CL_FLOAT:
        return "float";
      default:
        return "unknown";
    }
  }
  [[nodiscard]] auto
  Str_s() const -> std::string
  {
    switch (type)
    {
      case CL_SIGNED_INT8:
        return "c";
      case CL_SIGNED_INT16:
        return "s";
      case CL_SIGNED_INT32:
        return "i";
      case CL_UNSIGNED_INT8:
        return "uc";
      case CL_UNSIGNED_INT16:
        return "us";
      case CL_UNSIGNED_INT32:
        return "ui";
      case CL_FLOAT:
        return "f";
      default:
        return "unknown";
    }
  }
  [[nodiscard]] auto
  Bytes() const -> size_t
  {
    switch (type)
    {
      case CL_SIGNED_INT8:
        return sizeof(int8_t);
      case CL_SIGNED_INT16:
        return sizeof(int16_t);
      case CL_SIGNED_INT32:
        return sizeof(int32_t);
      case CL_UNSIGNED_INT8:
        return sizeof(uint8_t);
      case CL_UNSIGNED_INT16:
        return sizeof(uint16_t);
      case CL_UNSIGNED_INT32:
        return sizeof(uint32_t);
      case CL_FLOAT:
        return sizeof(float);
      default:
        return sizeof(float);
    }
  }
  [[nodiscard]] auto
  Get() const -> cl_channel_type
  {
    return type;
  }

  cl_channel_type type = CL_FLOAT;
};

struct BuildStatus
{
  [[nodiscard]] auto
  IsSuccess() const -> bool
  {
    return type == CL_BUILD_SUCCESS;
  }
  [[nodiscard]] auto
  IsFail() const -> bool
  {
    return type == CL_BUILD_ERROR;
  }
  void
  Set(const cl_build_status status)
  {
    type = status;
  }
  [[nodiscard]] auto
  Get() const -> cl_build_status
  {
    return type;
  }

  cl_build_status type;
};

// inline auto
// exec (const char *cmd) -> std::string
// {
//     const int buf_size = 128;
//     std::array<char, buf_size> buffer{};
//     std::string result;
// #ifdef WIN32
//     std::unique_ptr<FILE, decltype (&_pclose)> pipe (_popen (cmd, "r"), _pclose);
// #elif __APPLE__
//     std::unique_ptr<FILE, decltype (&pclose)> pipe (popen (cmd, "r"), pclose);
// #elif __linux__
//     std::unique_ptr<FILE, decltype (&pclose)> pipe (popen (cmd, "r"), pclose);
// #endif
//     if (!pipe)
//         {
//             throw std::runtime_error ("popen() failed!");
//         }
//     while (fgets (buffer.data (), buffer.size (), pipe.get ()) != nullptr)
//         {
//             result += buffer.data ();
//         }
//     return result;
// }

} // namespace cle

#endif // __CORE_CLEUTILS_HPP
