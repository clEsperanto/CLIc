#include "cleImage.hpp"
#include "cleBackend.hpp"
#include "cleTypes.hpp"


namespace cle
{

Image::Image(const ProcessorPointer & device,
             const cl::Memory &       data,
             const ShapeArray &       shape,
             const DataType &         data_type,
             const MemoryType &       object_type,
             const ChannelsType &     channels_type)
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
  switch (this->BitType().Get())
  {
    case CL_FLOAT:
      this->CastFill<float>(static_cast<float>(value));
      break;
    case CL_SIGNED_INT32:
      this->CastFill<int>(static_cast<int>(value));
      break;
    case CL_UNSIGNED_INT32:
      this->CastFill<unsigned int>(static_cast<unsigned int>(value));
      break;
    case CL_SIGNED_INT16:
      this->CastFill<short>(static_cast<short>(value));
      break;
    case CL_UNSIGNED_INT16:
      this->CastFill<unsigned short>(static_cast<unsigned short>(value));
      break;
    case CL_SIGNED_INT8:
      this->CastFill<char>(static_cast<char>(value));
      break;
    case CL_UNSIGNED_INT8:
      this->CastFill<unsigned char>(static_cast<unsigned char>(value));
      break;
  }
}

auto
Image::CopyDataTo(const Image & dst_obj) const -> void
{
  if (this->Device()->Context() != dst_obj.Device()->Context())
  {
    std::cerr << "Error in CopyDataTo : Memory Objects does not share the same Context. \n";
    return;
  }
  if (this->Bytes() != dst_obj.Bytes())
  {
    std::cerr << "Error in CopyDataTo : Memory Objects does not share the same bytes size. \n";
    return;
  }
  if (this->IsBuffer() && dst_obj.IsBuffer())
  {
    Backend::EnqueueCopyBuffer(this->Device()->Queue(), this->Get(), dst_obj.Get(), true, 0, 0, this->Bytes());
    return;
  }
  if (this->IsBuffer() && dst_obj.IsImage())
  {
    Backend::EnqueueCopyBufferToImage(
      this->Device()->Queue(), this->Get(), dst_obj.Get(), true, 0, this->Origin(), dst_obj.Shape());
    return;
  }
  if (this->IsImage() && dst_obj.IsBuffer())
  {
    Backend::EnqueueCopyImageToBuffer(
      this->Device()->Queue(), this->Get(), dst_obj.Get(), true, this->Origin(), this->Shape(), 0);
    return;
  }
  if (this->IsImage() && dst_obj.IsImage())
  {
    Backend::EnqueueCopyImage(
      this->Device()->Queue(), this->Get(), dst_obj.Get(), true, this->Origin(), this->Origin(), this->Shape());
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
Image::MemoryInfo() const -> std::string
{
  if (this->IsBuffer())
  {
    return "buffer";
  }
  return "image";
}

auto
Image::DataInfo() const -> std::string
{
  return this->data_type_.Str();
}

auto
Image::DataInfoShort() const -> std::string
{
  return this->data_type_.Str_s();
}

auto
Image::IsBuffer() const -> bool
{
  return this->mem_type_ == BUFFER;
}

auto
Image::IsImage() const -> bool
{
  return this->mem_type_ == IMAGE;
}

auto
Image::BitType() const -> DataType
{
  return this->data_type_;
}

auto
Image::Memory() const -> MemoryType
{
  return this->mem_type_;
}

auto
Image::ToString() const -> std::string
{
  std::string str = this->MemoryInfo() + "(" + this->DataInfo() + ")";
  str += " of shape=[" + std::to_string(this->Shape()[0]) + "," + std::to_string(this->Shape()[1]) + "," +
         std::to_string(this->Shape()[2]) + "]";
  return str;
}

} // namespace cle
