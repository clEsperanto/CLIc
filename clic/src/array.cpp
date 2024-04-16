#include "array.hpp"

#include <array>

namespace cle
{

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
  , data_(std::make_shared<void *>(nullptr))
{}

Array::~Array()
{
  if (initialized() && data_.unique())
  {
    backend_.freeMemory(device(), mtype(), get());
  }
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
  ptr->write(host_data);
  return ptr;
}

auto
Array::create(const Array::Pointer & array) -> Array::Pointer
{
  auto ptr = create(array->width(),
                    array->height(),
                    array->depth(),
                    array->dimension(),
                    array->dtype(),
                    array->mtype(),
                    array->device());
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
  backend_.allocateMemory(device(), { this->width(), this->height(), this->depth() }, dtype(), mtype(), get());
  initialized_ = true;
}

auto
Array::write(const void * host_data) -> void
{
  if (host_data == nullptr)
  {
    throw std::runtime_error("Error: Host data is null");
  }
  if (!initialized())
  {
    allocate();
  }
  std::array<size_t, 3> _origin = { 0, 0, 0 };
  std::array<size_t, 3> _shape = { this->width(), this->height(), this->depth() };
  std::array<size_t, 3> _region = { this->width(), this->height(), this->depth() };
  backend_.writeMemory(device(), get(), _shape, _origin, _region, dtype(), mtype(), host_data);
}

auto
Array::write(const void * host_data, const std::array<size_t, 3> & region, const std::array<size_t, 3> & buffer_origin)
  -> void
{
  if (host_data == nullptr)
  {
    throw std::runtime_error("Error: Host data is null");
  }
  if (!initialized())
  {
    allocate();
  }
  std::array<size_t, 3> _origin = buffer_origin;
  std::array<size_t, 3> _region = region;
  std::array<size_t, 3> _shape = { this->width(), this->height(), this->depth() };
  backend_.writeMemory(device(), get(), _shape, _origin, _region, dtype(), mtype(), host_data);
}

auto
Array::write(const void * host_data, const size_t x_coord, const size_t y_coord, const size_t z_coord) -> void
{
  write(host_data, { 1, 1, 1 }, { x_coord, y_coord, z_coord });
}

auto
Array::read(void * host_data) const -> void
{
  if (host_data == nullptr)
  {
    throw std::runtime_error("Error: Host data is null");
  }
  if (!initialized())
  {
    throw std::runtime_error("Error: Array is not initialized, it cannot be read");
  }
  std::array<size_t, 3> _origin = { 0, 0, 0 };
  std::array<size_t, 3> _shape = { this->width(), this->height(), this->depth() };
  std::array<size_t, 3> _region = { this->width(), this->height(), this->depth() };
  backend_.readMemory(device(), c_get(), _shape, _origin, _region, dtype(), mtype(), host_data);
}

auto
Array::read(void * host_data, const std::array<size_t, 3> & region, const std::array<size_t, 3> & buffer_origin) const
  -> void
{
  if (host_data == nullptr)
  {
    throw std::runtime_error("Error: Host data is null");
  }
  if (!initialized())
  {
    throw std::runtime_error("Error: Array is not initialized, it cannot be read");
  }
  std::array<size_t, 3> _origin = buffer_origin;
  std::array<size_t, 3> _region = region;
  std::array<size_t, 3> _shape = { this->width(), this->height(), this->depth() };
  backend_.readMemory(device(), c_get(), _shape, _origin, _region, dtype(), mtype(), host_data);
}

auto
Array::read(void * host_data, const size_t x_coord, const size_t y_coord, const size_t z_coord) const -> void
{
  read(host_data, { 1, 1, 1 }, { x_coord, y_coord, z_coord });
}

auto
Array::copy(const Array::Pointer & dst) const -> void
{
  if (!initialized() || !dst->initialized())
  {
    throw std::runtime_error("Error: Arrays are not initialized_");
  }
  if (device() != dst->device())
  {
    throw std::runtime_error("Error: Copying Arrays from different devices");
  }
  if (width() != dst->width() || height() != dst->height() || depth() != dst->depth() || itemSize() != dst->itemSize())
  {
    throw std::runtime_error("Error: Arrays dimensions do not match");
  }
  std::array<size_t, 3> _src_origin = { 0, 0, 0 };
  std::array<size_t, 3> _dst_origin = { 0, 0, 0 };
  std::array<size_t, 3> _region = { this->width(), this->height(), this->depth() };
  std::array<size_t, 3> _src_shape = { this->width(), this->height(), this->depth() };
  std::array<size_t, 3> _dst_shape = { dst->width(), dst->height(), dst->depth() };
  if (mtype() == mType::BUFFER && dst->mtype() == mType::BUFFER)
  {
    backend_.copyMemoryBufferToBuffer(
      device(), c_get(), _src_origin, _src_shape, dst->get(), _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
  else if (mtype() == mType::IMAGE && dst->mtype() == mType::IMAGE)
  {
    backend_.copyMemoryImageToImage(
      device(), c_get(), _src_origin, _src_shape, dst->get(), _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
  else if (mtype() == mType::BUFFER && dst->mtype() == mType::IMAGE)
  {
    backend_.copyMemoryBufferToImage(
      device(), c_get(), _src_origin, _src_shape, dst->get(), _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
  else if (mtype() == mType::IMAGE && dst->mtype() == mType::BUFFER)
  {
    backend_.copyMemoryImageToBuffer(
      device(), c_get(), _src_origin, _src_shape, dst->get(), _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
  else
  {
    throw std::runtime_error("Error: Copying Arrays from different memory types");
  }
}

auto
Array::copy(const Array::Pointer &        dst,
            const std::array<size_t, 3> & region,
            const std::array<size_t, 3> & src_origin,
            const std::array<size_t, 3> & dst_origin) const -> void
{
  if (!initialized() || !dst->initialized())
  {
    throw std::runtime_error("Error: Arrays are not initialized_");
  }
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
    backend_.copyMemoryBufferToBuffer(
      device(), c_get(), _src_origin, _src_shape, dst->get(), _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
  else if (mtype() == mType::IMAGE && dst->mtype() == mType::IMAGE)
  {
    backend_.copyMemoryImageToImage(
      device(), c_get(), _src_origin, _src_shape, dst->get(), _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
  else if (mtype() == mType::BUFFER && dst->mtype() == mType::IMAGE)
  {
    backend_.copyMemoryBufferToImage(
      device(), c_get(), _src_origin, _src_shape, dst->get(), _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
  else if (mtype() == mType::IMAGE && dst->mtype() == mType::BUFFER)
  {
    backend_.copyMemoryImageToBuffer(
      device(), c_get(), _src_origin, _src_shape, dst->get(), _dst_origin, _dst_shape, _region, toBytes(dtype()));
  }
}

auto
Array::fill(const float value) -> void
{
  if (!initialized())
  {
    throw std::runtime_error("Error: Array it is not initialized.");
  }

#if defined(__APPLE__) && defined(__arm64__)
  // clEnqueueFillBuffer not behaving as expected on Apple Silicon
  // FIX: Filling buffer with host data
  // TODO: Find a better solution
  auto data_type = dtype();
  switch (data_type)
  {
    case dType::FLOAT: {
      std::vector<float> data(this->size(), value);
      write(data.data());
      return;
    }
    case dType::INT8: {
      std::vector<int8_t> data(this->size(), static_cast<int8_t>(value));
      write(data.data());
      return;
    }
    case dType::INT16: {
      std::vector<int16_t> data(this->size(), static_cast<int16_t>(value));
      write(data.data());
      return;
    }
    case dType::INT32: {
      std::vector<int32_t> data(this->size(), static_cast<int32_t>(value));
      write(data.data());
      return;
    }
    case dType::UINT8: {
      std::vector<uint8_t> data(this->size(), static_cast<uint8_t>(value));
      write(data.data());
      return;
    }
    case dType::UINT16: {
      std::vector<uint16_t> data(this->size(), static_cast<uint16_t>(value));
      write(data.data());
      return;
    }
    case dType::UINT32: {
      std::vector<uint32_t> data(this->size(), static_cast<uint32_t>(value));
      write(data.data());
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
  backend_.setMemory(device(), get(), _shape, _origin, _region, dtype(), mtype(), value);
#endif
}

auto
Array::size() const -> size_t
{
  return width_ * height_ * depth_;
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
Array::get() const -> void **
{
  return data_.get();
}
auto
Array::c_get() const -> const void **
{
  return (const void **)data_.get();
}

auto
Array::shortType() const -> std::string
{
  const auto str_type = toString(dtype());
  return (str_type[0] == 'u') ? str_type.substr(0, 2) : str_type.substr(0, 1);
}


} // namespace cle
