#include "array.hpp"

#include <array>

namespace cle
{

auto
Array::New() -> Array::Pointer
{
  return std::shared_ptr<Array>(new Array());
}


Array::Array(const size_t            width,
             const size_t            height,
             const size_t            depth,
             const size_t            dimension,
             const dType &           data_type,
             const mType &           mem_type,
             const Device::Pointer & device_ptr)
  : width_((width > 1) ? width : 1)
  , height_((height > 1) ? height : 1)
  , depth_((depth > 1) ? depth : 1)
  , dim_(dimension)
  , dataType_(data_type)
  , memType_(mem_type)
  , device_(device_ptr)
  , data_(nullptr)
{}

Array::Array(const size_t                  width,
             const size_t                  height,
             const size_t                  depth,
             const size_t                  dimension,
             const dType &                 data_type,
             const mType &                 mem_type,
             const std::shared_ptr<void> & gpu_data,
             const Device::Pointer &       device_ptr)
  : width_((width > 1) ? width : 1)
  , height_((height > 1) ? height : 1)
  , depth_((depth > 1) ? depth : 1)
  , dim_(dimension)
  , dataType_(data_type)
  , memType_(mem_type)
  , device_(device_ptr)
  , data_(gpu_data)
  , initialized_(true)
{}

auto
Array::create(const size_t            width,
              const size_t            height,
              const size_t            depth,
              const size_t            dimension,
              const dType &           data_type,
              const mType &           mem_type,
              const Device::Pointer & device_ptr) -> Array::Pointer
{
  auto ptr = std::shared_ptr<Array>(new Array(width, height, depth, dimension, data_type, mem_type, device_ptr));
  ptr->allocate();
  return ptr;
}

auto
Array::create(const size_t            width,
              const size_t            height,
              const size_t            depth,
              const size_t            dimension,
              const dType &           data_type,
              const mType &           mem_type,
              const void *            host_data,
              const Device::Pointer & device_ptr) -> Array::Pointer
{
  auto ptr = create(width, height, depth, dimension, data_type, mem_type, device_ptr);
  ptr->writeFrom(host_data);
  return ptr;
}

auto
Array::create(const Array::Pointer & array) -> Array::Pointer
{
  auto ptr = create(array->width(), array->height(), array->depth(), array->dimension(), array->dtype(), array->mtype(), array->device());
  return ptr;
}

auto
Array::reshape(size_t new_width, size_t new_height, size_t new_depth, size_t new_dimension) const -> Array::Pointer
{
  if (new_dimension == 0)
  {
    new_dimension = (this->depth() > 1) ? 3 : (this->height() > 1) ? 2 : 1;
  }
  if (new_width * new_height * new_depth != this->size())
  {
    throw std::invalid_argument("Error: Array reshape size mismatch. Original size: " + std::to_string(this->size()) +
                                ", new size: " + std::to_string(new_width * new_height * new_depth));
  }
  auto ptr = std::shared_ptr<Array>(
    new Array(new_width, new_height, new_depth, new_dimension, this->dtype(), this->mtype(), this->get_ptr(), this->device()));
  return ptr;
}

auto
Array::shallow_copy() const -> Array::Pointer
{
  auto ptr = std::shared_ptr<Array>(new Array(width_, height_, depth_, dim_, dataType_, memType_, data_, device_));
  return ptr;
}

auto
operator<<(std::ostream & out, const Array::Pointer & array) -> std::ostream &
{
  if (array == nullptr)
  {
    out << "Null Array";
    return out;
  }
  out << array->dimension() << "dArray ([" << array->width() << "," << array->height() << "," << array->depth()
      << "], dtype=" << array->dtype() << ", mtype=" << array->mtype() << ")";
  return out;
}

auto
Array::allocate() -> void
{
  if (initialized())
  {
    return;
  }
  BackendManager::getInstance().getBackend().allocateMemory(
    device(), { this->width(), this->height(), this->depth() }, dtype(), mtype(), data_);
  initialized_ = true;
}

auto
Array::writeFrom(const void * host_data) -> void
{
  if (host_data == nullptr)
  {
    throw std::runtime_error("Error: Host data is null");
  }
  std::array<size_t, 3> _origin = { 0, 0, 0 };
  std::array<size_t, 3> _shape = { this->width(), this->height(), this->depth() };
  std::array<size_t, 3> _region = { this->width(), this->height(), this->depth() };
  BackendManager::getInstance().getBackend().writeMemory(device(), data_, _shape, _origin, _region, dtype(), mtype(), host_data);
}

auto
Array::writeFrom(const void * host_data, const std::array<size_t, 3> & region, const std::array<size_t, 3> & buffer_origin) -> void
{
  if (host_data == nullptr)
  {
    throw std::runtime_error("Error: Host data is null");
  }
  std::array<size_t, 3> _origin = buffer_origin;
  std::array<size_t, 3> _region = region;
  std::array<size_t, 3> _shape = { this->width(), this->height(), this->depth() };
  BackendManager::getInstance().getBackend().writeMemory(device(), data_, _shape, _origin, _region, dtype(), mtype(), host_data);
}

auto
Array::writeFrom(const void * host_data, const size_t x_coord, const size_t y_coord, const size_t z_coord) -> void
{
  writeFrom(host_data, { 1, 1, 1 }, { x_coord, y_coord, z_coord });
}

auto
Array::readTo(void * host_data) const -> void
{
  if (host_data == nullptr)
  {
    throw std::runtime_error("Error: Host data is null");
  }
  std::array<size_t, 3> _origin = { 0, 0, 0 };
  std::array<size_t, 3> _shape = { this->width(), this->height(), this->depth() };
  std::array<size_t, 3> _region = { this->width(), this->height(), this->depth() };
  BackendManager::getInstance().getBackend().readMemory(device(), data_, _shape, _origin, _region, dtype(), mtype(), host_data);
}

auto
Array::readTo(void * host_data, const std::array<size_t, 3> & region, const std::array<size_t, 3> & buffer_origin) const -> void
{
  if (host_data == nullptr)
  {
    throw std::runtime_error("Error: Host data is null");
  }
  std::array<size_t, 3> _origin = buffer_origin;
  std::array<size_t, 3> _region = region;
  std::array<size_t, 3> _shape = { this->width(), this->height(), this->depth() };
  BackendManager::getInstance().getBackend().readMemory(device(), data_, _shape, _origin, _region, dtype(), mtype(), host_data);
}

auto
Array::readTo(void * host_data, const size_t x_coord, const size_t y_coord, const size_t z_coord) const -> void
{
  readTo(host_data, { 1, 1, 1 }, { x_coord, y_coord, z_coord });
}

auto
Array::copyTo(const Array::Pointer & dst) const -> void
{
  check_ptr(dst, "Error: Destination Array is null");
  if (device() != dst->device())
  {
    throw std::runtime_error("Error: Copying Arrays from different devices");
  }
  if (size() != dst->size())
  {
    throw std::runtime_error("Error: Arrays sizes do not match");
  }

  std::array<size_t, 3> _src_origin = { 0, 0, 0 };
  std::array<size_t, 3> _dst_origin = { 0, 0, 0 };
  std::array<size_t, 3> _region = { this->width(), this->height(), this->depth() };
  std::array<size_t, 3> _src_shape = { this->width(), this->height(), this->depth() };
  std::array<size_t, 3> _dst_shape = { dst->width(), dst->height(), dst->depth() };

  auto dst_ptr = dst->get_ptr();

  if (mtype() == mType::BUFFER && dst->mtype() == mType::BUFFER)
  {
    _region = { this->size(), 1, 1 };
    _src_shape = { this->size(), 1, 1 };
    _dst_shape = { dst->size(), 1, 1 };
    BackendManager::getInstance().getBackend().copyMemoryBufferToBuffer(
      device(), data_, _src_origin, _src_shape, dst_ptr, _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
  else if (mtype() == mType::IMAGE && dst->mtype() == mType::IMAGE)
  {
    if (this->width() != dst->width() || this->height() != dst->height() || this->depth() != dst->depth())
    {
      throw std::runtime_error("Error: Copying Images of different dimensions");
    }
    BackendManager::getInstance().getBackend().copyMemoryImageToImage(
      device(), data_, _src_origin, _src_shape, dst_ptr, _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
  else if (mtype() == mType::BUFFER && dst->mtype() == mType::IMAGE)
  {
    _src_shape = { this->size(), 1, 1 };
    BackendManager::getInstance().getBackend().copyMemoryBufferToImage(
      device(), data_, _src_origin, _src_shape, dst_ptr, _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
  else if (mtype() == mType::IMAGE && dst->mtype() == mType::BUFFER)
  {
    _dst_shape = { dst->size(), 1, 1 };
    BackendManager::getInstance().getBackend().copyMemoryImageToBuffer(
      device(), data_, _src_origin, _src_shape, dst_ptr, _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
  else
  {
    throw std::runtime_error("Error: Copying Arrays from different memory types");
  }
}

auto
Array::copyTo(const Array::Pointer &        dst,
              const std::array<size_t, 3> & region,
              const std::array<size_t, 3> & src_origin,
              const std::array<size_t, 3> & dst_origin) const -> void
{
  check_ptr(dst, "Error: Destination Array is null");
  if (device() != dst->device())
  {
    throw std::runtime_error("Error: Copying Arrays from different devices");
  }

  std::array<size_t, 3> _src_origin = src_origin;
  std::array<size_t, 3> _dst_origin = dst_origin;
  std::array<size_t, 3> _region = region;
  std::array<size_t, 3> _src_shape = { this->width(), this->height(), this->depth() };
  std::array<size_t, 3> _dst_shape = { dst->width(), dst->height(), dst->depth() };

  auto dst_ptr = dst->get_ptr();

  if (mtype() == mType::BUFFER && dst->mtype() == mType::BUFFER)
  {
    BackendManager::getInstance().getBackend().copyMemoryBufferToBuffer(
      device(), data_, _src_origin, _src_shape, dst_ptr, _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
  else if (mtype() == mType::IMAGE && dst->mtype() == mType::IMAGE)
  {
    BackendManager::getInstance().getBackend().copyMemoryImageToImage(
      device(), data_, _src_origin, _src_shape, dst_ptr, _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
  else if (mtype() == mType::BUFFER && dst->mtype() == mType::IMAGE)
  {
    BackendManager::getInstance().getBackend().copyMemoryBufferToImage(
      device(), data_, _src_origin, _src_shape, dst_ptr, _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
  else if (mtype() == mType::IMAGE && dst->mtype() == mType::BUFFER)
  {
    BackendManager::getInstance().getBackend().copyMemoryImageToBuffer(
      device(), data_, _src_origin, _src_shape, dst_ptr, _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
}

auto
Array::fill(const float value) -> void
{
#if defined(__APPLE__)
  // clEnqueueFillBuffer not behaving as expected on Apple Silicon
  // FIX: Filling buffer with host data
  // TODO: Find a better solution
  auto data_type = dtype();
  switch (data_type)
  {
    case dType::FLOAT: {
      std::vector<float> data(this->size(), value);
      writeFrom(data.data());
      return;
    }
    case dType::INT8: {
      std::vector<int8_t> data(this->size(), static_cast<int8_t>(value));
      writeFrom(data.data());
      return;
    }
    case dType::INT16: {
      std::vector<int16_t> data(this->size(), static_cast<int16_t>(value));
      writeFrom(data.data());
      return;
    }
    case dType::INT32: {
      std::vector<int32_t> data(this->size(), static_cast<int32_t>(value));
      writeFrom(data.data());
      return;
    }
    case dType::UINT8: {
      std::vector<uint8_t> data(this->size(), static_cast<uint8_t>(value));
      writeFrom(data.data());
      return;
    }
    case dType::UINT16: {
      std::vector<uint16_t> data(this->size(), static_cast<uint16_t>(value));
      writeFrom(data.data());
      return;
    }
    case dType::UINT32: {
      std::vector<uint32_t> data(this->size(), static_cast<uint32_t>(value));
      writeFrom(data.data());
      return;
    }
    default: {
      throw std::runtime_error("Error: Unsupported data type");
    }
  }
#else
  std::array<size_t, 3> _origin = { 0, 0, 0 };
  std::array<size_t, 3> _region = { this->width(), this->height(), this->depth() };
  std::array<size_t, 3> _shape = { this->width(), this->height(), this->depth() };
  BackendManager::getInstance().getBackend().setMemory(device(), data_, _shape, _origin, _region, dtype(), mtype(), value);
#endif
}

auto
Array::size() const -> size_t
{
  return width_ * height_ * depth_;
}
auto
Array::bitsize() const -> size_t
{
  return size() * itemSize();
}

auto
Array::width() const -> size_t
{
  return width_;
}

auto
Array::height() const -> size_t
{
  return height_;
}

auto
Array::depth() const -> size_t
{
  return depth_;
}

auto
Array::itemSize() const -> size_t
{
  return toBytes(dataType_);
}

auto
Array::dtype() const -> dType
{
  return dataType_;
}

auto
Array::mtype() const -> mType
{
  return memType_;
}

auto
Array::device() const -> Device::Pointer
{
  return device_;
}

auto
Array::dim() const -> unsigned int
{
  return (depth_ > 1) ? 3 : (height_ > 1) ? 2 : 1;
}

auto
Array::dimension() const -> unsigned int
{
  return dim_;
}

auto
Array::initialized() const -> bool
{
  return initialized_;
}

auto
Array::get() const -> void *
{
  return data_.get();
}

auto
Array::c_get() const -> const void *
{
  return data_.get();
}

auto
Array::get_ptr() const -> std::shared_ptr<void>
{
  return data_;
}


// DLPack interoperability
// Helper: compute C-contiguous strides for a given shape
static auto
makeContiguousStrides(int32_t ndim, const int64_t * shape) -> int64_t *
{
  auto * strides = new int64_t[ndim];
  strides[ndim - 1] = 1;
  for (int32_t i = ndim - 2; i >= 0; --i)
    strides[i] = strides[i + 1] * shape[i + 1];
  return strides;
}

auto
Array::toDLPack() const -> DLManagedTensorVersioned *
{
  if (mtype() != mType::BUFFER)
    throw std::runtime_error("DLPack export only supports BUFFER memory type");
  if (!initialized())
    throw std::runtime_error("Array is not initialized");

  auto * managed = new DLManagedTensorVersioned();
  managed->version = { DLPACK_MAJOR_VERSION, DLPACK_MINOR_VERSION };
  managed->flags = 0;

  // Shape: DLPack C-order — [depth, height, width]
  int32_t ndim = static_cast<int32_t>(dimension());
  auto *  shape = new int64_t[ndim];
  if (ndim == 1)
  {
    shape[0] = static_cast<int64_t>(width());
  }
  else if (ndim == 2)
  {
    shape[0] = static_cast<int64_t>(height());
    shape[1] = static_cast<int64_t>(width());
  }
  else
  {
    shape[0] = static_cast<int64_t>(depth());
    shape[1] = static_cast<int64_t>(height());
    shape[2] = static_cast<int64_t>(width());
  }

  int64_t * strides = makeContiguousStrides(ndim, shape);

  // Device type
  DLDeviceType dl_device_type;
#if USE_CUDA
  dl_device_type = kDLCUDA;
#elif USE_OPENCL
  dl_device_type = kDLOpenCL;
#endif

  managed->dl_tensor.data = get(); // void* — raw CUdeviceptr or cl_mem
  managed->dl_tensor.device = { dl_device_type, static_cast<int32_t>(device()->getDeviceIndex()) };
  managed->dl_tensor.ndim = ndim;
  managed->dl_tensor.dtype = toDLDataType(dtype());
  managed->dl_tensor.shape = shape;
  managed->dl_tensor.strides = strides; // required since DLPack v1.2
  managed->dl_tensor.byte_offset = 0;

  // Keep the Array alive via manager_ctx
  managed->manager_ctx = new Array::Pointer(shallow_copy());
  managed->deleter = [](DLManagedTensorVersioned * self) {
    delete[] self->dl_tensor.shape;
    delete[] self->dl_tensor.strides;
    delete static_cast<Array::Pointer *>(self->manager_ctx);
    delete self;
  };

  return managed;
}


auto
Array::fromDLPack(DLManagedTensorVersioned * src, const Device::Pointer & device_ptr) -> Array::Pointer
{
  if (src == nullptr)
  {
    throw std::invalid_argument("Error: DLManagedTensorVersioned is null");
  }

  // Version check: major version mismatch = must call deleter and bail
  if (src->version.major != DLPACK_MAJOR_VERSION)
  {
    if (src->deleter)
      src->deleter(src);
    throw std::runtime_error("DLPack major version mismatch");
  }

  const auto & t = src->dl_tensor;

#if USE_CUDA
  if (t.device.device_type != kDLCUDA)
    throw std::runtime_error("CUDA backend: DLPack tensor must be on a CUDA device");
#elif USE_OPENCL
  if (t.device.device_type != kDLOpenCL)
    throw std::runtime_error("OpenCL backend: DLPack tensor must be on an OpenCL device");
#endif

  if (t.byte_offset != 0)
    throw std::runtime_error("DLPack tensors with non-zero byte_offset are not supported");

  // Verify contiguous strides (strides is always non-null in v1.2+)
  {
    int64_t expected = 1;
    for (int32_t i = t.ndim - 1; i >= 0; --i)
    {
      if (t.strides[i] != expected)
        throw std::runtime_error("Non-contiguous DLPack tensors are not supported");
      expected *= t.shape[i];
    }
  }

  // Reconstruct w/h/d from C-order shape [d, h, w]
  size_t w = 1, h = 1, d = 1;
  if (t.ndim >= 1)
    w = static_cast<size_t>(t.shape[t.ndim - 1]);
  if (t.ndim >= 2)
    h = static_cast<size_t>(t.shape[t.ndim - 2]);
  if (t.ndim >= 3)
    d = static_cast<size_t>(t.shape[t.ndim - 3]);

  dType dt = fromDLDataType(t.dtype);

  // Wrap raw pointer — deleter is called when the shared_ptr ref-count hits 0
  DLManagedTensorVersioned * captured = src;
  auto                       shared_data = std::shared_ptr<void>(t.data, [captured](void *) {
    if (captured->deleter)
      captured->deleter(captured);
  });

  return std::shared_ptr<Array>(new Array(w, h, d, static_cast<size_t>(t.ndim), dt, mType::BUFFER, shared_data, device_ptr));
}

auto 
Array::syncToStream(int64_t consumer_stream) const -> void
{
  BackendManager::getInstance().getBackend().syncToStream(device(), consumer_stream);
}


} // namespace cle
