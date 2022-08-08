#ifndef __CORE_CLESCALAR_HPP
#define __CORE_CLESCALAR_HPP

#include "cleLightObject.hpp"

namespace cle
{

template <class Type>
class Scalar : public LightObject
{
public:
  Scalar() = delete;
  ~Scalar() = default;
  Scalar(const Scalar & obj) = default;
  Scalar(Scalar && obj) = default;
  auto
  operator=(const Scalar & obj) -> Scalar & = default;
  auto
  operator=(Scalar && obj) -> Scalar & = default;
  explicit Scalar(const Type & data);

  [[nodiscard]] auto
  Get() const -> Type;
  [[nodiscard]] auto
  Ndim() const -> unsigned int override;
  [[nodiscard]] auto
  Shape() const -> ShapeArray override;
  [[nodiscard]] auto
  MemoryInfo() const -> std::string override;
  [[nodiscard]] auto
  DataInfo() const -> std::string override;
  [[nodiscard]] auto
  DataInfoShort() const -> std::string override;
  [[nodiscard]] auto
  Bytes() const -> size_t override;
  [[nodiscard]] auto
  ToString() const -> std::string override;

private:
  Type data_;
};

template <class Type>
Scalar<Type>::Scalar(const Type & data)
  : data_(data)
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
Scalar<Type>::Shape() const -> ShapeArray
{
  return { 1, 1, 1 };
}

template <class Type>
auto
Scalar<Type>::MemoryInfo() const -> std::string
{
  return "scalar";
}

template <class Type>
auto
Scalar<Type>::DataInfo() const -> std::string
{
  return typeid(this->Get()).name();
}

template <class Type>
auto
Scalar<Type>::DataInfoShort() const -> std::string
{
  return typeid(this->Get()).name();
}

template <class Type>
auto
Scalar<Type>::Bytes() const -> size_t
{
  return sizeof(this->Get());
}

template <class Type>
auto
Scalar<Type>::ToString() const -> std::string
{
  std::string str = this->MemoryInfo() + "(" + this->DataInfo() + ")";
  str += " of shape=[" + std::to_string(this->Shape()[0]) + "," + std::to_string(this->Shape()[1]) + "," +
         std::to_string(this->Shape()[2]) + "]";
  return str;
}

} // namespace cle

#endif // __CORE_CLESCALAR_HPP
