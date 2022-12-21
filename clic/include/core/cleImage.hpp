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
  GetDataSizeOf() const -> size_t override;
  [[nodiscard]] auto
  GetMemorySize() const -> size_t override;
  [[nodiscard]] auto
  Ndim() const -> unsigned int override;
  [[nodiscard]] auto
  Shape() const -> const ShapeArray & override;
  [[nodiscard]] auto
  GetNumberOfElements() const -> size_t override;
  [[nodiscard]] auto
  Origin() const -> const ShapeArray &;
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
    if (this->GetMemoryType() == MemoryType::BUFFER)
    {
      Backend::EnqueueFillBuffer(this->GetDevice()->QueuePtr(), this->Get(), true, 0, this->GetMemorySize(), value);
    }
    else
    {
      switch (this->GetDataType())
      {
        case DataType::DOUBLE:
        case DataType::FLOAT: {
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
          cl_int4 color = { static_cast<cl_int>(value),
                            static_cast<cl_int>(value),
                            static_cast<cl_int>(value),
                            static_cast<cl_int>(value) };
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
      }
    }
  }
};

} // namespace cle

#endif // __CORE_CLEIMAGE_HPP
