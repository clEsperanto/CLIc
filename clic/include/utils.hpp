#ifndef __INCLUDE_UTILS_HPP
#define __INCLUDE_UTILS_HPP

#include <algorithm>
#include <cmath>
#include <fstream>
#include <limits>
#include <type_traits>
#include <unordered_map>
#ifndef M_PI
#  define M_PI 3.14159265358979323846 /* pi */
#endif

namespace cle
{

constexpr float NaN = std::numeric_limits<float>::quiet_NaN();
constexpr float pINF = std::numeric_limits<float>::infinity();
constexpr float nINF = -std::numeric_limits<float>::infinity();

/**
 * @brief mType is an enum class to represent the type of memory
 */
enum class mType
{
  BUFFER,
  IMAGE
};

/**
 * @brief dType is an enum class to represent the type of data
 */
enum class dType
{
  INT8,
  UINT8,
  INT16,
  UINT16,
  INT32,
  UINT32,
  // INT64,    // not supported by Metal
  // UINT64,   // not supported by Metal
  FLOAT,
  // DOUBLE,   // not supported by GPUs

  INT = INT32,
  INDEX = UINT32,
  LABEL = UINT32,
  BINARY = UINT8,

  UNKNOWN
};

/**
 * @brief Convert a cle::dType to a string
 */
inline auto
toString(const dType & dtype) -> std::string
{
  static const std::unordered_map<dType, std::string> dtypeToString = {
    { dType::FLOAT, "float" }, { dType::INT32, "int" },   { dType::UINT32, "uint" },  { dType::INT8, "char" },
    { dType::UINT8, "uchar" }, { dType::INT16, "short" }, { dType::UINT16, "ushort" }
  };

  auto it = dtypeToString.find(dtype);
  return it != dtypeToString.end() ? it->second : "unknown";
}

/**
 * @brief Convert a template type T to a cle::dType
 */
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
  else
  {
    throw std::invalid_argument("Error: Invalid type");
  }
}

/**
 * @brief return the size in bytes of a cle::dType
 */
inline auto
toBytes(const dType & dtype) -> size_t
{
  static const std::unordered_map<dType, size_t> dtypeToBytes = {
    { dType::FLOAT, sizeof(float) },    { dType::INT32, sizeof(int32_t) }, { dType::UINT32, sizeof(uint32_t) },
    { dType::INT8, sizeof(int8_t) },    { dType::UINT8, sizeof(uint8_t) }, { dType::INT16, sizeof(int16_t) },
    { dType::UINT16, sizeof(uint16_t) }
  };

  auto it = dtypeToBytes.find(dtype);
  if (it != dtypeToBytes.end())
  {
    return it->second;
  }
  else
  {
    throw std::invalid_argument("Invalid Array::Type value");
  }
}

/**
 * @brief cast a value to a cle::dType value
 */
template <typename T>
inline auto
castTo(const T & value, const dType & dtype) ->
  typename std::common_type<float, uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t>::type
{
  using CommonType =
    typename std::common_type<float, uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t>::type;

  if (dtype == dType::FLOAT)
  {
    return static_cast<CommonType>(value);
  }
  else if (dtype == dType::INT32)
  {
    return static_cast<CommonType>(value);
  }
  else if (dtype == dType::UINT32)
  {
    return static_cast<CommonType>(value);
  }
  else if (dtype == dType::INT8)
  {
    return static_cast<CommonType>(value);
  }
  else if (dtype == dType::UINT8)
  {
    return static_cast<CommonType>(value);
  }
  else if (dtype == dType::INT16)
  {
    return static_cast<CommonType>(value);
  }
  else if (dtype == dType::UINT16)
  {
    return static_cast<CommonType>(value);
  }
  else
  {
    throw std::invalid_argument("Invalid Array::Type value");
  }
}


/**
 * @brief compute a kernel size from a sigma value
 */
inline auto
sigma2kernelsize(const float & sigma) -> int
{
  auto rad = static_cast<int>(sigma * 8.0);
  return (rad % 2 == 0) ? rad + 1 : rad;
}

/**
 * @brief compute a kernel size from a radius value
 */
inline auto
radius2kernelsize(const float & radius) -> int
{
  return static_cast<int>(radius * 2 + 1);
}

/**
 * @brief load a file into a string
 */
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

/**
 * @brief save a string into a file
 */
inline auto
saveFile(const std::string & file_path, const std::string & source) -> void
{
  std::ofstream ofs(file_path);
  if (!ofs.is_open())
  {
    throw std::runtime_error("Error: Failed to open file: " + file_path);
  }
  ofs << source;
  ofs.close();
}

/**
 * @brief compute the dimension from a shape (width, height, depth)
 */
inline auto
shape_to_dimension(const size_t & width, const size_t & height, const size_t & depth) -> size_t
{
  if (depth > 1)
  {
    return 3;
  }
  else if (height > 1)
  {
    return 2;
  }
  else
  {
    return 1;
  }
}

/**
 * @brief correct the range of a start, stop and step based on the size of an array
 */
inline auto
correct_range(int * start, int * stop, int * step, int size) -> void
{
  // # set in case not set (passed None)
  if (step == nullptr)
  {
    *step = 1;
  }
  if (start == nullptr)
  {
    *start = (*step >= 0) ? 0 : size - 1;
  }
  if (stop == nullptr)
  {
    *stop = (*step >= 0) ? size : -1;
  }
  // # Check if ranges make sense
  if (*start >= size)
  {
    *start = (*step >= 0) ? size : size - 1;
  }
  if (*start < -size + 1)
  {
    *start = -size + 1;
  }
  if (*stop > size)
  {
    *stop = size;
  }
  if (*stop < -size)
  {
    *stop = (*start > 0) ? 0 - 1 : -size;
  }
  if (*start < 0)
  {
    *start = size - *start;
  }
  if ((*start > *stop && *step > 0) || (*start < *stop && *step < 0))
  {
    *stop = *start;
  }
}

/**
 * @brief Convert a cle::mType to a string
 */
inline auto
toString(const mType & mtype) -> std::string
{
  switch (mtype)
  {
    case mType::BUFFER:
      return "Buffer";
    case mType::IMAGE:
      return "Image";
    default:
      return "unknown";
  }
}

/**
 * @brief Operator << for cle::dType
 */
inline auto
operator<<(std::ostream & out, const dType & dtype) -> std::ostream &
{
  return out << toString(dtype);
}

/**
 * @brief Operator << for cle::mType
 */
inline auto
operator<<(std::ostream & out, const mType & mtype) -> std::ostream &
{
  return out << toString(mtype);
}


/**
 * @brief convert a string to lower case
 */
inline auto
to_lower(const std::string & str) -> std::string
{
  std::string result = str;
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result;
}

} // namespace cle

#endif // __INCLUDE_UTILS_HPP
