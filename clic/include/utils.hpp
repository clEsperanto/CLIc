#ifndef __INCLUDE_UTILS_HPP
#define __INCLUDE_UTILS_HPP

#include <cstdint>
#include <fstream>
#include <iostream>
#include <istream>
#include <limits>
#include <string>

#include <cmath>
#ifndef M_PI
#  define M_PI 3.14159265358979323846 /* pi */
#endif

namespace cle
{

constexpr float NaN = std::numeric_limits<float>::quiet_NaN();
constexpr float pINF = std::numeric_limits<float>::infinity();
constexpr float nINF = -std::numeric_limits<float>::infinity();

enum class mType
{
  BUFFER,
  IMAGE
};

enum class dType
{
  FLOAT,
  INT32,
  UINT32,
  INT8,
  UINT8,
  INT16,
  UINT16,
  INT64,
  UINT64,
  UNKNOWN
};

inline auto
operator<<(std::ostream & out, const dType & dtype) -> std::ostream &
{
  switch (dtype)
  {
    case dType::FLOAT:
      out << "float";
      break;
    case dType::INT32:
      out << "int";
      break;
    case dType::UINT32:
      out << "uint";
      break;
    case dType::INT8:
      out << "char";
      break;
    case dType::UINT8:
      out << "uchar";
      break;
    case dType::INT16:
      out << "short";
      break;
    case dType::UINT16:
      out << "ushort";
      break;
    case dType::INT64:
      out << "long";
      break;
    case dType::UINT64:
      out << "ulong";
      break;
    default:
      out << "unknown";
      break;
  }
  return out;
}

inline auto
operator<<(std::ostream & out, const mType & mtype) -> std::ostream &
{
  switch (mtype)
  {
    case mType::BUFFER:
      out << "Buffer";
      break;
    case mType::IMAGE:
      out << "Image";
      break;
  }
  return out;
}

template <typename T>
inline auto
toType() -> dType
{
  if constexpr (std::is_same_v<T, float>)
  {
    return dType::FLOAT;
  }
  else if constexpr (std::is_same_v<T, int32_t>)
  {
    return dType::INT32;
  }
  else if constexpr (std::is_same_v<T, uint32_t>)
  {
    return dType::UINT32;
  }
  else if constexpr (std::is_same_v<T, int16_t>)
  {
    return dType::INT16;
  }
  else if constexpr (std::is_same_v<T, uint16_t>)
  {
    return dType::UINT16;
  }
  else if constexpr (std::is_same_v<T, int8_t>)
  {
    return dType::INT8;
  }
  else if constexpr (std::is_same_v<T, uint8_t>)
  {
    return dType::UINT8;
  }
  else if constexpr (std::is_same_v<T, int64_t>)
  {
    return dType::INT64;
  }
  else if constexpr (std::is_same_v<T, uint64_t>)
  {
    return dType::UINT64;
  }
  else
  {
    throw std::invalid_argument("Error: Invalid type");
  }
}

inline auto
toBytes(const dType & dtype) -> size_t
{
  switch (dtype)
  {
    case dType::FLOAT:
      return sizeof(float);
    case dType::INT32:
      return sizeof(int32_t);
    case dType::UINT32:
      return sizeof(uint32_t);
    case dType::INT8:
      return sizeof(int8_t);
    case dType::UINT8:
      return sizeof(uint8_t);
    case dType::INT16:
      return sizeof(int16_t);
    case dType::UINT16:
      return sizeof(uint16_t);
    case dType::INT64:
      return sizeof(int64_t);
    case dType::UINT64:
      return sizeof(uint64_t);
    default:
      throw std::invalid_argument("Invalid Array::Type value");
  }
}

template <typename T>
inline auto
castTo(const T & value, const dType & dtype) ->
  typename std::common_type<float, uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t>::type
{
  switch (dtype)
  {
    case dType::FLOAT:
      return static_cast<float>(value);
    case dType::INT32:
      return static_cast<int32_t>(value);
    case dType::UINT32:
      return static_cast<uint32_t>(value);
    case dType::INT8:
      return static_cast<int8_t>(value);
    case dType::UINT8:
      return static_cast<uint8_t>(value);
    case dType::INT16:
      return static_cast<int16_t>(value);
    case dType::UINT16:
      return static_cast<uint16_t>(value);
    case dType::INT64:
      return static_cast<int64_t>(value);
    case dType::UINT64:
      return static_cast<uint64_t>(value);
    default:
      throw std::invalid_argument("Invalid Array::Type value");
  }
}

inline auto
sigma2kernelsize(const float & sigma) -> int
{
  auto rad = static_cast<int>(sigma * 8.0);
  return (rad % 2 == 0) ? rad + 1 : rad;
}

inline auto
radius2kernelsize(const int & radius) -> int
{
  return static_cast<int>(radius * 2 + 1);
}

inline auto
loadFile(const std::string & file_path) -> std::string
{
  std::ifstream ifs(file_path);
  if (!ifs.is_open())
  {
    throw std::runtime_error("Error: Failed to open file: " + file_path);
  }
  std::string source((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
  ifs.close();
  return source;
}

inline auto
saveFile(std::string & file_path, std::string & source) -> void
{
  std::ofstream ofs(file_path);
  if (!ofs.is_open())
  {
    throw std::runtime_error("Error: Failed to open file: " + file_path);
  }
  ofs << source;
  ofs.close();
}

inline auto
correct_range(int * start, int * stop, int * step, size_t size) -> void
{
  // # set in case not set (passed None)
  if (step == nullptr)
    *step = 1;
  if (start == nullptr)
    *start = (*step >= 0) ? 0 : size - 1;
  if (stop == nullptr)
    *stop = (*step >= 0) ? size : -1;
  // # Check if ranges make sense
  if (*start >= size)
    *start = (*step >= 0) ? size : size - 1;
  if (*start < -size + 1)
    *start = -size + 1;
  if (*stop > size)
    *stop = size;
  if (*stop < -size)
    *stop = (*start > 0) ? 0 - 1 : -size;
  if (*start < 0)
    *start = size - *start;
  if ((*start > *stop && *step > 0) || (*start < *stop && *step < 0))
    *stop = *start;
}


} // namespace cle

#endif // __INCLUDE_UTILS_HPP
