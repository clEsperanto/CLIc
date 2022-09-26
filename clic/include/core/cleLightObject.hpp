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
  virtual ~LightObject() = default;
  LightObject(const LightObject & obj) = default;
  LightObject(LightObject && obj) = default;
  auto
  operator=(const LightObject & obj) -> LightObject & = default;
  auto
  operator=(LightObject && obj) -> LightObject & = default;

  // virtual auto Get () const -> void = 0;
  [[nodiscard]] virtual auto
  Ndim() const -> unsigned int = 0;
  [[nodiscard]] virtual auto
  Shape() const -> ShapeArray = 0;
  [[nodiscard]] virtual auto
  ObjectInfo() const -> std::string = 0;
  [[nodiscard]] virtual auto
  DataInfo() const -> std::string = 0;
  [[nodiscard]] virtual auto
  DataInfoShort() const -> std::string = 0;
  [[nodiscard]] virtual auto
  Bytes() const -> size_t = 0;
  [[nodiscard]] virtual auto
  ToString() const -> std::string = 0;
  [[nodiscard]] virtual auto
  Data() const -> DataType = 0;
  [[nodiscard]] virtual auto
  Object() const -> ObjectType = 0;
};

} // namespace cle

#endif // __CORE_CLELIGHTOBJECT_HPP
