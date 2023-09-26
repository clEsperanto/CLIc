#include "backend.hpp"
#include "cle_preamble_cl.h"

#include <array>

namespace cle
{

auto
OpenCLBackend::getDevices(const std::string & type) const -> std::vector<Device::Pointer>
{
#if USE_OPENCL
  std::vector<Device::Pointer> devices;

  cl_uint platformCount = 0;
  clGetPlatformIDs(0, nullptr, &platformCount);
  if (platformCount == 0)
  {
    throw std::runtime_error("Error (ocl): Failed to find platforms.");
  }
  std::vector<cl_platform_id> platformIds(platformCount);
  clGetPlatformIDs(platformCount, platformIds.data(), nullptr);

  cl_device_type deviceType;
  if (type == "cpu")
  {
    deviceType = CL_DEVICE_TYPE_CPU;
  }
  else if (type == "gpu")
  {
    deviceType = CL_DEVICE_TYPE_GPU;
  }
  else if (type == "all")
  {
    deviceType = CL_DEVICE_TYPE_ALL;
  }
  else
  {
    std::cerr << "Warning: Unknown device type '" << type << "' provided. Default: fetching 'all' type." << std::endl;
    deviceType = CL_DEVICE_TYPE_ALL;
  }

  for (auto && platform_id : platformIds)
  {
    cl_uint deviceCount = 0;
    clGetDeviceIDs(platform_id, deviceType, 0, nullptr, &deviceCount);
    if (deviceCount == 0)
    {
      continue;
    }
    std::vector<cl_device_id> deviceIds(deviceCount);
    clGetDeviceIDs(platform_id, deviceType, deviceCount, deviceIds.data(), nullptr);
    for (auto && device_id : deviceIds)
    {
      devices.emplace_back(std::make_shared<OpenCLDevice>(platform_id, device_id));
    }
  }

  if (devices.empty())
  {
    throw std::runtime_error("Error (ocl): Failed to find devices.");
  }

  return devices;
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::getDevice(const std::string & name, const std::string & type) const -> Device::Pointer
{
#if USE_OPENCL
  auto devices = getDevices(type);
  auto ite = std::find_if(devices.begin(), devices.end(), [&name](const Device::Pointer & dev) {
    return dev->getName().find(name) != std::string::npos;
  });
  if (ite != devices.end())
  {
    return std::move(*ite);
  }
  if (!devices.empty())
  {
    return std::move(devices.back());
  }
  return nullptr;
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::getDevicesList(const std::string & type) const -> std::vector<std::string>
{
#if USE_OPENCL
  auto                     devices = getDevices(type);
  std::vector<std::string> deviceList;
  for (auto && device : devices)
  {
    deviceList.emplace_back(device->getName());
  }
  return deviceList;
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::getType() const -> Backend::Type
{
  return Backend::Type::OPENCL;
}

auto
OpenCLBackend::allocateMemory(const Device::Pointer &       device,
                              const std::array<size_t, 3> & region,
                              const dType &                 dtype,
                              const mType &                 mtype,
                              void **                       data_ptr) const -> void
{
#if USE_OPENCL
  switch (mtype)
  {
    case mType::BUFFER: {
      const size_t size = region[0] * region[1] * region[2] * toBytes(dtype);
      allocateBuffer(device, size, data_ptr);
      break;
    }
    case mType::IMAGE: {
      allocateImage(device, region, dtype, data_ptr);
      break;
    }
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::allocateBuffer(const Device::Pointer & device, const size_t & size, void ** data_ptr) -> void
{
#if USE_OPENCL
  cl_int err;
  auto   opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  auto   mem = clCreateBuffer(opencl_device->getCLContext(), CL_MEM_READ_WRITE, size, nullptr, &err);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error (ocl): Failed to allocate memory (buffer) with error code " + std::to_string(err));
  }
  *data_ptr = static_cast<void *>(new cl_mem(mem));
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::allocateImage(const Device::Pointer &       device,
                             const std::array<size_t, 3> & region,
                             const dType &                 dtype,
                             void **                       data_ptr) -> void
{
#if USE_OPENCL
  auto            opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  cl_image_format image_format = { 0 };
  image_format.image_channel_order = CL_INTENSITY;
  cl_image_desc image_description = { 0 };
  image_description.image_type = CL_MEM_OBJECT_IMAGE1D;
  image_description.image_width = region[0];
  image_description.image_height = region[1];
  image_description.image_depth = region[2];
  switch (dtype)
  {
    case dType::FLOAT: {
      image_format.image_channel_data_type = CL_FLOAT;
      break;
    }
    case dType::INT32: {
      image_format.image_channel_data_type = CL_SIGNED_INT32;
      break;
    }
    case dType::UINT32: {
      image_format.image_channel_data_type = CL_UNSIGNED_INT32;
      break;
    }
    case dType::INT8: {
      image_format.image_channel_data_type = CL_SIGNED_INT8;
      break;
    }
    case dType::UINT8: {
      image_format.image_channel_data_type = CL_UNSIGNED_INT8;
      break;
    }
    case dType::INT16: {
      image_format.image_channel_data_type = CL_SIGNED_INT16;
      break;
    }
    case dType::UINT16: {
      image_format.image_channel_data_type = CL_UNSIGNED_INT16;
      break;
    }
    default:
      image_format.image_channel_data_type = CL_FLOAT;
      std::cerr << "Warning: Unsupported data type for 'image', default type 'float' will be used." << std::endl;
      break;
  }
  if (region[1] > 1)
  {
    image_description.image_type = CL_MEM_OBJECT_IMAGE2D;
  }
  if (region[2] > 1)
  {
    image_description.image_type = CL_MEM_OBJECT_IMAGE3D;
  }
  cl_int err;
  auto   image =
    clCreateImage(opencl_device->getCLContext(), CL_MEM_READ_WRITE, &image_format, &image_description, nullptr, &err);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error (ocl): Failed to allocate memory (image) with error code " + std::to_string(err));
  }
  *data_ptr = static_cast<void *>(new cl_mem(image));
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::freeMemory(const Device::Pointer & device, const mType & mtype, void ** data_ptr) const -> void
{
#if USE_OPENCL
  auto * cl_mem_ptr = static_cast<cl_mem *>(*data_ptr);
  auto   err = clReleaseMemObject(*cl_mem_ptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error (ocl): Failed to free memory with error code " + std::to_string(err));
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::writeBuffer(const Device::Pointer &       device,
                           void **                       data_ptr,
                           const std::array<size_t, 3> & region,
                           const std::array<size_t, 3> & origin,
                           const dType &                 dtype,
                           const void *                  host_ptr) -> void
{
#if USE_OPENCL
  auto                        opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  cl_int                      err;
  const std::array<size_t, 3> host_origin = { 0, 0, 0 };
  const std::array<size_t, 3> region_ocl = { region[0] * toBytes(dtype), region[1], region[2] };
  if (region[2] > 1 || region[1] > 1)
  {
    err = clEnqueueWriteBufferRect(opencl_device->getCLCommandQueue(),
                                   *static_cast<cl_mem *>(*data_ptr),
                                   CL_TRUE,
                                   origin.data(),
                                   host_origin.data(),
                                   region_ocl.data(),
                                   0,
                                   0,
                                   0,
                                   0,
                                   host_ptr,
                                   0,
                                   nullptr,
                                   nullptr);
  }
  else
  {
    err = clEnqueueWriteBuffer(opencl_device->getCLCommandQueue(),
                               *static_cast<cl_mem *>(*data_ptr),
                               CL_TRUE,
                               origin[0],
                               region_ocl[0],
                               host_ptr,
                               0,
                               nullptr,
                               nullptr);
  }
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error (ocl): Failed to write memory (buffer) with error code " + std::to_string(err));
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::writeImage(const Device::Pointer &       device,
                          void **                       data_ptr,
                          const std::array<size_t, 3> & region,
                          const std::array<size_t, 3> & origin,
                          const dType &                 dtype,
                          const void *                  host_ptr) -> void
{
#if USE_OPENCL
  auto         opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  const size_t rowPitch = (region[1] > 1) ? region[0] * toBytes(dtype) : 0;
  const size_t slicePitch = (region[2] > 1) ? rowPitch * region[1] : 0;
  auto         err = clEnqueueWriteImage(opencl_device->getCLCommandQueue(),
                                 *static_cast<cl_mem *>(*data_ptr),
                                 CL_TRUE,
                                 origin.data(),
                                 region.data(),
                                 rowPitch,
                                 slicePitch,
                                 host_ptr,
                                 0,
                                 nullptr,
                                 nullptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error (ocl): Failed to write memory (image) with error code " + std::to_string(err));
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::writeMemory(const Device::Pointer &       device,
                           void **                       data_ptr,
                           const std::array<size_t, 3> & region,
                           const std::array<size_t, 3> & origin,
                           const dType &                 dtype,
                           const mType &                 mtype,
                           const void *                  host_ptr) const -> void
{
  switch (mtype)
  {
    case mType::BUFFER: {
      writeBuffer(device, data_ptr, region, origin, dtype, host_ptr);
      break;
    }
    case mType::IMAGE: {
      writeImage(device, data_ptr, region, origin, dtype, host_ptr);
      break;
    }
  }
}

auto
OpenCLBackend::readBuffer(const Device::Pointer &       device,
                          const void **                 data_ptr,
                          const std::array<size_t, 3> & region,
                          const std::array<size_t, 3> & origin,
                          const dType &                 dtype,
                          void *                        host_ptr) -> void
{
#if USE_OPENCL
  auto                        opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  cl_int                      err;
  const std::array<size_t, 3> region_ocl = { region[0] * toBytes(dtype), region[1], region[2] };
  const std::array<size_t, 3> host_origin = { 0, 0, 0 };
  if (region[2] > 1 || region[1] > 1)
  {
    err = clEnqueueReadBufferRect(opencl_device->getCLCommandQueue(),
                                  *static_cast<const cl_mem *>(*data_ptr),
                                  CL_TRUE,
                                  origin.data(),
                                  host_origin.data(),
                                  region_ocl.data(),
                                  0,
                                  0,
                                  0,
                                  0,
                                  host_ptr,
                                  0,
                                  nullptr,
                                  nullptr);
  }
  else
  {
    err = clEnqueueReadBuffer(opencl_device->getCLCommandQueue(),
                              *static_cast<const cl_mem *>(*data_ptr),
                              CL_TRUE,
                              origin[0],
                              region_ocl[0],
                              host_ptr,
                              0,
                              nullptr,
                              nullptr);
  }
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error (ocl): Failed to read memory (buffer) with error code " + std::to_string(err));
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::readImage(const Device::Pointer &       device,
                         const void **                 data_ptr,
                         const std::array<size_t, 3> & region,
                         const std::array<size_t, 3> & origin,
                         const dType &                 dtype,
                         void *                        host_ptr) -> void
{
#if USE_OPENCL
  auto         opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  const size_t rowPitch = (region[1] > 1) ? region[0] * toBytes(dtype) : 0;
  const size_t slicePitch = (region[2] > 1) ? rowPitch * region[1] : 0;
  auto         err = clEnqueueReadImage(opencl_device->getCLCommandQueue(),
                                *static_cast<const cl_mem *>(*data_ptr),
                                CL_TRUE,
                                origin.data(),
                                region.data(),
                                rowPitch,
                                slicePitch,
                                host_ptr,
                                0,
                                nullptr,
                                nullptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error (ocl): Failed to read memory (image) with error code " + std::to_string(err));
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::readMemory(const Device::Pointer &       device,
                          const void **                 data_ptr,
                          const std::array<size_t, 3> & region,
                          const std::array<size_t, 3> & origin,
                          const dType &                 dtype,
                          const mType &                 mtype,
                          void *                        host_ptr) const -> void
{
#if USE_OPENCL
  switch (mtype)
  {
    case mType::BUFFER: {
      readBuffer(device, data_ptr, region, origin, dtype, host_ptr);
      break;
    }
    case mType::IMAGE: {
      readImage(device, data_ptr, region, origin, dtype, host_ptr);
      break;
    }
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::copyMemoryBufferToBuffer(const Device::Pointer &       device,
                                        const void **                 src_data_ptr,
                                        const std::array<size_t, 3> & region,
                                        const std::array<size_t, 3> & origin,
                                        const size_t &                bytes,
                                        void **                       dst_data_ptr) const -> void
{
#if USE_OPENCL
  auto                        opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  cl_int                      err;
  const std::array<size_t, 3> region_ocl = { region[0] * bytes, region[1], region[2] };
  if (region[2] > 1 || region[1] > 1)
  {
    err = clEnqueueCopyBufferRect(opencl_device->getCLCommandQueue(),
                                  *static_cast<const cl_mem *>(*src_data_ptr),
                                  *static_cast<cl_mem *>(*dst_data_ptr),
                                  origin.data(),
                                  origin.data(),
                                  region_ocl.data(),
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  nullptr,
                                  nullptr);
  }
  else
  {
    err = clEnqueueCopyBuffer(opencl_device->getCLCommandQueue(),
                              *static_cast<const cl_mem *>(*src_data_ptr),
                              *static_cast<cl_mem *>(*dst_data_ptr),
                              origin[0],
                              origin[0],
                              region_ocl[0],
                              0,
                              nullptr,
                              nullptr);
  }
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error (ocl): Failed to copy memory (buffer->buffer) with error code " +
                             std::to_string(err));
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::copyMemoryBufferToImage(const Device::Pointer &       device,
                                       const void **                 src_data_ptr,
                                       const std::array<size_t, 3> & region,
                                       const std::array<size_t, 3> & origin,
                                       const size_t &                bytes,
                                       void **                       dst_data_ptr) const -> void
{
#if USE_OPENCL
  auto opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  auto err = clEnqueueCopyBufferToImage(opencl_device->getCLCommandQueue(),
                                        *static_cast<const cl_mem *>(*src_data_ptr),
                                        *static_cast<cl_mem *>(*dst_data_ptr),
                                        0,
                                        origin.data(),
                                        region.data(),
                                        0,
                                        nullptr,
                                        nullptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error (ocl): Failed to copy memory (buffer->image) with error code " +
                             std::to_string(err));
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::copyMemoryImageToBuffer(const Device::Pointer &       device,
                                       const void **                 src_data_ptr,
                                       const std::array<size_t, 3> & region,
                                       const std::array<size_t, 3> & origin,
                                       const size_t &                bytes,
                                       void **                       dst_data_ptr) const -> void
{
#if USE_OPENCL
  auto opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  auto err = clEnqueueCopyImageToBuffer(opencl_device->getCLCommandQueue(),
                                        *static_cast<const cl_mem *>(*src_data_ptr),
                                        *static_cast<cl_mem *>(*dst_data_ptr),
                                        origin.data(),
                                        region.data(),
                                        0,
                                        0,
                                        nullptr,
                                        nullptr);

  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error (ocl): Failed to copy memory (image->buffer) with error code " +
                             std::to_string(err));
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::copyMemoryImageToImage(const Device::Pointer &       device,
                                      const void **                 src_data_ptr,
                                      const std::array<size_t, 3> & region,
                                      const std::array<size_t, 3> & origin,
                                      const size_t &                bytes,
                                      void **                       dst_data_ptr) const -> void
{
#if USE_OPENCL
  auto opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  auto err = clEnqueueCopyImage(opencl_device->getCLCommandQueue(),
                                *static_cast<const cl_mem *>(*src_data_ptr),
                                *static_cast<cl_mem *>(*dst_data_ptr),
                                origin.data(),
                                origin.data(),
                                region.data(),
                                0,
                                nullptr,
                                nullptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error (ocl): Failed to copy memory (image->image) with error code " +
                             std::to_string(err));
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::setMemory(const Device::Pointer &       device,
                         void **                       data_ptr,
                         const std::array<size_t, 3> & region,
                         const std::array<size_t, 3> & origin,
                         const dType &                 dtype,
                         const mType &                 mtype,
                         const float &                 value) const -> void
{
#if USE_OPENCL
  switch (mtype)
  {
    case mType::BUFFER: {
      setBuffer(device, data_ptr, region, origin, dtype, value);
      break;
    }
    case mType::IMAGE: {
      setImage(device, data_ptr, region, origin, dtype, value);
      break;
    }
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::setBuffer(const Device::Pointer &       device,
                         void **                       data_ptr,
                         const std::array<size_t, 3> & region,
                         const std::array<size_t, 3> & origin,
                         const dType &                 dtype,
                         const float &                 value) -> void
{
#if USE_OPENCL
  auto         opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  const size_t size = region[0] * region[1] * region[2] * toBytes(dtype);
  cl_int       err;
  switch (dtype)
  {
    case dType::FLOAT: {
      auto cval = static_cast<float>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*data_ptr),
                                &cval,
                                sizeof(cval),
                                origin[0],
                                size,
                                0,
                                nullptr,
                                nullptr);
      break;
    }
    case dType::INT32: {
      auto cval = static_cast<int32_t>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*data_ptr),
                                &cval,
                                sizeof(cval),
                                origin[0],
                                size,
                                0,
                                nullptr,
                                nullptr);
      break;
    }
    case dType::UINT32: {
      auto cval = static_cast<uint32_t>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*data_ptr),
                                &cval,
                                sizeof(cval),
                                origin[0],
                                size,
                                0,
                                nullptr,
                                nullptr);
      break;
    }
    case dType::INT8: {
      auto cval = static_cast<int8_t>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*data_ptr),
                                &cval,
                                sizeof(cval),
                                origin[0],
                                size,
                                0,
                                nullptr,
                                nullptr);
      break;
    }
    case dType::UINT8: {
      auto cval = static_cast<uint8_t>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*data_ptr),
                                &cval,
                                sizeof(cval),
                                origin[0],
                                size,
                                0,
                                nullptr,
                                nullptr);
      break;
    }
    case dType::INT16: {
      auto cval = static_cast<int16_t>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*data_ptr),
                                &cval,
                                sizeof(cval),
                                origin[0],
                                size,
                                0,
                                nullptr,
                                nullptr);
      break;
    }
    case dType::UINT16: {
      auto cval = static_cast<uint16_t>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*data_ptr),
                                &cval,
                                sizeof(cval),
                                origin[0],
                                size,
                                0,
                                nullptr,
                                nullptr);
      break;
    }
    case dType::INT64: {
      auto cval = static_cast<int64_t>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*data_ptr),
                                &cval,
                                sizeof(cval),
                                origin[0],
                                size,
                                0,
                                nullptr,
                                nullptr);
      break;
    }
    case dType::UINT64: {
      auto cval = static_cast<uint64_t>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*data_ptr),
                                &cval,
                                sizeof(cval),
                                origin[0],
                                size,
                                0,
                                nullptr,
                                nullptr);
      break;
    }
    default:
      throw std::invalid_argument("Invalid Array::Type value");
  }

  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error (ocl): Failed to set memory (buffer) with error code " + std::to_string(err) + ".");
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::setImage(const Device::Pointer &       device,
                        void **                       data_ptr,
                        const std::array<size_t, 3> & region,
                        const std::array<size_t, 3> & origin,
                        const dType &                 dtype,
                        const float &                 value) -> void
{
#if USE_OPENCL
  auto   opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  cl_int err;
  switch (dtype)
  {
    case dType::FLOAT: {
      auto cval = static_cast<cl_float>(value);
      err = clEnqueueFillImage(opencl_device->getCLCommandQueue(),
                               *static_cast<cl_mem *>(*data_ptr),
                               &cval,
                               origin.data(),
                               region.data(),
                               0,
                               nullptr,
                               nullptr);
      break;
    }
    case dType::INT32:
    case dType::INT16:
    case dType::INT8: {
      auto cval = static_cast<cl_int>(value);
      err = clEnqueueFillImage(opencl_device->getCLCommandQueue(),
                               *static_cast<cl_mem *>(*data_ptr),
                               &cval,
                               origin.data(),
                               region.data(),
                               0,
                               nullptr,
                               nullptr);
      break;
    }
    case dType::UINT32:
    case dType::UINT16:
    case dType::UINT8: {
      auto cval = static_cast<cl_uint>(value);
      err = clEnqueueFillImage(opencl_device->getCLCommandQueue(),
                               *static_cast<cl_mem *>(*data_ptr),
                               &cval,
                               origin.data(),
                               region.data(),
                               0,
                               nullptr,
                               nullptr);
      break;
    }
    default:
      throw std::invalid_argument("Invalid Array::Type value");
  }
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error (ocl): Failed to set memory (image) with error code " + std::to_string(err) + ".");
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::loadProgramFromCache(const Device::Pointer & device, const std::string & hash, void * program) const
  -> void
{
#if USE_OPENCL
  if (auto opencl_device = std::dynamic_pointer_cast<OpenCLDevice>(device))
  {
    const auto & cache = opencl_device->getCache();
    auto         ite = cache.find(hash);
    if (ite != cache.end())
    {
      *static_cast<cl_program *>(program) = ite->second;
    }
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::saveProgramToCache(const Device::Pointer & device, const std::string & hash, void * program) const
  -> void
{
#if USE_OPENCL
  if (auto opencl_device = std::dynamic_pointer_cast<OpenCLDevice>(device))
  {
    opencl_device->getCache().emplace(hash, *static_cast<cl_program *>(program));
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::buildKernel(const Device::Pointer & device,
                           const std::string &     kernel_source,
                           const std::string &     kernel_name,
                           void *                  kernel) const -> void
{
#if USE_OPENCL
  cl_int      err;
  auto        opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  cl_program  prog = nullptr;
  std::string hash = std::to_string(std::hash<std::string>{}(kernel_source));
  loadProgramFromCache(device, hash, &prog);
  if (prog == nullptr)
  {
    const char * source = kernel_source.c_str();
    prog = clCreateProgramWithSource(opencl_device->getCLContext(), 1, &source, nullptr, &err);
    if (err != CL_SUCCESS)
    {
      throw std::runtime_error("Error (ocl): Failed to create program from source with error code " +
                               std::to_string(err));
    }
    cl_int buildStatus = clBuildProgram(prog, 0, nullptr, nullptr, nullptr, nullptr);
    if (buildStatus != CL_SUCCESS)
    {
      size_t      len;
      std::string buffer;
      clGetProgramBuildInfo(prog, opencl_device->getCLDevice(), CL_PROGRAM_BUILD_LOG, 0, nullptr, &len);
      buffer.resize(len);
      clGetProgramBuildInfo(prog, opencl_device->getCLDevice(), CL_PROGRAM_BUILD_LOG, len, &buffer[0], &len);
      std::cerr << "Build log: " << buffer << std::endl;
      throw std::runtime_error("Error (ocl): Failed to build program " + kernel_name + " with error code " +
                               std::to_string(err));
    }
    saveProgramToCache(device, hash, &prog);
  }
  auto ocl_kernel = clCreateKernel(prog, kernel_name.c_str(), &err);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to create kernel with error code " + std::to_string(err));
  }
  *reinterpret_cast<cl_kernel *>(kernel) = ocl_kernel;
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::executeKernel(const Device::Pointer &       device,
                             const std::string &           kernel_source,
                             const std::string &           kernel_name,
                             const std::array<size_t, 3> & global_size,
                             const std::vector<void *> &   args,
                             const std::vector<size_t> &   sizes) const -> void
{
#if USE_OPENCL
  auto      opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  cl_kernel ocl_kernel;
  try
  {
    buildKernel(device, kernel_source, kernel_name, &ocl_kernel);
  }
  catch (const std::exception & e)
  {
    throw std::runtime_error("Error (ocl): Failed to build kernel. \n\t > " + std::string(e.what()));
  }
  for (size_t i = 0; i < args.size(); i++)
  {
    auto err = clSetKernelArg(ocl_kernel, i, sizes[i], args[i]);
    if (err != CL_SUCCESS)
    {
      throw std::runtime_error("Error (ocl): Failed to set kernel arguments (" + std::to_string(err) + ").)");
    }
  }
  auto err = clEnqueueNDRangeKernel(
    opencl_device->getCLCommandQueue(), ocl_kernel, 3, nullptr, global_size.data(), nullptr, 0, nullptr, nullptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error (ocl): Failed to launch kernel (" + std::to_string(err) + ").)");
  }
  opencl_device->finish();
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::getPreamble() const -> std::string
{
  return kernel::preamble_cl;
}

} // namespace cle