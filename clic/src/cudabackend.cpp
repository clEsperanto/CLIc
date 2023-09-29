#include "backend.hpp"
#include "cle_preamble_cu.h"
#include <array>
#include <chrono>

namespace cle
{

CUDABackend::CUDABackend()
{
#if USE_CUDA
  cuInit(0);
#endif
}

auto
CUDABackend::getDevices(const std::string & type) const -> std::vector<Device::Pointer>
{
#if USE_CUDA
  int  deviceCount;
  auto error = cuDeviceGetCount(&deviceCount);
  if (error != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to get CUDA device count (" + std::to_string(error) + ").");
  }
  std::vector<Device::Pointer> devices;
  for (int i = 0; i < deviceCount; i++)
  {
    devices.push_back(std::make_shared<CUDADevice>(i));
  }
  return devices;
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::getDevice(const std::string & name, const std::string & type) const -> Device::Pointer
{
#if USE_CUDA
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
    std::cerr << "WARNING: Device with name '" << name << "' not found. Using default device." << std::endl;
    return std::move(devices.back());
  }
  return nullptr;
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::getDevicesList(const std::string & type) const -> std::vector<std::string>
{
#if USE_CUDA
  auto                     devices = getDevices(type);
  std::vector<std::string> deviceList;
  for (size_t i = 0; i < devices.size(); i++)
  {
    deviceList.emplace_back(devices[i]->getName());
  }
  return deviceList;
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::getType() const -> Backend::Type
{
  return Backend::Type::CUDA;
}

auto
CUDABackend::allocateMemory(const Device::Pointer &       device,
                            const std::array<size_t, 3> & region,
                            const dType &                 dtype,
                            const mType &                 mtype,
                            void **                       data_ptr) const -> void
{
#if USE_CUDA
  switch (mtype)
  {
    case mType::BUFFER: {
      const size_t size = region[0] * region[1] * region[2] * toBytes(dtype);
      allocateBuffer(device, size, data_ptr);
      break;
    }
    case mType::IMAGE: {
      // TODO @StRigaud: implement image support for CUDA
      // allocateImage(device, region, dtype, data_ptr);
      const size_t size = region[0] * region[1] * region[2] * toBytes(dtype);
      allocateBuffer(device, size, data_ptr);
      break;
    }
  }
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::allocateBuffer(const Device::Pointer & device, const size_t & size, void ** data_ptr) -> void
{
#if USE_CUDA
  auto cuda_device = std::dynamic_pointer_cast<const CUDADevice>(device);
  auto err = cuCtxSetCurrent(cuda_device->getCUDAContext());
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to get context from device (" + std::to_string(err) + ").");
  }
  CUdeviceptr mem;
  err = cuMemAlloc(&mem, size);
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to allocate memory (buffer) with error code " + std::to_string(err));
  }
  *data_ptr = reinterpret_cast<void *>(mem);
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

/*
// TODO @StRigaud: implement image support for CUDA
auto
CUDABackend::allocateImage(const Device::Pointer &       device,
                           const std::array<size_t, 3> & region,
                           const dType &                 dtype,
                           void **                       data_ptr) -> void
{
#if USE_CUDA
  auto cuda_device = std::dynamic_pointer_cast<const CUDADevice>(device);
  auto err = cuCtxSetCurrent(cuda_device->getCUDAContext());
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to get context from device (" + std::to_string(err) + ").");
  }
  CUarray        array;
  CUarray_format format;
  switch (dtype)
  {
    case dType::FLOAT:
      format = CU_AD_FORMAT_FLOAT;
      break;
    case dType::INT8:
      format = CU_AD_FORMAT_SIGNED_INT8;
      break;
    case dType::UINT8:
      format = CU_AD_FORMAT_UNSIGNED_INT8;
      break;
    case dType::INT16:
      format = CU_AD_FORMAT_SIGNED_INT16;
      break;
    case dType::UINT16:
      format = CU_AD_FORMAT_UNSIGNED_INT16;
      break;
    case dType::INT32:
      format = CU_AD_FORMAT_SIGNED_INT32;
      break;
    case dType::UINT32:
      format = CU_AD_FORMAT_UNSIGNED_INT32;
      break;
    default:
      format = CU_AD_FORMAT_FLOAT;
      std::cerr << "Warning: Unsupported data type for 'image', default type 'float' will be used." << std::endl;
      break;
  }
  if (depth > 1)
  {
    CUDA_ARRAY3D_DESCRIPTOR desc;
    desc.Width = region[0];
    desc.Height = region[1];
    desc.Depth = region[2];
    desc.Format = format;
    desc.NumChannels = 1;
    desc.Flags = 0;
    err = cuArray3DCreate(&array, &desc);
  }
  else
  {
    CUDA_ARRAY_DESCRIPTOR desc;
    desc.Width = region[0];
    desc.Height = region[1];
    desc.Format = format;
    desc.NumChannels = 1;
    err = cuArrayCreate(&array, &desc);
  }
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to allocate memory (image) with error code " + std::to_string(err));
  }
  *data_ptr = reinterpret_cast<void *>(array);
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}
*/

auto
CUDABackend::freeMemory(const Device::Pointer & device, const mType & mtype, void ** data_ptr) const -> void
{
#if USE_CUDA
  auto cuda_device = std::dynamic_pointer_cast<const CUDADevice>(device);
  auto err = cuCtxSetCurrent(cuda_device->getCUDAContext());
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to get context from device (" + std::to_string(err) + ").");
  }
  if (mtype == mType::IMAGE)
  {
    // TODO @StRigaud: implement image support for CUDA
    // err = cuArrayDestroy(reinterpret_cast<CUarray>(*data_ptr));
    err = cuMemFree(reinterpret_cast<CUdeviceptr>(*data_ptr));
  }
  else
  {
    err = cuMemFree(reinterpret_cast<CUdeviceptr>(*data_ptr));
  }
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to free memory with error code " + std::to_string(err) + ".");
  }
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::writeMemory(const Device::Pointer & device,
                         void **                 buffer_ptr,
                         std::array<size_t, 3> & buffer_shape,
                         std::array<size_t, 3> & buffer_origin,
                         std::array<size_t, 3> & region,
                         const dType &           dtype,
                         const mType &           mtype,
                         const void *            host_ptr) const -> void
{
#if USE_CUDA
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
      // TODO @StRigaud: implement image support for CUDA
      writeBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
      break;
    }
  }
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::writeBuffer(const Device::Pointer &       device,
                         void **                       buffer_ptr,
                         const std::array<size_t, 3> & buffer_shape,
                         const std::array<size_t, 3> & buffer_origin,
                         const std::array<size_t, 3> & region,
                         const void *                  host_ptr) -> void
{
#if USE_CUDA
  auto cuda_device = std::dynamic_pointer_cast<const CUDADevice>(device);
  auto err = cuCtxSetCurrent(cuda_device->getCUDAContext());
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to get context from device (" + std::to_string(err) + ").");
  }

  size_t buffer_row_pitch = buffer_shape[1] > 1 ? buffer_shape[0] : 0;
  size_t buffer_slice_pitch = buffer_shape[2] > 1 ? buffer_shape[1] : 0;

  const std::array<size_t, 3> host_origin = { 0, 0, 0 };
  if (buffer_shape[2] > 1)
  {
    CUDA_MEMCPY3D copyParams = { 0 };

    copyParams.srcMemoryType = CU_MEMORYTYPE_HOST; // Source memory type.
    copyParams.srcHost = host_ptr;

    copyParams.dstMemoryType = CU_MEMORYTYPE_DEVICE; // Destination memory type.
    copyParams.dstDevice = reinterpret_cast<CUdeviceptr>(*buffer_ptr);
    copyParams.dstPitch = buffer_row_pitch;
    copyParams.dstHeight = buffer_slice_pitch;
    copyParams.dstXInBytes = buffer_origin[0];
    copyParams.dstY = buffer_origin[1];
    copyParams.dstZ = buffer_origin[2];

    copyParams.WidthInBytes = region[0];
    copyParams.Height = region[1];
    copyParams.Depth = region[2];
    err = cuMemcpy3D(&copyParams);
  }
  else if (buffer_shape[1] > 1)
  {
    CUDA_MEMCPY2D copyParams = { 0 };

    copyParams.srcMemoryType = CU_MEMORYTYPE_HOST; // Source memory type.
    copyParams.srcHost = host_ptr;

    copyParams.dstMemoryType = CU_MEMORYTYPE_DEVICE; // Destination memory type.
    copyParams.dstDevice = reinterpret_cast<CUdeviceptr>(*buffer_ptr);
    copyParams.dstPitch = buffer_row_pitch;
    copyParams.dstXInBytes = buffer_origin[0];
    copyParams.dstY = buffer_origin[1];

    copyParams.WidthInBytes = region[0];
    copyParams.Height = region[1];
    err = cuMemcpy2D(&copyParams);
  }
  else
  {
    void * cast_host_ptr = const_cast<void *>(host_ptr);
    cast_host_ptr = static_cast<char *>(cast_host_ptr) + (host_origin[0]);
    CUdeviceptr devicePtr = reinterpret_cast<CUdeviceptr>(reinterpret_cast<char *>(*buffer_ptr) + buffer_origin[0]);
    err = cuMemcpy(reinterpret_cast<CUdeviceptr>(*buffer_ptr), reinterpret_cast<CUdeviceptr>(cast_host_ptr), region[0]);
  }
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to write on device (host -> buffer) with error code " +
                             std::to_string(err));
  }
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::readBuffer(const Device::Pointer &       device,
                        const void **                 buffer_ptr,
                        const std::array<size_t, 3> & buffer_shape,
                        const std::array<size_t, 3> & buffer_origin,
                        const std::array<size_t, 3> & region,
                        void *                        host_ptr) -> void
{
#if USE_CUDA
  auto cuda_device = std::dynamic_pointer_cast<const CUDADevice>(device);
  auto err = cuCtxSetCurrent(cuda_device->getCUDAContext());
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to get context from device (" + std::to_string(err) + ").");
  }
  size_t buffer_row_pitch = buffer_shape[1] > 1 ? buffer_shape[0] : 0;
  size_t buffer_slice_pitch = buffer_shape[2] > 1 ? buffer_shape[1] : 0;

  const std::array<size_t, 3> host_origin = { 0, 0, 0 };
  if (buffer_shape[2] > 1)
  {
    CUDA_MEMCPY3D copyParams = { 0 };

    copyParams.dstMemoryType = CU_MEMORYTYPE_HOST; // Source memory type.
    copyParams.dstHost = host_ptr;

    copyParams.srcMemoryType = CU_MEMORYTYPE_DEVICE; // Destination memory type.
    copyParams.srcDevice = reinterpret_cast<CUdeviceptr>(*buffer_ptr);
    copyParams.srcPitch = buffer_row_pitch;
    copyParams.srcHeight = buffer_slice_pitch;
    copyParams.srcXInBytes = buffer_origin[0];
    copyParams.srcY = buffer_origin[1];
    copyParams.srcZ = buffer_origin[2];

    copyParams.WidthInBytes = region[0];
    copyParams.Height = region[1];
    copyParams.Depth = region[2];
    err = cuMemcpy3D(&copyParams);
  }
  else if (buffer_shape[1] > 1)
  {
    CUDA_MEMCPY2D copyParams = { 0 };

    copyParams.dstMemoryType = CU_MEMORYTYPE_HOST; // Source memory type.
    copyParams.dstHost = host_ptr;

    copyParams.srcMemoryType = CU_MEMORYTYPE_DEVICE; // Destination memory type.
    copyParams.srcDevice = reinterpret_cast<CUdeviceptr>(*buffer_ptr);
    copyParams.srcPitch = buffer_row_pitch;
    copyParams.srcXInBytes = buffer_origin[0];
    copyParams.srcY = buffer_origin[1];

    copyParams.WidthInBytes = region[0];
    copyParams.Height = region[1];
    err = cuMemcpy2D(&copyParams);
  }
  else
  {
    void * cast_host_ptr = const_cast<void *>(host_ptr);
    cast_host_ptr = static_cast<char *>(cast_host_ptr) + (host_origin[0]);
    CUdeviceptr devicePtr =
      reinterpret_cast<CUdeviceptr>(reinterpret_cast<const char *>(*buffer_ptr) + buffer_origin[0]);
    err = cuMemcpy((CUdeviceptr)cast_host_ptr, devicePtr, region[0]);
  }
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to read memory (buffer -> host) with error code " +
                             std::to_string(err));
  }
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::readMemory(const Device::Pointer & device,
                        const void **           buffer_ptr,
                        std::array<size_t, 3> & buffer_shape,
                        std::array<size_t, 3> & buffer_origin,
                        std::array<size_t, 3> & region,
                        const dType &           dtype,
                        const mType &           mtype,
                        void *                  host_ptr) const -> void
{
#if USE_CUDA
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
      // TODO @StRigaud: implement image support for CUDA
      readBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
      break;
    }
  }
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::copyMemoryBufferToBuffer(const Device::Pointer &       device,
                                      const void **                 src_data_ptr,
                                      const std::array<size_t, 3> & region,
                                      const std::array<size_t, 3> & origin,
                                      const size_t &                bytes,
                                      void **                       dst_data_ptr) const -> void
{
#if USE_CUDA
  auto cuda_device = std::dynamic_pointer_cast<const CUDADevice>(device);
  auto err = cuCtxSetCurrent(cuda_device->getCUDAContext());
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to get context from device (" + std::to_string(err) + ").");
  }
  if (region[2] > 1)
  {
    CUDA_MEMCPY3D copyParams = { 0 };

    copyParams.dstMemoryType = CU_MEMORYTYPE_DEVICE; // Source memory type.
    copyParams.dstDevice = reinterpret_cast<CUdeviceptr>(*dst_data_ptr);
    copyParams.dstXInBytes = origin[0] * bytes;
    copyParams.dstY = origin[1];
    copyParams.dstZ = origin[2];

    copyParams.srcMemoryType = CU_MEMORYTYPE_DEVICE; // Destination memory type.
    copyParams.srcDevice = reinterpret_cast<CUdeviceptr>(*src_data_ptr);
    copyParams.srcXInBytes = origin[0] * bytes;
    copyParams.srcY = origin[1];
    copyParams.srcZ = origin[2];

    copyParams.WidthInBytes = region[0] * bytes;
    copyParams.Height = region[1];
    copyParams.Depth = region[2];
    err = cuMemcpy3D(&copyParams);
  }
  else if (region[1] > 1)
  {
    CUDA_MEMCPY2D copyParams = { 0 };

    copyParams.dstMemoryType = CU_MEMORYTYPE_DEVICE; // Source memory type.
    copyParams.dstDevice = reinterpret_cast<CUdeviceptr>(*dst_data_ptr);
    copyParams.dstXInBytes = origin[0] * bytes;
    copyParams.dstY = origin[1];

    copyParams.srcMemoryType = CU_MEMORYTYPE_DEVICE; // Destination memory type.
    copyParams.srcDevice = reinterpret_cast<CUdeviceptr>(*src_data_ptr);
    copyParams.srcXInBytes = origin[0] * bytes;
    copyParams.srcY = origin[1];

    copyParams.WidthInBytes = region[0] * bytes;
    copyParams.Height = region[1];
    err = cuMemcpy2D(&copyParams);
  }
  else
  {
    auto        dst_ptr = reinterpret_cast<CUdeviceptr>(*dst_data_ptr) + (origin[0] * bytes);
    CUdeviceptr src_ptr = reinterpret_cast<CUdeviceptr>(*src_data_ptr) + (origin[0] * bytes);
    err = cuMemcpy((CUdeviceptr)dst_ptr, src_ptr, region[0] * bytes);
  }
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to copy device memory (buffer -> buffer)  with error code " +
                             std::to_string(err));
  }
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::copyMemoryImageToBuffer(const Device::Pointer &       device,
                                     const void **                 src_data_ptr,
                                     const std::array<size_t, 3> & region,
                                     const std::array<size_t, 3> & origin,
                                     const size_t &                bytes,
                                     void **                       dst_data_ptr) const -> void
{
#if USE_CUDA
  copyMemoryBufferToBuffer(device, src_data_ptr, region, origin, bytes, dst_data_ptr);
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::copyMemoryBufferToImage(const Device::Pointer &       device,
                                     const void **                 src_data_ptr,
                                     const std::array<size_t, 3> & region,
                                     const std::array<size_t, 3> & origin,
                                     const size_t &                bytes,
                                     void **                       dst_data_ptr) const -> void
{
#if USE_CUDA
  copyMemoryBufferToBuffer(device, src_data_ptr, region, origin, bytes, dst_data_ptr);
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::copyMemoryImageToImage(const Device::Pointer &       device,
                                    const void **                 src_data_ptr,
                                    const std::array<size_t, 3> & region,
                                    const std::array<size_t, 3> & origin,
                                    const size_t &                bytes,
                                    void **                       dst_data_ptr) const -> void
{
#if USE_CUDA
  copyMemoryBufferToBuffer(device, src_data_ptr, region, origin, bytes, dst_data_ptr);
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::setBuffer(const Device::Pointer &       device,
                       void **                       buffer_ptr,
                       const std::array<size_t, 3> & buffer_shape,
                       const std::array<size_t, 3> & buffer_origin,
                       const std::array<size_t, 3> & region,
                       const dType &                 dtype,
                       const float &                 value) -> void
{
#if USE_CUDA
  auto cuda_device = std::dynamic_pointer_cast<const CUDADevice>(device);
  auto err = cuCtxSetCurrent(cuda_device->getCUDAContext());
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to get context from device (" + std::to_string(err) + ").");
  }
  const auto count = region[0] * region[1] * region[2];
  const auto dev_ptr = reinterpret_cast<CUdeviceptr>(*buffer_ptr);
  switch (dtype)
  {
    case dType::FLOAT: {
      auto cval = static_cast<float>(value);
      err = cuMemsetD32(dev_ptr, *(reinterpret_cast<uint32_t *>(&cval)), count);
      break;
    }
    // case dType::INT64: {
    //   std::vector<int64_t> host_buffer(count, static_cast<int64_t>(value));
    //   writeBuffer(device, buffer_ptr, region, origin, dType::INT64, host_buffer.data());
    //   break;
    // }
    // case dType::UINT64: {
    //   std::vector<uint64_t> host_buffer(count, static_cast<uint64_t>(value));
    //   writeBuffer(device, buffer_ptr, region, origin, dType::UINT64, host_buffer.data());
    //   break;
    // }
    case dType::INT32: {
      auto cval = static_cast<int32_t>(value);
      err = cuMemsetD32(dev_ptr, *(reinterpret_cast<uint32_t *>(&cval)), count);
      break;
    }
    case dType::UINT32: {
      auto cval = static_cast<uint32_t>(value);
      err = cuMemsetD32(dev_ptr, *(reinterpret_cast<uint32_t *>(&cval)), count);
      break;
    }
    case dType::INT16: {
      auto cval = static_cast<int16_t>(value);
      err = cuMemsetD16(dev_ptr, *(reinterpret_cast<uint16_t *>(&cval)), count);
      break;
    }
    case dType::UINT16: {
      auto cval = static_cast<uint16_t>(value);
      err = cuMemsetD16(dev_ptr, *(reinterpret_cast<uint16_t *>(&cval)), count);
      break;
    }
    case dType::INT8: {
      auto cval = static_cast<int8_t>(value);
      err = cuMemsetD8(dev_ptr, *(reinterpret_cast<uint8_t *>(&cval)), count);
      break;
    }
    case dType::UINT8: {
      auto cval = static_cast<uint8_t>(value);
      err = cuMemsetD8(dev_ptr, *(reinterpret_cast<uint8_t *>(&cval)), count);
      break;
    }
    default:
      std::cerr << "Warning: Unsupported value size for cuda setMemory" << std::endl;
      break;
  }
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to set memory with error code " + std::to_string(err));
  }
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::setMemory(const Device::Pointer & device,
                       void **                 buffer_ptr,
                       std::array<size_t, 3> & buffer_shape,
                       std::array<size_t, 3> & buffer_origin,
                       std::array<size_t, 3> & region,
                       const dType &           dtype,
                       const mType &           mtype,
                       const float &           value) const -> void
{
#if USE_CUDA
  switch (mtype)
  {
    case mType::BUFFER: {
      setBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, dtype, value);
      break;
    }
    case mType::IMAGE: {
      setBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, dtype, value);
      break;
    }
  }
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::loadProgramFromCache(const Device::Pointer & device, const std::string & hash, void * program) const
  -> void
{
#if USE_CUDA
  if (auto cuda_device = std::dynamic_pointer_cast<CUDADevice>(device))
  {
    const auto & cache = cuda_device->getCache();
    auto         ite = cache.find(hash);
    if (ite != cache.end())
    {
      *static_cast<CUmodule *>(program) = ite->second;
    }
  }
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::saveProgramToCache(const Device::Pointer & device, const std::string & hash, void * program) const -> void
{
#if USE_CUDA
  if (auto cuda_device = std::dynamic_pointer_cast<CUDADevice>(device))
  {
    cuda_device->getCache().emplace(hash, *reinterpret_cast<CUmodule *>(program));
  }
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}


auto
CUDABackend::buildKernel(const Device::Pointer & device,
                         const std::string &     kernel_source,
                         const std::string &     kernel_name,
                         void *                  kernel) const -> void
{
#if USE_CUDA
  auto cuda_device = std::dynamic_pointer_cast<const CUDADevice>(device);
  auto err = cuCtxSetCurrent(cuda_device->getCUDAContext());
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to set CUDA device before memory allocation.");
  }

  std::chrono::high_resolution_clock::time_point start_time, end_time;
  std::chrono::microseconds                      duration;

  CUmodule    cuModule = nullptr;
  std::string hash = std::to_string(std::hash<std::string>{}(kernel_source));
  loadProgramFromCache(device, hash, &cuModule);
  if (cuModule == nullptr)
  {
    nvrtcProgram prog;
    auto         res = nvrtcCreateProgram(&prog, kernel_source.c_str(), nullptr, 0, nullptr, nullptr);
    if (res != NVRTC_SUCCESS)
    {
      throw std::runtime_error("Error (cuda): Failed to create program from source with error code " +
                               std::to_string(res));
    }

    const std::string                 arch_comp = "-arch=compute_" + cuda_device->getArch();
    const std::array<const char *, 1> options = { arch_comp.c_str() };
    res = nvrtcCompileProgram(prog, options.size(), options.data());
    if (res != NVRTC_SUCCESS)
    {
      size_t log_size;
      nvrtcGetProgramLogSize(prog, &log_size);
      std::string log(log_size, '\0');
      nvrtcGetProgramLog(prog, &log[0]);
      std::cerr << "Build log: " << log << std::endl;
      throw std::runtime_error("Error (cuda): Failed to build program with error code " + std::to_string(res));
    }
    size_t ptxSize;
    nvrtcGetPTXSize(prog, &ptxSize);
    std::vector<char> ptx(ptxSize);
    nvrtcGetPTX(prog, ptx.data());
    res = nvrtcDestroyProgram(&prog);
    if (res != NVRTC_SUCCESS)
    {
      throw std::runtime_error("Error (cuda): Failed to destroy program with error code " + std::to_string(res));
    }

    err = cuModuleLoadData(&cuModule, ptx.data());
    if (err != CUDA_SUCCESS)
    {
      throw std::runtime_error("Error (cuda): Loading module with error code " + std::to_string(err));
    }


    saveProgramToCache(device, hash, &cuModule);
  }
  CUfunction cuFunction;
  err = cuModuleGetFunction(&cuFunction, cuModule, kernel_name.c_str());
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Getting function from module with error code " + std::to_string(err));
  }
  *(reinterpret_cast<CUfunction *>(kernel)) = cuFunction;
#else
  throw std::runtime_error("Error: CUDA is not enabled");
#endif
}

auto
CUDABackend::executeKernel(const Device::Pointer &       device,
                           const std::string &           kernel_source,
                           const std::string &           kernel_name,
                           const std::array<size_t, 3> & global_size,
                           const std::vector<void *> &   args,
                           const std::vector<size_t> &   sizes) const -> void
{
#if USE_CUDA
  auto cuda_device = std::dynamic_pointer_cast<const CUDADevice>(device);
  auto err = cuCtxSetCurrent(cuda_device->getCUDAContext());
  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed to set CUDA device before memory allocation.");
  }

  CUfunction cuFunction;
  try
  {
    buildKernel(device, kernel_source, kernel_name, &cuFunction);
  }
  catch (const std::exception & e)
  {
    throw std::runtime_error("Error (cuda): Failed to build kernel. \n\t > " + std::string(e.what()));
  }

  std::vector<void *> argsValues(args.size());
  argsValues = args;

  std::array<size_t, 3> block_size = { 0, 0, 0 };
  int                   dim = 0;
  for (size_t i = 0; i < global_size.size(); ++i)
  {
    if (global_size[i] > 1)
    {
      dim++;
      block_size[i] = 1;
    }
  }

  switch (dim)
  {
    case 1:
      // Warning: Ensure that the third dimension of the block size does not exceed 64.
      std::transform(block_size.begin(), block_size.end(), block_size.begin(), [](size_t value) {
        return (value == 0) ? (1) : (value * 512);
      });
      break;
    case 2:
      std::transform(block_size.begin(), block_size.end(), block_size.begin(), [](size_t value) {
        return (value == 0) ? (1) : (value * 16);
      });
      break;
    case 3:
      std::transform(
        block_size.begin(), block_size.end(), block_size.begin(), [](size_t value) { return (value * 8); });
      break;
    default:
      block_size = { 1, 1, 1 };
  }


  std::array<size_t, 3> grid_size = { (global_size.data()[0] + block_size.data()[0] - 1) / block_size.data()[0],
                                      (global_size.data()[1] + block_size.data()[1] - 1) / block_size.data()[1],
                                      (global_size.data()[2] + block_size.data()[2] - 1) / block_size.data()[2] };

  err = cuLaunchKernel(cuFunction,
                       grid_size.data()[0],
                       grid_size.data()[1],
                       grid_size.data()[2],
                       block_size.data()[0],
                       block_size.data()[1],
                       block_size.data()[2],
                       0,
                       cuda_device->getCUDAStream(),
                       argsValues.data(),
                       nullptr);

  if (err != CUDA_SUCCESS)
  {
    throw std::runtime_error("Error (cuda): Failed launching kernel with error code " + std::to_string(err));
  }
  cuda_device->finish();
#else
  throw std::runtime_error("Error: CUDA backend is not enabled");
#endif
}

auto
CUDABackend::getPreamble() const -> std::string
{
  return kernel::preamble_cu;
}

} // namespace cle
