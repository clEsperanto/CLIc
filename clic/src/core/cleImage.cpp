#include "cleImage.hpp"
#include "cleBackend.hpp"
#include "cleTypes.hpp"

#include <numeric>
#include <sstream>

namespace cle
{

Image::Image(const ProcessorPointer & device,
             const cl::Memory &       data,
             const ShapeArray &       shape,
             const DataType &         data_type,
             const MemoryType &       object_type)
  : LightObject(data_type, object_type)
  , data_(data)
  , device_(device)
  , shape_(shape)
{
  if (shape_[2] > 1)
  {
    dim_ = 3;
  }
  else if (shape_[1] > 1)
  {
    dim_ = 2;
  }
  else
  {
    dim_ = 1;
  }
}

auto
Image::CopyDataTo(const Image & dst_img) const -> void
{
  if (this->GetDevice()->ContextPtr() != dst_img.GetDevice()->ContextPtr())
  {
    throw std::runtime_error("Error in CopyDataTo: Memory objects do not share the same context.");
  }
  if (this->GetDataType() != dst_img.GetDataType())
  {
    throw std::runtime_error("Error in CopyDataTo: Memory objects do not have the same data type.");
  }

  const bool src_is_buffer = (this->GetMemoryType() == MemoryType::BUFFER);
  const bool dst_is_buffer = (dst_img.GetMemoryType() == MemoryType::BUFFER);

  if (src_is_buffer && dst_is_buffer)
  {
    if (this->Ndim() > 1 && dst_img.Ndim() > 1)
    {
      Backend::EnqueueCopyBufferRect(this->GetDevice()->QueuePtr(),
                                     this->Get(),
                                     dst_img.Get(),
                                     true,
                                     this->Origin(),
                                     dst_img.Origin(),
                                     this->Shape());
    }
    else
    {
      Backend::EnqueueCopyBuffer(
        this->GetDevice()->QueuePtr(), this->Get(), dst_img.Get(), true, 0, 0, this->GetMemorySize());
    }
  }
  else if (src_is_buffer && !dst_is_buffer)
  {
    Backend::EnqueueCopyBufferToImage(
      this->GetDevice()->QueuePtr(), this->Get(), dst_img.Get(), true, 0, this->Origin(), dst_img.Shape());
  }
  else if (!src_is_buffer && dst_is_buffer)
  {
    Backend::EnqueueCopyImageToBuffer(
      this->GetDevice()->QueuePtr(), this->Get(), dst_img.Get(), true, this->Origin(), this->Shape(), 0);
  }
  else if (!src_is_buffer && !dst_is_buffer)
  {
    Backend::EnqueueCopyImage(
      this->GetDevice()->QueuePtr(), this->Get(), dst_img.Get(), true, this->Origin(), this->Origin(), this->Shape());
  }
}

auto
Image::Get() const -> const cl::Memory &
{
  return this->data_;
}

auto
Image::GetDevice() const -> ProcessorPointer
{
  return this->device_;
}

auto
Image::GetMemorySize() const -> size_t
{
  return this->Get().getInfo<CL_MEM_SIZE>();
}

auto
Image::GetDataSizeOf() const -> size_t
{
  return DataTypeToSizeOf(this->GetDataType());
}

auto
Image::Ndim() const -> unsigned int
{
  return this->dim_;
}

auto
Image::Shape() const -> const ShapeArray &
{
  return this->shape_;
}

auto
Image::GetNumberOfElements() const -> size_t
{
  return std::accumulate(shape_.begin(), shape_.end(), static_cast<size_t>(1), std::multiplies<size_t>{});
}

auto
Image::Origin() const -> const ShapeArray &
{
  return this->origin_;
}

auto
Image::ToString() const -> std::string
{
  std::ostringstream out_string{};
  out_string << MemoryTypeToString(GetMemoryType()) << "[" << std::to_string(Shape()[0]) << ","
             << std::to_string(Shape()[1]) << "," << std::to_string(Shape()[2]) << "]"
             << "(dtype=" << DataTypeToString(GetDataType()) << ")";
  return out_string.str();
}

auto
operator<<(std::ostream & out, const Image & image) -> std::ostream &
{
  out << image.ToString();
  return out;
}

auto
Image::Fill(const float & value) const -> void
{
  if (this->GetMemoryType() == MemoryType::BUFFER)
  {
    Backend::EnqueueFillBuffer(this->GetDevice()->QueuePtr(), this->Get(), true, 0, this->GetMemorySize(), value);
  }
  else if (this->GetMemoryType() == MemoryType::IMAGE)
  {
    switch (this->GetDataType())
    {
      case DataType::FLOAT32: {
        cl_float4 color = { static_cast<cl_float>(value),
                            static_cast<cl_float>(value),
                            static_cast<cl_float>(value),
                            static_cast<cl_float>(value) };
        Backend::EnqueueFillImage(
          this->GetDevice()->QueuePtr(), this->Get(), true, this->Origin(), this->Shape(), color);
        break;
      }
      case DataType::INT8:
      case DataType::INT16:
      case DataType::INT32: {
        cl_int4 color = {
          static_cast<cl_int>(value), static_cast<cl_int>(value), static_cast<cl_int>(value), static_cast<cl_int>(value)
        };
        Backend::EnqueueFillImage(
          this->GetDevice()->QueuePtr(), this->Get(), true, this->Origin(), this->Shape(), color);
        break;
      }
      case DataType::UINT8:
      case DataType::UINT16:
      case DataType::UINT32: {
        cl_uint4 color = { static_cast<cl_uint>(value),
                           static_cast<cl_uint>(value),
                           static_cast<cl_uint>(value),
                           static_cast<cl_uint>(value) };
        Backend::EnqueueFillImage(
          this->GetDevice()->QueuePtr(), this->Get(), true, this->Origin(), this->Shape(), color);
        break;
      }
      default:
        throw std::runtime_error("Unsupported data type for fill Image memory type.");
    }
  }
  else
  {
    throw std::runtime_error("Unsupported memory type for fill operation.");
  }
}

} // namespace cle
