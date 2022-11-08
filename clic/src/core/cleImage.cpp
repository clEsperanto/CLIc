#include "cleImage.hpp"
#include "cleBackend.hpp"
#include "cleTypes.hpp"

#include <sstream>

namespace cle
{

Image::Image(const ProcessorPointer & device,
             const cl::Memory &       data,
             const ShapeArray &       shape,
             const DataType &         data_type,
             const MemoryType &       object_type)
  : data_(data)
  , device_(device)
  , shape_(shape)
  , LightObject(data_type, object_type)
{
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
  switch (this->GetDataType())
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
  if (this->GetDevice()->ContextPtr() != dst_img.GetDevice()->ContextPtr())
  {
    std::cerr << "Error in CopyDataTo : Memory Objects does not share the same Context. \n";
    return;
  }
  if (this->GetDataType() != dst_img.GetDataType())
  {
    std::cerr << "Error in CopyDataTo : Memory Objects does not share the same bytes size. \n";
    return;
  }
  if (this->GetMemoryType() == BUFFER && dst_img.GetMemoryType() == BUFFER)
  {
    Backend::EnqueueCopyBuffer(this->GetDevice()->QueuePtr(), this->Get(), dst_img.Get(), true, 0, 0, this->GetSize());
    return;
  }

  bool src_is_buffer = false;
  if (this->GetMemoryType() == BUFFER)
  {
    src_is_buffer = true;
  }
  bool dst_is_buffer = false;
  if (dst_img.GetMemoryType() == BUFFER)
  {
    dst_is_buffer = true;
  }

  if (src_is_buffer && !dst_is_buffer)
  {
    Backend::EnqueueCopyBufferToImage(
      this->GetDevice()->QueuePtr(), this->Get(), dst_img.Get(), true, 0, this->Origin(), dst_img.Shape());
    return;
  }
  if (!src_is_buffer && dst_is_buffer)
  {
    Backend::EnqueueCopyImageToBuffer(
      this->GetDevice()->QueuePtr(), this->Get(), dst_img.Get(), true, this->Origin(), this->Shape(), 0);
    return;
  }
  if (!src_is_buffer && !dst_is_buffer)
  {
    Backend::EnqueueCopyImage(
      this->GetDevice()->QueuePtr(), this->Get(), dst_img.Get(), true, this->Origin(), this->Origin(), this->Shape());
    return;
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
Image::GetSize() const -> size_t
{
  return this->Get().getInfo<CL_MEM_SIZE>();
}

auto
Image::GetSizeOfElements() const -> size_t
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
Image::Origin() const -> const ShapeArray &
{
  return this->origin_;
}

auto
Image::GetMemoryType_Str() const -> std::string
{
  std::string res;
  switch (this->GetMemoryType())
  {
    case BUFFER:
      res = "buffer";
      break;
    case SCALAR:
      res = "scalar";
      break;
    case IMAGE1D:
      res = "image1d";
      break;
    case IMAGE2D:
      res = "image2d";
      break;
    case IMAGE3D:
      res = "image3d";
      break;
    default:
      res = "unknown";
  }
  return res;
}

auto
Image::GetDataType_Str(const bool & short_version) const -> std::string
{
  std::string res;
  switch (this->GetDataType())
  {
    case CL_SIGNED_INT8:
      res = (short_version) ? "c" : "char";
      break;
    case CL_SIGNED_INT16:
      res = (short_version) ? "s" : "short";
      break;
    case CL_SIGNED_INT32:
      res = (short_version) ? "i" : "int";
      break;
    case CL_UNSIGNED_INT8:
      res = (short_version) ? "uc" : "uchar";
      break;
    case CL_UNSIGNED_INT16:
      res = (short_version) ? "us" : "ushort";
      break;
    case CL_UNSIGNED_INT32:
      res = (short_version) ? "ui" : "uint";
      break;
    case CL_FLOAT:
      res = (short_version) ? "f" : "float";
      break;
    default:
      res = "unknown";
  }
  return res;
}

auto
Image::ToString() const -> std::string
{
  std::stringstream out_string;
  out_string << this->GetMemoryType_Str();
  out_string << "[" << std::to_string(this->Shape()[0]) << "," << std::to_string(this->Shape()[1]) << ","
             << std::to_string(this->Shape()[2]) << "]";
  out_string << "(dtype=" << this->GetDataType_Str(false) << ")";
  return out_string.str();
}

auto
operator<<(std::ostream & out, const Image & image) -> std::ostream &
{
  out << image.ToString();
  return out;
}

} // namespace cle
