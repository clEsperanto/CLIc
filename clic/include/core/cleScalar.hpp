#ifndef __CORE_CLESCALAR_HPP
#define __CORE_CLESCALAR_HPP

#include "cleLightObject.hpp"

#include <sstream>

namespace cle
{

template <class Type>
class Scalar : public LightObject
{
public:
  Scalar() = delete;
  ~Scalar() override = default;
  Scalar(const Scalar & obj) noexcept = default;
  Scalar(Scalar && obj) noexcept = default;
  auto
  operator=(const Scalar & obj) -> Scalar & = default;
  auto
  operator=(Scalar && obj) noexcept -> Scalar & = default;
  explicit Scalar(const Type & data);

  [[nodiscard]] auto
  Get() const -> Type;
  [[nodiscard]] auto
  Ndim() const -> unsigned int override;
  [[nodiscard]] auto
  Shape() const -> const ShapeArray & override;
  [[nodiscard]] auto
  GetDataSizeOf() const -> size_t override;
  [[nodiscard]] auto
  GetNumberOfElements() const -> size_t override;
  [[nodiscard]] auto
  GetMemorySize() const -> size_t override;
  [[nodiscard]] auto
  ToString() const -> std::string override;

  template <class T>
  friend auto
  operator<<(std::ostream & out, const Scalar<T> & scalar) -> std::ostream &;

private:
  Type       data_;
  ShapeArray shape_{ 1, 1, 1 };
};

template <class Type>
Scalar<Type>::Scalar(const Type & data)
  : data_(data)
  , LightObject(TypeToDataType<Type>(), MemoryType::SCALAR)
{
  static_assert(std::is_fundamental<Type>::value, "Scalar can only be of native type");
}

template <class Type>
auto
Scalar<Type>::Get() const -> Type
{
  return this->data_;
}

template <class Type>
auto
Scalar<Type>::Ndim() const -> unsigned int
{
  return 1;
}

template <class Type>
auto
Scalar<Type>::Shape() const -> const ShapeArray &
{
  return this->shape_;
}

template <class Type>
auto
Scalar<Type>::GetNumberOfElements() const -> size_t
{
  return 1;
}

template <class Type>
auto
Scalar<Type>::GetDataSizeOf() const -> size_t
{
  return sizeof(this->Get());
}

template <class Type>
auto
Scalar<Type>::GetMemorySize() const -> size_t
{
  return sizeof(this->Get());
}

template <class Type>
auto
Scalar<Type>::ToString() const -> std::string
{
  std::stringstream out_string;
  out_string << this->Get() << "(" << DataTypeToString(this->GetDataType()) << ")";
  return out_string.str();
}

template <class Type>
auto
operator<<(std::ostream & out, const Scalar<Type> & scalar) -> std::ostream &
{
  out << scalar.ToString();
  return out;
}

} // namespace cle

#endif // __CORE_CLESCALAR_HPP
