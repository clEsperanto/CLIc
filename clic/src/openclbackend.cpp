#include "backend.hpp"
#include "cle_preamble_cl.h"

#include <unordered_map>

namespace cle
{

#if USE_OPENCL
[[nodiscard]] static auto
getErrorString(const cl_int & error) -> std::string
{
  static const std::unordered_map<cl_int, std::string> openCLErrorToStr = {
    { CL_SUCCESS, "CL_SUCCESS" },
    { CL_DEVICE_NOT_FOUND, "CL_DEVICE_NOT_FOUND" },
    { CL_DEVICE_NOT_AVAILABLE, "CL_DEVICE_NOT_AVAILABLE" },
    { CL_COMPILER_NOT_AVAILABLE, "CL_COMPILER_NOT_AVAILABLE" },
    { CL_MEM_OBJECT_ALLOCATION_FAILURE, "CL_MEM_OBJECT_ALLOCATION_FAILURE" },
    { CL_OUT_OF_RESOURCES, "CL_OUT_OF_RESOURCES" },
    { CL_OUT_OF_HOST_MEMORY, "CL_OUT_OF_HOST_MEMORY" },
    { CL_PROFILING_INFO_NOT_AVAILABLE, "CL_PROFILING_INFO_NOT_AVAILABLE" },
    { CL_MEM_COPY_OVERLAP, "CL_MEM_COPY_OVERLAP" },
    { CL_IMAGE_FORMAT_MISMATCH, "CL_IMAGE_FORMAT_MISMATCH" },
    { CL_IMAGE_FORMAT_NOT_SUPPORTED, "CL_IMAGE_FORMAT_NOT_SUPPORTED" },
    { CL_BUILD_PROGRAM_FAILURE, "CL_BUILD_PROGRAM_FAILURE" },
    { CL_MAP_FAILURE, "CL_MAP_FAILURE" },
    { CL_INVALID_VALUE, "CL_INVALID_VALUE" },
    { CL_INVALID_DEVICE_TYPE, "CL_INVALID_DEVICE_TYPE" },
    { CL_INVALID_PLATFORM, "CL_INVALID_PLATFORM" },
    { CL_INVALID_DEVICE, "CL_INVALID_DEVICE" },
    { CL_INVALID_CONTEXT, "CL_INVALID_CONTEXT" },
    { CL_INVALID_QUEUE_PROPERTIES, "CL_INVALID_QUEUE_PROPERTIES" },
    { CL_INVALID_COMMAND_QUEUE, "CL_INVALID_COMMAND_QUEUE" },
    { CL_INVALID_HOST_PTR, "CL_INVALID_HOST_PTR" },
    { CL_INVALID_MEM_OBJECT, "CL_INVALID_MEM_OBJECT" },
    { CL_INVALID_IMAGE_FORMAT_DESCRIPTOR, "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR" },
    { CL_INVALID_IMAGE_SIZE, "CL_INVALID_IMAGE_SIZE" },
    { CL_INVALID_SAMPLER, "CL_INVALID_SAMPLER" },
    { CL_INVALID_BINARY, "CL_INVALID_BINARY" },
    { CL_INVALID_BUILD_OPTIONS, "CL_INVALID_BUILD_OPTIONS" },
    { CL_INVALID_PROGRAM, "CL_INVALID_PROGRAM" },
    { CL_INVALID_PROGRAM_EXECUTABLE, "CL_INVALID_PROGRAM_EXECUTABLE" },
    { CL_INVALID_KERNEL_NAME, "CL_INVALID_KERNEL_NAME" },
    { CL_INVALID_KERNEL_DEFINITION, "CL_INVALID_KERNEL_DEFINITION" },
    { CL_INVALID_KERNEL, "CL_INVALID_KERNEL" },
    { CL_INVALID_ARG_INDEX, "CL_INVALID_ARG_INDEX" },
    { CL_INVALID_ARG_VALUE, "CL_INVALID_ARG_VALUE" },
    { CL_INVALID_ARG_SIZE, "CL_INVALID_ARG_SIZE" },
    { CL_INVALID_KERNEL_ARGS, "CL_INVALID_KERNEL_ARGS" },
    { CL_INVALID_WORK_DIMENSION, "CL_INVALID_WORK_DIMENSION" },
    { CL_INVALID_WORK_GROUP_SIZE, "CL_INVALID_WORK_GROUP_SIZE" },
    { CL_INVALID_WORK_ITEM_SIZE, "CL_INVALID_WORK_ITEM_SIZE" },
    { CL_INVALID_GLOBAL_OFFSET, "CL_INVALID_GLOBAL_OFFSET" },
    { CL_INVALID_EVENT_WAIT_LIST, "CL_INVALID_EVENT_WAIT_LIST" },
    { CL_INVALID_EVENT, "CL_INVALID_EVENT" },
    { CL_INVALID_OPERATION, "CL_INVALID_OPERATION" },
    { CL_INVALID_GL_OBJECT, "CL_INVALID_GL_OBJECT" },
    { CL_INVALID_BUFFER_SIZE, "CL_INVALID_BUFFER_SIZE" },
    { CL_INVALID_MIP_LEVEL, "CL_INVALID_MIP_LEVEL" },
    { CL_INVALID_GLOBAL_WORK_SIZE, "CL_INVALID_GLOBAL_WORK_SIZE" },
  };
  auto ite = openCLErrorToStr.find(error);
  return (ite != openCLErrorToStr.end()) ? ite->second : "CL_UNKNOWN_ERROR";
}
#endif

OpenCLBackend::OpenCLBackend()
{
#if USE_OPENCL
  OpenCLBackend::initialiseRessources();
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

OpenCLBackend::~OpenCLBackend()
{
#if USE_OPENCL
  if (!device_list_.empty())
  {
    device_list_.clear();
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::initialiseRessources() -> void
{
#if USE_OPENCL

  // explore ressources available
  std::unordered_map<cl_platform_id, std::vector<cl_device_id>> ressources;
  size_t                                                        device_counter = 0;

  cl_uint platformCount = 0;
  cl_int  err = clGetPlatformIDs(0, nullptr, &platformCount);
  if (err != CL_SUCCESS || platformCount == 0)
  {
    throw std::runtime_error("Error: Failed to find any OpenCL compatible platforms.");
  }
  std::vector<cl_platform_id> platformIds(platformCount);
  err = clGetPlatformIDs(platformCount, platformIds.data(), nullptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to get OpenCL platform IDs.");
  }
  for (const auto & platform_id : platformIds)
  {
    cl_uint deviceCount = 0;
    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceCount);
    if (err != CL_SUCCESS || deviceCount == 0)
    {
      continue;
    }

    std::vector<cl_device_id> deviceIds(deviceCount);
    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, deviceCount, deviceIds.data(), nullptr);
    if (err != CL_SUCCESS)
    {
      continue;
    }
    ressources[platform_id] = deviceIds;
    device_counter += deviceCount;
  }
  if (ressources.empty())
  {
    std::cerr << "Warning: Failed to find OpenCL compatible devices." << std::endl;
  }

  // // allocate ressources (1 context = n devices)
  // device_list_.reserve(device_counter);
  // for (const auto & [platform_id, device_ids] : ressources)
  // {
  //   cl_uint num_devices = device_ids.size();
  //   auto    context = std::make_shared<OpenCLDevice::Context>(
  //     clCreateContext(nullptr, num_devices, device_ids.data(), nullptr, nullptr, nullptr));
  //   if (context == nullptr)
  //   {
  //     throw std::runtime_error("Error: Failed to create OpenCL context.");
  //   }
  //   size_t device_index = 0;
  //   for (const auto & device_id : device_ids)
  //   {
  //     auto command_queue =
  //       std::make_shared<OpenCLDevice::CommandQueue>(clCreateCommandQueue(context->get(), device_id, 0, nullptr));
  //     if (command_queue == nullptr)
  //     {
  //       throw std::runtime_error("Error: Failed to create OpenCL command queue.");
  //     }
  //     device_list_.emplace_back(std::make_shared<OpenCLDevice>(
  //       std::make_shared<OpenCLDevice::Ressources>(platform_id, device_id, device_index++), context, command_queue));
  //   }
  // }

  // allocate resources (1 context = 1 device)
  device_list_.reserve(device_counter);
  for (const auto & [platform_id, device_ids] : ressources)
  {
    size_t device_index = 0;
    for (const auto & device_id : device_ids)
    {
      auto context =
        std::make_shared<OpenCLDevice::Context>(clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, nullptr));
      if (context == nullptr)
      {
        throw std::runtime_error("Error: Failed to create OpenCL context.");
      }
      auto command_queue =
        std::make_shared<OpenCLDevice::CommandQueue>(clCreateCommandQueue(context->get(), device_id, 0, nullptr));
      if (command_queue == nullptr)
      {
        throw std::runtime_error("Error: Failed to create OpenCL command queue.");
      }
      device_list_.emplace_back(std::make_shared<OpenCLDevice>(
        std::make_shared<OpenCLDevice::Ressources>(platform_id, device_id, device_index++), context, command_queue));
    }
  }

#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::getDevices(const std::string & type) const -> std::vector<Device::Pointer>
{
  if (type != "gpu" && type != "cpu")
  {
    return device_list_;
  }
  std::vector<Device::Pointer> filtered_devices;
  filtered_devices.reserve(device_list_.size()); // Reserve space to avoid multiple allocations
  std::copy_if(device_list_.begin(),
               device_list_.end(),
               std::back_inserter(filtered_devices),
               [&type](const Device::Pointer & device) { return device->getDeviceType() == type; });
  filtered_devices.shrink_to_fit(); // Reduce memory usage if the filtered list is smaller
  return filtered_devices;
}

auto
OpenCLBackend::getDevice(const std::string & name, const std::string & type) const -> Device::Pointer
{
#if USE_OPENCL

  // if no device in system, return nullptr
  if (device_list_.empty())
  {
    std::cerr << "Warning: Fail to find any OpenCL compatible devices." << std::endl;
    return nullptr;
  }

  // if no device of the specified type, return the last device from system
  const auto devices = getDevices(type);
  if (devices.empty())
  {
    return device_list_.back();
  }

  // if no specific device name, return the last device of the specified type
  if (name.empty())
  {
    return devices.back();
  }

  // search for the device by name
  auto lower_case_name = to_lower(name);
  auto ite = std::find_if(devices.begin(), devices.end(), [&lower_case_name](const Device::Pointer & dev) {
    return (to_lower(dev->getName()).find(lower_case_name) != std::string::npos);
  });
  if (ite != devices.end())
  {
    return *ite;
  }

  // if no device found, return the last device of the specified type
  return devices.back();

#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::getDeviceFromIndex(size_t index, const std::string & type) const -> Device::Pointer
{
#if USE_OPENCL
  // if no device in system, return nullptr
  if (device_list_.empty())
  {
    std::cerr << "Warning: Fail to find any OpenCL compatible devices." << std::endl;
    return nullptr;
  }

  auto devices = getDevices(type);
  if (devices.empty())
  {
    return device_list_.back();
  }

  if (index < devices.size())
  {
    return devices[index];
  }

  return devices.back();
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
  deviceList.reserve(devices.size());
  std::transform(devices.begin(), devices.end(), std::back_inserter(deviceList), [](const Device::Pointer & device) {
    return device->getName();
  });
  return deviceList;
#else
  throw std::runtime_error
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
    throw std::runtime_error("Error: Fail to allocate buffer memory. OpenCL error : " + getErrorString(err) + " (" +
                             std::to_string(err) + ").");
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
  image_format.image_channel_order = CL_R; // WARNING: see https://github.com/clEsperanto/CLIc/issues/317
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
    throw std::runtime_error("Error: Fail to allocate image memory. OpenCL error : " + getErrorString(err) + " (" +
                             std::to_string(err) + ").");
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
  if (data_ptr == nullptr || *data_ptr == nullptr)
  {
    throw std::invalid_argument("Error: data_ptr is null.");
  }

  auto * cl_mem_ptr = static_cast<cl_mem *>(*data_ptr);
  auto   err = clReleaseMemObject(*cl_mem_ptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to free memory. OpenCL error: " + getErrorString(err) + " (" +
                             std::to_string(err) + ").");
  }

  *data_ptr = nullptr; // Reset the pointer to avoid dangling pointers
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::writeBuffer(const Device::Pointer &       device,
                           void **                       buffer_ptr,
                           const std::array<size_t, 3> & buffer_shape,
                           const std::array<size_t, 3> & buffer_origin,
                           const std::array<size_t, 3> & region,
                           const void *                  host_ptr) -> void
{
#if USE_OPENCL
  auto opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);

  cl_bool blocking_write = CL_TRUE;
  size_t  buffer_row_pitch = buffer_shape[1] > 1 ? buffer_shape[0] : 0;
  size_t  buffer_slice_pitch = buffer_shape[2] > 1 ? buffer_shape[0] * buffer_shape[1] : 0;

  const std::array<size_t, 3> host_origin = { 0, 0, 0 };

  cl_int err;
  if (buffer_shape[2] > 1 || buffer_shape[1] > 1)
  {
    err = clEnqueueWriteBufferRect(opencl_device->getCLCommandQueue(),
                                   *static_cast<cl_mem *>(*buffer_ptr),
                                   blocking_write,
                                   buffer_origin.data(),
                                   host_origin.data(),
                                   region.data(),
                                   buffer_row_pitch,
                                   buffer_slice_pitch,
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
                               *static_cast<cl_mem *>(*buffer_ptr),
                               blocking_write,
                               buffer_origin[0],
                               region[0],
                               host_ptr,
                               0,
                               nullptr,
                               nullptr);
  }
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Fail to write buffer memory. OpenCL error : " + getErrorString(err) + " (" +
                             std::to_string(err) + ").");
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::writeImage(const Device::Pointer &       device,
                          void **                       buffer_ptr,
                          const std::array<size_t, 3> & buffer_shape,
                          const std::array<size_t, 3> & buffer_origin,
                          const std::array<size_t, 3> & region,
                          const void *                  host_ptr) -> void
{
#if USE_OPENCL
  auto opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);

  cl_bool blocking_write = CL_TRUE;
  auto    err = clEnqueueWriteImage(opencl_device->getCLCommandQueue(),
                                 *static_cast<cl_mem *>(*buffer_ptr),
                                 blocking_write,
                                 buffer_origin.data(),
                                 region.data(),
                                 0,
                                 0,
                                 host_ptr,
                                 0,
                                 nullptr,
                                 nullptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Fail to write image memory. OpenCL error : " + getErrorString(err) + " (" +
                             std::to_string(err) + ").");
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::writeMemory(const Device::Pointer & device,
                           void **                 buffer_ptr,
                           std::array<size_t, 3> & buffer_shape,
                           std::array<size_t, 3> & buffer_origin,
                           std::array<size_t, 3> & region,
                           const dType &           dtype,
                           const mType &           mtype,
                           const void *            host_ptr) const -> void
{

  switch (mtype)
  {
    case mType::BUFFER: {
      buffer_shape[0] *= toBytes(dtype);
      buffer_origin[0] *= toBytes(dtype);
      region[0] *= toBytes(dtype);
      writeBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
      break;
    }
    case mType::IMAGE: {
      writeImage(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
      break;
    }
  }
}

auto
OpenCLBackend::readBuffer(const Device::Pointer &       device,
                          const void **                 buffer_ptr,
                          const std::array<size_t, 3> & buffer_shape,
                          const std::array<size_t, 3> & buffer_origin,
                          const std::array<size_t, 3> & region,
                          void *                        host_ptr) -> void
{
#if USE_OPENCL
  auto opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);

  cl_bool blocking_read = CL_TRUE;
  size_t  buffer_row_pitch = buffer_shape[1] > 1 ? buffer_shape[0] : 0;
  size_t  buffer_slice_pitch = buffer_shape[2] > 1 ? buffer_shape[0] * buffer_shape[1] : 0;

  const std::array<size_t, 3> host_origin = { 0, 0, 0 };

  cl_int err;
  if (buffer_shape[2] > 1 || buffer_shape[1] > 1)
  {
    err = clEnqueueReadBufferRect(opencl_device->getCLCommandQueue(),
                                  *static_cast<const cl_mem *>(*buffer_ptr),
                                  blocking_read,
                                  buffer_origin.data(),
                                  host_origin.data(),
                                  region.data(),
                                  buffer_row_pitch,
                                  buffer_slice_pitch,
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
                              *static_cast<const cl_mem *>(*buffer_ptr),
                              blocking_read,
                              buffer_origin[0],
                              region[0],
                              host_ptr,
                              0,
                              nullptr,
                              nullptr);
  }
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Fail to read buffer memory. OpenCL error : " + getErrorString(err) + " (" +
                             std::to_string(err) + ").");
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::readImage(const Device::Pointer &       device,
                         const void **                 buffer_ptr,
                         const std::array<size_t, 3> & buffer_shape,
                         const std::array<size_t, 3> & buffer_origin,
                         const std::array<size_t, 3> & region,
                         void *                        host_ptr) -> void
{
#if USE_OPENCL
  auto opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);

  cl_bool blocking_read = CL_TRUE;

  auto err = clEnqueueReadImage(opencl_device->getCLCommandQueue(),
                                *static_cast<const cl_mem *>(*buffer_ptr),
                                blocking_read,
                                buffer_origin.data(),
                                region.data(),
                                0,
                                0,
                                host_ptr,
                                0,
                                nullptr,
                                nullptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Fail to read image memory. OpenCL error : " + getErrorString(err) + " (" +
                             std::to_string(err) + ").");
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::readMemory(const Device::Pointer & device,
                          const void **           buffer_ptr,
                          std::array<size_t, 3> & buffer_shape,
                          std::array<size_t, 3> & buffer_origin,
                          std::array<size_t, 3> & region,
                          const dType &           dtype,
                          const mType &           mtype,
                          void *                  host_ptr) const -> void
{
#if USE_OPENCL
  switch (mtype)
  {
    case mType::BUFFER: {
      buffer_shape[0] *= toBytes(dtype);
      buffer_origin[0] *= toBytes(dtype);
      region[0] *= toBytes(dtype);
      readBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
      break;
    }
    case mType::IMAGE: {
      readImage(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
      break;
    }
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::copyMemoryBufferToBuffer(const Device::Pointer & device,
                                        const void **           src_ptr,
                                        std::array<size_t, 3> & src_origin,
                                        std::array<size_t, 3> & src_shape,
                                        void **                 dst_ptr,
                                        std::array<size_t, 3> & dst_origin,
                                        std::array<size_t, 3> & dst_shape,
                                        std::array<size_t, 3> & region,
                                        const size_t &          bytes) const -> void
{
#if USE_OPENCL
  auto   opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  cl_int err;

  region[0] *= bytes;
  src_origin[0] *= bytes;
  src_shape[0] *= bytes;
  dst_origin[0] *= bytes;
  dst_shape[0] *= bytes;

  size_t src_row_pitch = src_shape[1] > 1 ? src_shape[0] : 0;
  size_t src_slice_pitch = src_shape[2] > 1 ? src_shape[0] * src_shape[1] : 0;
  size_t dst_row_pitch = dst_shape[1] > 1 ? dst_shape[0] : 0;
  size_t dst_slice_pitch = dst_shape[2] > 1 ? dst_shape[0] * dst_shape[1] : 0;

  if (src_shape[2] > 1 || src_shape[1] > 1)
  {
    err = clEnqueueCopyBufferRect(opencl_device->getCLCommandQueue(),
                                  *static_cast<const cl_mem *>(*src_ptr),
                                  *static_cast<cl_mem *>(*dst_ptr),
                                  src_origin.data(),
                                  dst_origin.data(),
                                  region.data(),
                                  src_row_pitch,
                                  src_slice_pitch,
                                  dst_row_pitch,
                                  dst_slice_pitch,
                                  0,
                                  nullptr,
                                  nullptr);
  }
  else
  {
    err = clEnqueueCopyBuffer(opencl_device->getCLCommandQueue(),
                              *static_cast<const cl_mem *>(*src_ptr),
                              *static_cast<cl_mem *>(*dst_ptr),
                              src_origin[0],
                              dst_origin[0],
                              region[0],
                              0,
                              nullptr,
                              nullptr);
  }
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Fail to copy memory from buffer to buffer. OpenCL error : " + getErrorString(err) +
                             " (" + std::to_string(err) + ").");
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::copyMemoryBufferToImage(const Device::Pointer & device,
                                       const void **           src_ptr,
                                       std::array<size_t, 3> & src_origin,
                                       std::array<size_t, 3> & src_shape,
                                       void **                 dst_ptr,
                                       std::array<size_t, 3> & dst_origin,
                                       std::array<size_t, 3> & dst_shape,
                                       std::array<size_t, 3> & region,
                                       const size_t &          bytes) const -> void
{
#if USE_OPENCL
  auto opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);

  // not needed when copy from buffer to image
  // region[0] *= bytes;
  // src_origin[0] *= bytes;
  // src_shape[0] *= bytes;
  // dst_origin[0] *= bytes;

  size_t src_row_pitch = src_shape[1] > 1 ? src_shape[0] : 0;
  size_t src_slice_pitch = src_shape[2] > 1 ? src_shape[0] * src_shape[1] : 0;
  size_t bufferOffset = src_origin[0] + src_origin[1] * src_row_pitch + src_origin[2] * src_slice_pitch;

  auto err = clEnqueueCopyBufferToImage(opencl_device->getCLCommandQueue(),
                                        *static_cast<const cl_mem *>(*src_ptr),
                                        *static_cast<cl_mem *>(*dst_ptr),
                                        bufferOffset,
                                        dst_origin.data(),
                                        region.data(),
                                        0,
                                        nullptr,
                                        nullptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Fail to copy memory from buffer to image. OpenCL error : " + getErrorString(err) +
                             " (" + std::to_string(err) + ").");
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::copyMemoryImageToBuffer(const Device::Pointer & device,
                                       const void **           src_ptr,
                                       std::array<size_t, 3> & src_origin,
                                       std::array<size_t, 3> & src_shape,
                                       void **                 dst_ptr,
                                       std::array<size_t, 3> & dst_origin,
                                       std::array<size_t, 3> & dst_shape,
                                       std::array<size_t, 3> & region,
                                       const size_t &          bytes) const -> void
{
#if USE_OPENCL
  auto opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);

  // region[0] *= bytes;
  // src_origin[0] *= bytes;
  // dst_shape[0] *= bytes;
  // dst_origin[0] *= bytes;

  size_t dst_row_pitch = dst_shape[1] > 1 ? dst_shape[0] : 0;
  size_t dst_slice_pitch = dst_shape[2] > 1 ? dst_shape[0] * dst_shape[1] : 0;
  size_t bufferOffset = src_origin[0] + src_origin[1] * dst_row_pitch + src_origin[2] * dst_slice_pitch;

  auto err = clEnqueueCopyImageToBuffer(opencl_device->getCLCommandQueue(),
                                        *static_cast<const cl_mem *>(*src_ptr),
                                        *static_cast<cl_mem *>(*dst_ptr),
                                        src_origin.data(),
                                        region.data(),
                                        bufferOffset,
                                        0,
                                        nullptr,
                                        nullptr);

  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Fail to copy memory from image to buffer. OpenCL error : " + getErrorString(err) +
                             " (" + std::to_string(err) + ").");
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::copyMemoryImageToImage(const Device::Pointer & device,
                                      const void **           src_ptr,
                                      std::array<size_t, 3> & src_origin,
                                      std::array<size_t, 3> & src_shape,
                                      void **                 dst_ptr,
                                      std::array<size_t, 3> & dst_origin,
                                      std::array<size_t, 3> & dst_shape,
                                      std::array<size_t, 3> & region,
                                      const size_t &          bytes) const -> void
{
#if USE_OPENCL
  auto opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);

  // region[0] *= bytes;
  // src_origin[0] *= bytes;
  // src_shape[0] *= bytes;
  // dst_origin[0] *= bytes;
  // dst_shape[0] *= bytes;

  auto err = clEnqueueCopyImage(opencl_device->getCLCommandQueue(),
                                *static_cast<const cl_mem *>(*src_ptr),
                                *static_cast<cl_mem *>(*dst_ptr),
                                src_origin.data(),
                                dst_origin.data(),
                                region.data(),
                                0,
                                nullptr,
                                nullptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Fail to copy memory from image to image. OpenCL error : " + getErrorString(err) +
                             " (" + std::to_string(err) + ").");
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::setMemory(const Device::Pointer & device,
                         void **                 buffer_ptr,
                         std::array<size_t, 3> & buffer_shape,
                         std::array<size_t, 3> & buffer_origin,
                         std::array<size_t, 3> & region,
                         const dType &           dtype,
                         const mType &           mtype,
                         const float &           value) const -> void
{
#if USE_OPENCL
  switch (mtype)
  {
    case mType::BUFFER: {
      setBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, dtype, value);
      break;
    }
    case mType::IMAGE: {
      setImage(device, buffer_ptr, buffer_shape, buffer_origin, region, dtype, value);
      break;
    }
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::setBuffer(const Device::Pointer &       device,
                         void **                       buffer_ptr,
                         const std::array<size_t, 3> & buffer_shape,
                         const std::array<size_t, 3> & buffer_origin,
                         const std::array<size_t, 3> & region,
                         const dType &                 dtype,
                         const float &                 value) -> void
{
#if USE_OPENCL
  auto opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);

  const size_t size = region[0] * region[1] * region[2] * toBytes(dtype);
  cl_int       err;
  switch (dtype)
  {
    case dType::FLOAT: {
      auto cval = static_cast<float>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*buffer_ptr),
                                &cval,
                                sizeof(cval),
                                0,
                                size,
                                0,
                                nullptr,
                                nullptr);
      break;
    }
    case dType::INT32: {
      auto cval = static_cast<int32_t>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*buffer_ptr),
                                &cval,
                                sizeof(cval),
                                0,
                                size,
                                0,
                                nullptr,
                                nullptr);
      break;
    }
    case dType::UINT32: {
      auto cval = static_cast<uint32_t>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*buffer_ptr),
                                &cval,
                                sizeof(cval),
                                0,
                                size,
                                0,
                                nullptr,
                                nullptr);
      break;
    }
    case dType::INT8: {
      auto cval = static_cast<int8_t>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*buffer_ptr),
                                &cval,
                                sizeof(cval),
                                0,
                                size,
                                0,
                                nullptr,
                                nullptr);
      break;
    }
    case dType::UINT8: {
      auto cval = static_cast<uint8_t>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*buffer_ptr),
                                &cval,
                                sizeof(cval),
                                0,
                                size,
                                0,
                                nullptr,
                                nullptr);
      break;
    }
    case dType::INT16: {
      auto cval = static_cast<int16_t>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*buffer_ptr),
                                &cval,
                                sizeof(cval),
                                0,
                                size,
                                0,
                                nullptr,
                                nullptr);
      break;
    }
    case dType::UINT16: {
      auto cval = static_cast<uint16_t>(value);
      err = clEnqueueFillBuffer(opencl_device->getCLCommandQueue(),
                                *static_cast<cl_mem *>(*buffer_ptr),
                                &cval,
                                sizeof(cval),
                                0,
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
    throw std::runtime_error("Error: Fail to fill buffer memory. OpenCL error : " + getErrorString(err) + " (" +
                             std::to_string(err) + ").");
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

auto
OpenCLBackend::setImage(const Device::Pointer &       device,
                        void **                       buffer_ptr,
                        const std::array<size_t, 3> & buffer_shape,
                        const std::array<size_t, 3> & buffer_origin,
                        const std::array<size_t, 3> & region,
                        const dType &                 dtype,
                        const float &                 value) -> void
{
#if USE_OPENCL
  auto   opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  cl_int err;
  switch (dtype)
  {
    case dType::FLOAT: {
      float cval[4] = { static_cast<cl_float>(value),
                        static_cast<cl_float>(value),
                        static_cast<cl_float>(value),
                        static_cast<cl_float>(value) };
      err = clEnqueueFillImage(opencl_device->getCLCommandQueue(),
                               *static_cast<cl_mem *>(*buffer_ptr),
                               cval,
                               buffer_origin.data(),
                               region.data(),
                               0,
                               nullptr,
                               nullptr);
      break;
    }
    case dType::INT32:
    case dType::INT16:
    case dType::INT8: {
      cl_int cval[4] = {
        static_cast<cl_int>(value), static_cast<cl_int>(value), static_cast<cl_int>(value), static_cast<cl_int>(value)
      };
      err = clEnqueueFillImage(opencl_device->getCLCommandQueue(),
                               *static_cast<cl_mem *>(*buffer_ptr),
                               cval,
                               buffer_origin.data(),
                               region.data(),
                               0,
                               nullptr,
                               nullptr);
      break;
    }
    case dType::UINT32:
    case dType::UINT16:
    case dType::UINT8: {
      cl_uint cval[4] = { static_cast<cl_uint>(value),
                          static_cast<cl_uint>(value),
                          static_cast<cl_uint>(value),
                          static_cast<cl_uint>(value) };
      err = clEnqueueFillImage(opencl_device->getCLCommandQueue(),
                               *static_cast<cl_mem *>(*buffer_ptr),
                               cval,
                               buffer_origin.data(),
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
    throw std::runtime_error("Error: Fail to fill image memory. OpenCL error : " + getErrorString(err) + " (" +
                             std::to_string(err) + ").");
  }
#else
  throw std::runtime_error("Error: OpenCL is not enabled");
#endif
}

#if USE_OPENCL
static auto
buildProgram(const Device::Pointer & device, const cl_program & program) -> void
{
  auto   opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  cl_int buildStatus = clBuildProgram(program, 1, &opencl_device->getCLDevice(), "-w", nullptr, nullptr);
  if (buildStatus != CL_SUCCESS)
  {
    size_t      len;
    std::string buffer;
    clGetProgramBuildInfo(program, opencl_device->getCLDevice(), CL_PROGRAM_BUILD_LOG, 0, nullptr, &len);
    buffer.resize(len);
    clGetProgramBuildInfo(program, opencl_device->getCLDevice(), CL_PROGRAM_BUILD_LOG, len, &buffer[0], &len);
    throw std::runtime_error("Build log: " + buffer + "\nError: Fail to build program. OpenCL error : " +
                             getErrorString(buildStatus) + " (" + std::to_string(buildStatus) + ").");
  }
}

static auto
saveBinaryToCache(const std::string &     device_hash,
                  const std::string &     source_hash,
                  const cl_program &      program,
                  const Device::Pointer & device) -> void
{
  size_t   device_index = device->getDeviceIndex();
  size_t   nb_devices = device->getNbDevicesFromContext();
  size_t * bin_size_list = new size_t[nb_devices];
  auto err = clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, sizeof(size_t) * nb_devices, bin_size_list, nullptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Fail to fetch program binary size. OpenCL error : " + getErrorString(err) + " (" +
                             std::to_string(err) + ").");
  }

  char ** bin_value_list = new char *[nb_devices];
  for (int i = 0; i < nb_devices; i++)
  {
    bin_value_list[i] = new char[bin_size_list[i]];
  }
  err = clGetProgramInfo(program, CL_PROGRAM_BINARIES, sizeof(unsigned char *) * nb_devices, bin_value_list, nullptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Fail to fetch program binary. OpenCL error : " + getErrorString(err) + " (" +
                             std::to_string(err) + ").");
  }

  std::filesystem::path binary_path =
    CACHE_FOLDER_PATH / std::filesystem::path(device_hash) / std::filesystem::path(source_hash + ".bin");
  std::filesystem::create_directories(binary_path.parent_path());
  std::ofstream outfile(binary_path, std::ios::binary);
  if (!outfile)
  {
    throw std::runtime_error("Error: Fail to open binary cache file.");
  }

  outfile.write(reinterpret_cast<char *>(bin_value_list[device_index]), bin_size_list[device_index]);
  if (!outfile.good())
  {
    throw std::runtime_error("Error: Fail to write binary cache file.");
  }

  // properly deallocate the memory
  delete[] bin_size_list;
  for (int i = 0; i < nb_devices; i++)
  {
    delete[] bin_value_list[i];
  }
  delete[] bin_value_list;
}

static auto
loadProgramFromCache(const Device::Pointer & device, const std::string & device_hash, const std::string & source_hash)
  -> cl_program
{
  cl_int err;
  cl_int status;

  std::filesystem::path binary_path =
    CACHE_FOLDER_PATH / std::filesystem::path(device_hash) / std::filesystem::path(source_hash + ".bin");
  if (!std::filesystem::exists(binary_path))
  {
    return nullptr;
  }
  std::ifstream binary_file(binary_path, std::ios::binary | std::ios::ate);
  if (!binary_file.is_open())
  {
    std::cerr << "Error: Fail to open binary cache file." << std::endl;
    return nullptr;
  }
  size_t binary_size = binary_file.tellg();
  if (binary_size <= 0)
  {
    std::cerr << "Error: Fail to read binary file size. Reading size: " << binary_size << std::endl;
    return nullptr;
  }
  binary_file.seekg(0, std::ios::beg);
  std::vector<unsigned char> binary(binary_size);
  if (!binary_file.read((char *)binary.data(), binary_size))
  {
    std::cerr << "Error: Fail to read binary file." << std::endl;
    return nullptr;
  }

  auto         opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  const auto * binary_code_ptr = reinterpret_cast<const unsigned char *>(binary.data());
  auto         program = clCreateProgramWithBinary(
    opencl_device->getCLContext(), 1, &opencl_device->getCLDevice(), &binary_size, &binary_code_ptr, &status, &err);
  if (status != CL_SUCCESS)
  {
    std::cerr << "Error: Fail to create program from binary. OpenCL error : " + getErrorString(err) + " (" +
                   std::to_string(err) + ")."
              << std::endl;
    return nullptr;
  }
  try
  {
    buildProgram(device, program);
  }
  catch (const std::exception & e)
  {
    return nullptr;
  }
  return program;
}


static auto
CreateProgramFromSource(const Device::Pointer & device, const std::string & kernel_source) -> cl_program
{
  cl_int err;
  auto   opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);

  const char * source = kernel_source.c_str();
  auto         program = clCreateProgramWithSource(opencl_device->getCLContext(), 1, &source, nullptr, &err);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Fail to create program from source. OpenCL error : " + getErrorString(err) + " (" +
                             std::to_string(err) + ").");
  }
  buildProgram(device, program);
  return program;
}
#endif

auto
OpenCLBackend::buildKernel(const Device::Pointer & device,
                           const std::string &     kernel_source,
                           const std::string &     kernel_name,
                           void *                  kernel) const -> void
{
#if USE_OPENCL
  cl_int     err;
  auto       opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  const auto use_cache = is_cache_enabled();

  std::hash<std::string> hasher;
  const auto             source_hash = std::to_string(hasher(kernel_source));
  const auto             device_hash = std::to_string(hasher(opencl_device->getInfo()));

  cl_program program = nullptr;
  if (use_cache)
  {
    program = loadProgramFromCache(device, device_hash, source_hash);
  }

  if (program == nullptr)
  {
    program = CreateProgramFromSource(device, kernel_source);
    if (use_cache)
    {
      saveBinaryToCache(device_hash, source_hash, program, device);
    }
  }

  auto * ocl_kernel = clCreateKernel(program, kernel_name.c_str(), &err);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Fail to create kernel. OpenCL error : " + getErrorString(err) + " (" +
                             std::to_string(err) + ").");
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
  auto opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);

  cl_kernel ocl_kernel;
  buildKernel(device, kernel_source, kernel_name, &ocl_kernel);

  for (size_t i = 0; i < args.size(); i++)
  {
    auto err = clSetKernelArg(ocl_kernel, i, sizes[i], args[i]);
    if (err != CL_SUCCESS)
    {
      throw std::runtime_error("Error: Fail to set kernel argument " + std::to_string(i) +
                               ". OpenCL error : " + getErrorString(err) + " (" + std::to_string(err) + ").");
    }
  }

  auto err = clEnqueueNDRangeKernel(
    opencl_device->getCLCommandQueue(), ocl_kernel, 3, nullptr, global_size.data(), nullptr, 0, nullptr, nullptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Fail to launch kernel. OpenCL error : " + getErrorString(err) + " (" +
                             std::to_string(err) + ").");
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
