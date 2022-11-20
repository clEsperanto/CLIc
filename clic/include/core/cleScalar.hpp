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
  GetMemoryType_Str() const -> std::string override;
  [[nodiscard]] auto
  GetDataType_Str(const bool & short_version) const -> std::string override;
  [[nodiscard]] auto
  GetSizeOfElements() const -> size_t override;
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
  , LightObject(TypeToDataType<Type>(), SCALAR)
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
Scalar<Type>::GetMemoryType_Str() const -> std::string
{
  return "scalar";
}

template <class Type>
auto
Scalar<Type>::GetDataType_Str(const bool & short_version) const -> std::string
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

template <class Type>
auto
Scalar<Type>::GetSizeOfElements() const -> size_t
{
  return sizeof(this->Get());
}

template <class Type>
auto
Scalar<Type>::ToString() const -> std::string
{
  std::stringstream out_string;
  out_string << this->Get() << "(" << this->GetDataType_Str(false) << ")";
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
