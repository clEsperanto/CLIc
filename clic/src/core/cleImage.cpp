#include "cleImage.hpp"
#include "cleBackend.hpp"
#include "cleTypes.hpp"

namespace cle
{

Image::Image(const ProcessorPointer & device,
             const cl::Memory &       data,
             const ShapeArray &       shape,
             const DataType &         data_type,
             const ObjectType &       object_type,
             const ChannelType &      channels_type)
{
  this->data_ = data;
  this->device_ = device;
  this->shape_ = shape;
  this->data_type_ = data_type;
  this->mem_type_ = object_type;
  this->channels_type_ = channels_type;
  if (this->shape_[2] > 1)
  {
    this->dim_ = 3;
  }
  else if (this->shape_[1] > 1)
  {
    this->dim_ = 2;
  }
  else
  {
    this->dim_ = 1;
  }
}

auto
Image::Fill(const float & value) const -> void
{
  switch (this->Data())
  {
    case FLOAT:
      this->CastFill<float>(static_cast<float>(value));
      break;
    case INT32:
      this->CastFill<int32_t>(static_cast<int32_t>(value));
      break;
    case UINT32:
      this->CastFill<uint32_t>(static_cast<uint32_t>(value));
      break;
    case INT16:
      this->CastFill<int16_t>(static_cast<int16_t>(value));
      break;
    case UINT16:
      this->CastFill<uint16_t>(static_cast<uint16_t>(value));
      break;
    case INT8:
      this->CastFill<int8_t>(static_cast<int8_t>(value));
      break;
    case UINT8:
      this->CastFill<uint8_t>(static_cast<uint8_t>(value));
      break;
  }
}

auto
Image::CopyDataTo(const Image & dst_img) const -> void
{
  if (this->Device()->Context() != dst_img.Device()->Context())
  {
    std::cerr << "Error in CopyDataTo : Memory Objects does not share the same Context. \n";
    return;
  }
  if (this->Data() != dst_img.Data())
  {
    std::cerr << "Error in CopyDataTo : Memory Objects does not share the same bytes size. \n";
    return;
  }
  if (this->IsBuffer() && dst_img.IsBuffer())
  {
    Backend::EnqueueCopyBuffer(this->Device()->Queue(), this->Get(), dst_img.Get(), true, 0, 0, this->Bytes());
    return;
  }
  if (this->IsBuffer() && dst_img.IsImage())
  {
    Backend::EnqueueCopyBufferToImage(
      this->Device()->Queue(), this->Get(), dst_img.Get(), true, 0, this->Origin(), dst_img.Shape());
    return;
  }
  if (this->IsImage() && dst_img.IsBuffer())
  {
    Backend::EnqueueCopyImageToBuffer(
      this->Device()->Queue(), this->Get(), dst_img.Get(), true, this->Origin(), this->Shape(), 0);
    return;
  }
  if (this->IsImage() && dst_img.IsImage())
  {
    Backend::EnqueueCopyImage(
      this->Device()->Queue(), this->Get(), dst_img.Get(), true, this->Origin(), this->Origin(), this->Shape());
    return;
  }
}

auto
Image::Get() const -> cl::Memory
{
  return this->data_;
}

auto
Image::Device() const -> ProcessorPointer
{
  return this->device_;
}

auto
Image::Bytes() const -> size_t
{
  return this->Get().getInfo<CL_MEM_SIZE>();
}

auto
Image::Ndim() const -> unsigned int
{
  return this->dim_;
}

auto
Image::Shape() const -> ShapeArray
{
  return this->shape_;
}

auto
Image::ShapeZYX() const -> ShapeArray
{
  return { this->Shape()[2], this->Shape()[1], this->Shape()[0] };
}

auto
Image::Origin() const -> ShapeArray
{
  return this->origin_;
}

auto
Image::ObjectInfo() const -> std::string
{
  return ObjectTypeToString(this->Object());
}

auto
Image::DataInfo() const -> std::string
{
  return DataTypeToString(this->Data());
}

auto
Image::DataInfoShort() const -> std::string
{
  return DataTypeToString(this->Data(), true);
}

auto
Image::IsBuffer() const -> bool
{
  return this->Object() == BUFFER;
}

auto
Image::IsImage() const -> bool
{
  return (this->Object() == IMAGE1D) || (this->Object() == IMAGE2D) || (this->Object() == IMAGE3D);
}

auto
Image::Data() const -> DataType
{
  return this->data_type_;
}

auto
Image::Object() const -> ObjectType
{
  return this->mem_type_;
}

auto
Image::ToString() const -> std::string
{
  std::string str = this->ObjectInfo() + "(" + this->DataInfo() + ")";
  str += " of shape=[" + std::to_string(this->Shape()[0]) + "," + std::to_string(this->Shape()[1]) + "," +
         std::to_string(this->Shape()[2]) + "]";
  return str;
}

} // namespace cle
