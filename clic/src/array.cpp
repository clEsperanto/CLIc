#include "array.hpp"
#include "backend_common.hpp"

#include <array>

namespace cle
{

static auto
deduceDimension(size_t width, size_t height, size_t depth) -> unsigned int
{
  return (depth > 1) ? 3 : (height > 1) ? 2 : 1;
}

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
{
  resetStridesToContiguous();
}

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
{
  resetStridesToContiguous();
}

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
  if (new_width * new_height * new_depth != this->size())
  {
    throw std::invalid_argument("Error: Array reshape size mismatch. Original size: " + std::to_string(this->size()) +
                                ", new size: " + std::to_string(new_width * new_height * new_depth));
  }
  if (!isContiguous())
  {
    throw std::runtime_error("Error: Cannot reshape a non-contiguous Array");
  }
  if (new_dimension == 0)
  {
    new_dimension = this->dimension();
  }
  auto ptr = std::shared_ptr<Array>(
    new Array(new_width, new_height, new_depth, new_dimension, this->dtype(), this->mtype(), this->get_ptr(), this->device()));
  ptr->offset_ = offset_;
  return ptr;
}

auto
Array::shallow_copy() const -> Array::Pointer
{
  auto ptr = std::shared_ptr<Array>(new Array(width_, height_, depth_, dim_, dataType_, memType_, data_, device_));
  ptr->strides_ = strides_;
  ptr->offset_ = offset_;
  return ptr;
}

auto
Array::view(const std::array<size_t, 3> & origin, const std::array<size_t, 3> & region) const -> Array::Pointer
{
  if (!initialized())
  {
    throw std::runtime_error("Error: Cannot create a view on an uninitialized Array");
  }
  if (mtype() != mType::BUFFER)
  {
    throw std::runtime_error("Error: Views are only supported for BUFFER memory type");
  }
  const std::array<size_t, 3> dims = { width_, height_, depth_ };
  for (size_t i = 0; i < 3; ++i)
  {
    if (region[i] == 0 || origin[i] + region[i] > dims[i])
    {
      throw std::runtime_error("Error: View origin and region are out of bounds");
    }
  }
  size_t new_dim = deduceDimension(region[0], region[1], region[2]);
  auto   ptr = std::shared_ptr<Array>(new Array(region[0], region[1], region[2], new_dim, dataType_, memType_, data_, device_));
  ptr->strides_ = strides_;
  ptr->offset_ = offset_ + origin[0] * strides_[0] + origin[1] * strides_[1] + origin[2] * strides_[2];
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
  std::array<size_t, 3> _shape = { 0, 0, 0 };
  std::array<size_t, 3> _origin = { 0, 0, 0 };
  std::array<size_t, 3> _region = { this->width(), this->height(), this->depth() };
  resolveBufferAccess({ 0, 0, 0 }, _shape, _origin);
  BackendManager::getInstance().getBackend().writeMemory(device(), data_, _shape, _origin, _region, dtype(), mtype(), host_data);
}

auto
Array::writeFrom(const void * host_data, const std::array<size_t, 3> & region, const std::array<size_t, 3> & buffer_origin) -> void
{
  if (host_data == nullptr)
  {
    throw std::runtime_error("Error: Host data is null");
  }
  std::array<size_t, 3> _shape = { 0, 0, 0 };
  std::array<size_t, 3> _origin = { 0, 0, 0 };
  std::array<size_t, 3> _region = region;
  resolveBufferAccess(buffer_origin, _shape, _origin);
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
  std::array<size_t, 3> _shape = { 0, 0, 0 };
  std::array<size_t, 3> _origin = { 0, 0, 0 };
  std::array<size_t, 3> _region = { this->width(), this->height(), this->depth() };
  resolveBufferAccess({ 0, 0, 0 }, _shape, _origin);
  BackendManager::getInstance().getBackend().readMemory(device(), data_, _shape, _origin, _region, dtype(), mtype(), host_data);
}

auto
Array::readTo(void * host_data, const std::array<size_t, 3> & region, const std::array<size_t, 3> & buffer_origin) const -> void
{
  if (host_data == nullptr)
  {
    throw std::runtime_error("Error: Host data is null");
  }
  std::array<size_t, 3> _shape = { 0, 0, 0 };
  std::array<size_t, 3> _origin = { 0, 0, 0 };
  std::array<size_t, 3> _region = region;
  resolveBufferAccess(buffer_origin, _shape, _origin);
  BackendManager::getInstance().getBackend().readMemory(device(), data_, _shape, _origin, _region, dtype(), mtype(), host_data);
}

auto
Array::readTo(void * host_data, const size_t x_coord, const size_t y_coord, const size_t z_coord) const -> void
{
  readTo(host_data, { 1, 1, 1 }, { x_coord, y_coord, z_coord });
}

auto
Array::dispatchCopy(const Array::Pointer &        dst,
                    const std::array<size_t, 3> & src_origin,
                    const std::array<size_t, 3> & src_shape,
                    const std::array<size_t, 3> & dst_origin,
                    const std::array<size_t, 3> & dst_shape,
                    const std::array<size_t, 3> & region) const -> void
{
  auto &       backend = BackendManager::getInstance().getBackend();
  auto         dst_ptr = dst->get_ptr();
  const size_t bytes = toBytes(dtype());

  std::array<size_t, 3> _src_origin = src_origin;
  std::array<size_t, 3> _src_shape = src_shape;
  std::array<size_t, 3> _dst_origin = dst_origin;
  std::array<size_t, 3> _dst_shape = dst_shape;
  std::array<size_t, 3> _region = region;

  if (mtype() == mType::BUFFER && dst->mtype() == mType::BUFFER)
  {
    backend.copyMemoryBufferToBuffer(device(), data_, _src_origin, _src_shape, dst_ptr, _dst_origin, _dst_shape, _region, bytes);
  }
  else if (mtype() == mType::IMAGE && dst->mtype() == mType::IMAGE)
  {
    backend.copyMemoryImageToImage(device(), data_, _src_origin, _src_shape, dst_ptr, _dst_origin, _dst_shape, _region, bytes);
  }
  else if (mtype() == mType::BUFFER && dst->mtype() == mType::IMAGE)
  {
    if (!isContiguous() || offset() != 0)
    {
      throw std::runtime_error("Error: Copying a non-contiguous Array to an Image is not supported");
    }
    backend.copyMemoryBufferToImage(device(), data_, _src_origin, _src_shape, dst_ptr, _dst_origin, _dst_shape, _region, bytes);
  }
  else if (mtype() == mType::IMAGE && dst->mtype() == mType::BUFFER)
  {
    if (!dst->isContiguous() || dst->offset() != 0)
    {
      throw std::runtime_error("Error: Copying an Image to a non-contiguous Array is not supported");
    }
    backend.copyMemoryImageToBuffer(device(), data_, _src_origin, _src_shape, dst_ptr, _dst_origin, _dst_shape, _region, bytes);
  }
  else
  {
    throw std::runtime_error("Error: Copying Arrays from different memory types");
  }
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

  if (mtype() == mType::BUFFER && dst->mtype() == mType::BUFFER)
  {
    if (isContiguous() && offset() == 0 && dst->isContiguous() && dst->offset() == 0)
    {
      _region = { this->size(), 1, 1 };
      _src_shape = { this->size(), 1, 1 };
      _dst_shape = { dst->size(), 1, 1 };
    }
    else
    {
      if (this->width() != dst->width() || this->height() != dst->height() || this->depth() != dst->depth())
      {
        throw std::runtime_error("Error: Copying non-contiguous Arrays of different dimensions");
      }
      bufferLayout(_src_shape, _src_origin);
      dst->bufferLayout(_dst_shape, _dst_origin);
    }
  }
  else if (mtype() == mType::IMAGE && dst->mtype() == mType::IMAGE)
  {
    if (this->width() != dst->width() || this->height() != dst->height() || this->depth() != dst->depth())
    {
      throw std::runtime_error("Error: Copying Images of different dimensions");
    }
  }
  else if (mtype() == mType::BUFFER && dst->mtype() == mType::IMAGE)
  {
    _src_shape = { this->size(), 1, 1 };
  }
  else if (mtype() == mType::IMAGE && dst->mtype() == mType::BUFFER)
  {
    _dst_shape = { dst->size(), 1, 1 };
  }

  dispatchCopy(dst, _src_origin, _src_shape, _dst_origin, _dst_shape, _region);
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

  if (mtype() == mType::BUFFER && dst->mtype() == mType::BUFFER)
  {
    std::array<size_t, 3> _src_base = { 0, 0, 0 };
    std::array<size_t, 3> _dst_base = { 0, 0, 0 };
    bufferLayout(_src_shape, _src_base);
    dst->bufferLayout(_dst_shape, _dst_base);
    for (size_t i = 0; i < 3; ++i)
    {
      _src_origin[i] += _src_base[i];
      _dst_origin[i] += _dst_base[i];
    }
  }

  dispatchCopy(dst, _src_origin, _src_shape, _dst_origin, _dst_shape, _region);
}

static auto
fillWithHostData(Array & array, const float value) -> void
{
  if (array.dtype() == dType::COMPLEX || array.dtype() == dType::UNKNOWN)
  {
    throw std::runtime_error("Error: Unsupported data type");
  }
  detail::visitDType(array.dtype(), [&](auto tag) {
    using T = decltype(tag);
    const std::vector<T> data(array.size(), static_cast<T>(value));
    array.writeFrom(data.data());
  });
}

auto
Array::fill(const float value) -> void
{
#if defined(__APPLE__) && USE_OPENCL
  // clEnqueueFillBuffer not behaving as expected on Apple Silicon (OpenCL only, Metal fills natively)
  // FIX: Filling buffer with host data
  // TODO: Find a better solution
  fillWithHostData(*this, value);
#else
  if (mtype() == mType::BUFFER && (!isContiguous() || offset() != 0))
  {
    // setMemory fills from the buffer base and ignores origin, which would
    // corrupt data outside the view
    fillWithHostData(*this, value);
    return;
  }
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
Array::nbytes() const -> size_t
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
Array::strides() const -> const std::array<size_t, 3> &
{
  return strides_;
}

auto
Array::offset() const -> size_t
{
  return offset_;
}

auto
Array::isContiguous() const -> bool
{
  return strides_[0] == 1 && strides_[1] == width_ && strides_[2] == width_ * height_;
}

auto
Array::resetStridesToContiguous() -> void
{
  strides_ = { 1, width_, width_ * height_ };
  offset_ = 0;
}

auto
Array::bufferLayout(std::array<size_t, 3> & shape, std::array<size_t, 3> & origin) const -> void
{
  if (strides_[0] != 1)
  {
    throw std::runtime_error("Error: Arrays with non-unit x-stride are not supported for memory transfers");
  }
  const size_t row = strides_[1];
  const size_t slice = strides_[2];
  if (row == 0 || slice == 0 || slice % row != 0)
  {
    throw std::runtime_error("Error: Array strides do not describe a pitched buffer layout");
  }
  const size_t oz = offset_ / slice;
  const size_t rem = offset_ % slice;
  const size_t oy = rem / row;
  const size_t ox = rem % row;
  if (ox + width_ > row || (height_ > 1 && oy + height_ > slice / row))
  {
    throw std::runtime_error("Error: Array view exceeds its buffer layout bounds");
  }
  shape = { row, slice / row, oz + depth_ };
  origin = { ox, oy, oz };
}

auto
Array::resolveBufferAccess(const std::array<size_t, 3> & buffer_origin, std::array<size_t, 3> & shape, std::array<size_t, 3> & origin) const
  -> void
{
  std::array<size_t, 3> base = { 0, 0, 0 };
  shape = { this->width(), this->height(), this->depth() };
  bufferLayout(shape, base);
  origin = { base[0] + buffer_origin[0], base[1] + buffer_origin[1], base[2] + buffer_origin[2] };
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
  return deduceDimension(width_, height_, depth_);
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

/**
 * @brief Wait for the device work that may still use memory handed back across DLPack.
 *
 * @param all_users CUDA: also wait for the other users of the context (the consumer of an exported tensor — CuPy,
 *                  PyTorch — works in the same primary context, on its own streams), not only for the device stream
 *
 * Called from deleters: never throws (a device that is gone, or belongs to another backend, is skipped).
 */
static auto
waitBeforeDLPackRelease(const Device::Pointer & device, bool all_users) noexcept -> void
{
  if (device == nullptr || !device->isInitialized())
  {
    return;
  }
  try
  {
#if USE_CUDA
    if (all_users)
    {
      if (const auto cuda_device = std::dynamic_pointer_cast<const CUDADevice>(device))
      {
        if (cuCtxPushCurrent(cuda_device->getCUDAContext()) == CUDA_SUCCESS)
        {
          cuCtxSynchronize();
          CUcontext popped = nullptr;
          cuCtxPopCurrent(&popped);
        }
        return;
      }
    }
#endif
    BackendManager::getInstance().getBackend().syncToStream(device, 0);
  }
  catch (...)
  {}
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

  // Strides: DLPack C-order — element strides for [depth, height, width]
  auto * strides = new int64_t[ndim];
  for (int32_t i = 0; i < ndim; ++i)
  {
    strides[i] = static_cast<int64_t>(strides_[ndim - 1 - i]);
  }

  // Device type
  DLDeviceType dl_device_type;
#if USE_CUDA
  dl_device_type = kDLCUDA;
#elif USE_OPENCL
  dl_device_type = kDLOpenCL;
#elif USE_METAL
  dl_device_type = kDLMetal;
#endif

  managed->dl_tensor.data = get(); // void* — raw CUdeviceptr or cl_mem
  managed->dl_tensor.device = { dl_device_type, static_cast<int32_t>(device()->getDeviceIndex()) };
  managed->dl_tensor.ndim = ndim;
  managed->dl_tensor.dtype = toDLDataType(dtype());
  managed->dl_tensor.shape = shape;
  managed->dl_tensor.strides = strides; // required since DLPack v1.2
  managed->dl_tensor.byte_offset = static_cast<uint64_t>(offset_ * itemSize());

  // Keep the Array alive via manager_ctx
  managed->manager_ctx = new Array::Pointer(shallow_copy());
  managed->deleter = [](DLManagedTensorVersioned * self) {
    auto * array = static_cast<Array::Pointer *>(self->manager_ctx);
    // The consumer may release the tensor while kernels it queued on its own streams still use the memory; once the
    // Array is gone, a stream-ordered free (CUDA) would let our next allocation reuse it too early.
    if (*array != nullptr)
    {
      waitBeforeDLPackRelease((*array)->device(), true);
    }
    delete[] self->dl_tensor.shape;
    delete[] self->dl_tensor.strides;
    delete array;
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
#elif USE_METAL
  if (t.device.device_type != kDLMetal)
    throw std::runtime_error("Metal backend: DLPack tensor must be on a Metal device");
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
  // Before handing the memory back to its producer (which may free or reuse it at once), wait for our queued work
  DLManagedTensorVersioned * captured = src;
  auto                       shared_data = std::shared_ptr<void>(t.data, [captured, device_ptr](void *) {
    waitBeforeDLPackRelease(device_ptr, false);
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
