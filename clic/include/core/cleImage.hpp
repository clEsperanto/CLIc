#ifndef CLIC_INCLUDE_CORE_CLEIMAGE_HPP
#define CLIC_INCLUDE_CORE_CLEIMAGE_HPP

#include "cleLightObject.hpp"
#include "cleUtils.hpp"

namespace cle
{

class Image : public LightObject
{
  public:
    Image () = default;
    ~Image () = default;
    Image (const Image &obj) = default;
    Image (Image &&obj) = default;
    auto operator= (const Image &obj) -> Image & = default;
    auto operator= (Image &&obj) -> Image & = default;

    Image (const ProcessorPointer &device, const cl::Memory &data,
           const ShapeArray &shape, const DataType &data_type,
           const ObjectType &object_type, const ChannelsType &channels_type);

    auto Fill (float value) const -> void;
    auto CopyDataTo (const Image &dst_obj) const -> void;
    [[nodiscard]] auto Get () const -> cl::Memory;
    [[nodiscard]] auto Device () const -> ProcessorPointer;
    [[nodiscard]] auto Bytes () const -> size_t override;
    [[nodiscard]] auto Ndim () const -> unsigned int override;
    [[nodiscard]] auto Shape () const -> ShapeArray override;
    [[nodiscard]] auto Origin () const -> ShapeArray;
    [[nodiscard]] auto ObjectInfo () const -> std::string override;
    [[nodiscard]] auto DataInfo () const -> std::string override;
    [[nodiscard]] auto DataInfoShort () const -> std::string override;
    [[nodiscard]] auto IsBuffer () const -> bool;
    [[nodiscard]] auto IsImage () const -> bool;
    [[nodiscard]] auto ToString () const -> std::string override;
    [[nodiscard]] auto BitType () const -> DataType;
    [[nodiscard]] auto MemType () const -> ObjectType;

  private:
    cl::Memory data_;
    ProcessorPointer device_;
    unsigned int dim_{};
    ShapeArray shape_{ 1, 1, 1 };
    ShapeArray origin_{ 0, 0, 0 };
    DataType data_type_;
    ObjectType object_type_;
    ChannelsType channels_type_;
};

} // namespace cle

#endif // CLIC_INCLUDE_CORE_CLEIMAGE_HPP
