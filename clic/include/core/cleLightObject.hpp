#ifndef __CORE_CLELIGHTOBJECT_HPP
#define __CORE_CLELIGHTOBJECT_HPP

#include <array>
#include <memory>

#include "cleProcessor.hpp"
#include "cleTypes.hpp"

namespace cle
{

class LightObject
{
public:
  using ShapeArray = std::array<size_t, 3>;
  using ProcessorPointer = std::shared_ptr<Processor>;

  LightObject() = default;
  LightObject(const DataType & data_t, const MemoryType & mem_t)
    : data_type_(data_t)
    , mem_type_(mem_t){};
  virtual ~LightObject() = default;
  LightObject(const LightObject & obj) = default;
  LightObject(LightObject && obj) = default;
  auto
  operator=(const LightObject & obj) -> LightObject & = default;
  auto
  operator=(LightObject && obj) -> LightObject & = default;

  [[nodiscard]] virtual auto
  Ndim() const -> unsigned int = 0;
  [[nodiscard]] virtual auto
  Shape() const -> const ShapeArray & = 0;
  [[nodiscard]] virtual auto
  GetNumberOfElements() const -> size_t = 0;
  [[nodiscard]] virtual auto
  GetDataSizeOf() const -> size_t = 0;
  [[nodiscard]] virtual auto
  GetMemorySize() const -> size_t = 0;
  [[nodiscard]] virtual auto
  ToString() const -> std::string = 0;

  [[nodiscard]] virtual auto
  GetDataType() const -> DataType
  {
    return data_type_;
  }

  [[nodiscard]] virtual auto
  GetMemoryType() const -> MemoryType
  {
    return mem_type_;
  }

private:
  MemoryType mem_type_ = MemoryType::BUFFER;
  DataType   data_type_ = DataType::FLOAT32;
};

} // namespace cle

#endif // __CORE_CLELIGHTOBJECT_HPP
