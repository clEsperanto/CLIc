#ifndef __CORE_CLEIMAGE_HPP
#define __CORE_CLEIMAGE_HPP

#include "cleBackend.hpp"
#include "cleLightObject.hpp"
#include "cleTypes.hpp"


namespace cle
{

class Image : public LightObject
{
public:
  Image() = default;
  ~Image() override = default;
  Image(const Image & obj) = default;
  Image(Image && obj) = default;
  auto
  operator=(const Image & obj) -> Image & = default;
  auto
  operator=(Image && obj) -> Image & = default;

  Image(const ProcessorPointer & device,
        const cl::Memory &       data,
        const ShapeArray &       shape,
        const DataType &         data_type,
        const MemoryType &       object_type);

  auto
  Fill(const float & value) const -> void;
  auto
  CopyDataTo(const Image & dst_img) const -> void;
  [[nodiscard]] auto
  Get() const -> const cl::Memory &;
  [[nodiscard]] auto
  GetDevice() const -> ProcessorPointer;
  [[nodiscard]] auto
  GetSizeOfElements() const -> size_t override;
  [[nodiscard]] auto
  GetSize() const -> size_t;
  [[nodiscard]] auto
  Ndim() const -> unsigned int override;
  [[nodiscard]] auto
  Shape() const -> const ShapeArray & override;
  [[nodiscard]] auto
  Origin() const -> const ShapeArray &;
  [[nodiscard]] auto
  GetMemoryType_Str() const -> std::string override;
  [[nodiscard]] auto
  GetDataType_Str(const bool & short_version) const -> std::string override;
  [[nodiscard]] auto
  ToString() const -> std::string override;

  friend auto
  operator<<(std::ostream & out, const Image & image) -> std::ostream &;

private:
  cl::Memory       data_;
  ProcessorPointer device_;
  unsigned int     dim_{};
  ShapeArray       shape_{ 1, 1, 1 };
  ShapeArray       origin_{ 0, 0, 0 };
  ChannelType      channels_type_ = INTENSITY;

  template <class type>
  auto
  CastFill(const type & value) const -> void
  {
    if (this->GetMemoryType() == BUFFER)
    {
      Backend::EnqueueFillBuffer(this->GetDevice()->QueuePtr(), this->Get(), true, 0, this->GetSize(), value);
    }
    else
    {
      switch (this->GetDataType())
      {
        case FLOAT: {
          cl_float4 color = { static_cast<cl_float>(value),
                              static_cast<cl_float>(value),
                              static_cast<cl_float>(value),
                              static_cast<cl_float>(value) };
          Backend::EnqueueFillImage(
            this->GetDevice()->QueuePtr(), this->Get(), true, this->Origin(), this->Shape(), color);
          break;
        }
        case INT8:
        case INT16:
        case INT32: {
          cl_int4 color = { static_cast<cl_int>(value),
                            static_cast<cl_int>(value),
                            static_cast<cl_int>(value),
                            static_cast<cl_int>(value) };
          Backend::EnqueueFillImage(
            this->GetDevice()->QueuePtr(), this->Get(), true, this->Origin(), this->Shape(), color);
          break;
        }
        case UINT8:
        case UINT16:
        case UINT32: {
          cl_uint4 color = { static_cast<cl_uint>(value),
                             static_cast<cl_uint>(value),
                             static_cast<cl_uint>(value),
                             static_cast<cl_uint>(value) };
          Backend::EnqueueFillImage(
            this->GetDevice()->QueuePtr(), this->Get(), true, this->Origin(), this->Shape(), color);
          break;
        }
      }
    }
  }
};

} // namespace cle

#endif // __CORE_CLEIMAGE_HPP
