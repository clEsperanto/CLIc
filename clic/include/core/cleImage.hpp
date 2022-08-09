#ifndef __CORE_CLEIMAGE_HPP
#define __CORE_CLEIMAGE_HPP

#include "cleBackend.hpp"
#include "cleLightObject.hpp"
#include "cleTypes.hpp"
#include "cleUtils.hpp"

namespace cle
{

class Image : public LightObject
{
public:
  Image() = default;
  ~Image() = default;
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
        const MemoryType &       object_type,
        const ChannelsType &     channels_type);

  auto
  Fill(const float & value) const -> void;
  auto
  CopyDataTo(const Image & dst_obj) const -> void;
  [[nodiscard]] auto
  Get() const -> cl::Memory;
  [[nodiscard]] auto
  Device() const -> ProcessorPointer;
  [[nodiscard]] auto
  Bytes() const -> size_t override;
  [[nodiscard]] auto
  Ndim() const -> unsigned int override;
  [[nodiscard]] auto
  Shape() const -> ShapeArray override;
  [[nodiscard]] auto
  ShapeZYX() const -> ShapeArray;
  [[nodiscard]] auto
  Origin() const -> ShapeArray;
  [[nodiscard]] auto
  MemoryInfo() const -> std::string override;
  [[nodiscard]] auto
  DataInfo() const -> std::string override;
  [[nodiscard]] auto
  DataInfoShort() const -> std::string override;
  [[nodiscard]] auto
  IsBuffer() const -> bool;
  [[nodiscard]] auto
  IsImage() const -> bool;
  [[nodiscard]] auto
  ToString() const -> std::string override;
  [[nodiscard]] auto
  BitType() const -> DataType;
  [[nodiscard]] auto
  Memory() const -> MemoryType;


private:
  cl::Memory       data_;
  ProcessorPointer device_;
  unsigned int     dim_{};
  ShapeArray       shape_{ 1, 1, 1 };
  ShapeArray       origin_{ 0, 0, 0 };
  DataType         data_type_;
  ChannelsType     channels_type_;
  MemoryType       mem_type_;

  template <class type>
  auto
  CastFill(const type & value) const -> void
  {
    if (this->IsBuffer())
    {
      Backend::EnqueueFillBuffer(this->Device()->Queue(), this->Get(), true, 0, this->Bytes(), value);
    }
    else
    {
      if (this->DataInfo().find("float"))
      {
        cl_float4 color = { static_cast<cl_float>(value),
                            static_cast<cl_float>(value),
                            static_cast<cl_float>(value),
                            static_cast<cl_float>(value) };
        Backend::EnqueueFillImage(this->Device()->Queue(), this->Get(), true, this->Origin(), this->Shape(), color);
      }
      else if (this->DataInfo().find("unsigned"))
      {
        cl_uint4 color = { static_cast<cl_uint>(value),
                           static_cast<cl_uint>(value),
                           static_cast<cl_uint>(value),
                           static_cast<cl_uint>(value) };
        Backend::EnqueueFillImage(this->Device()->Queue(), this->Get(), true, this->Origin(), this->Shape(), color);
      }
      else
      {
        cl_int4 color = {
          static_cast<cl_int>(value), static_cast<cl_int>(value), static_cast<cl_int>(value), static_cast<cl_int>(value)
        };
        Backend::EnqueueFillImage(this->Device()->Queue(), this->Get(), true, this->Origin(), this->Shape(), color);
      }
    }
  }
};

} // namespace cle

#endif // __CORE_CLEIMAGE_HPP
