#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <type_traits>
#include <unordered_map>
#include <vector>
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

  COMPLEX,

  UNKNOWN,

  INT = INT32,
  INDEX = UINT32,
  LABEL = UINT32,
  BINARY = UINT8,
};

/**
 * @brief Convert a cle::dType to a string
 */
inline auto
toString(const dType & dtype) -> std::string
{
  static const std::unordered_map<dType, std::string> dtypeToString = { { dType::FLOAT, "float" },   { dType::INT32, "int" },
                                                                        { dType::UINT32, "uint" },   { dType::INT8, "char" },
                                                                        { dType::UINT8, "uchar" },   { dType::INT16, "short" },
                                                                        { dType::UINT16, "ushort" }, { dType::COMPLEX, "float" } };

  auto it = dtypeToString.find(dtype);
  return it != dtypeToString.end() ? it->second : "unknown";
}

/**
 * @brief Convert a cle::dType to a short string
 */
inline auto
toShortString(const dType & dtype) -> std::string
{
  static const std::unordered_map<dType, std::string> dtypeToString = { { dType::FLOAT, "f" },   { dType::INT32, "i" },
                                                                        { dType::UINT32, "ui" }, { dType::INT8, "c" },
                                                                        { dType::UINT8, "uc" },  { dType::INT16, "s" },
                                                                        { dType::UINT16, "us" }, { dType::COMPLEX, "f" } };

  auto it = dtypeToString.find(dtype);
  return it != dtypeToString.end() ? it->second : "?";
}

/**
 * @brief Render a template string by replacing {KEY} placeholders with values from a map.
 * Single-pass O(n) scan, suitable for generating kernel source strings.
 */
inline auto
renderTemplate(const std::string & tmpl, const std::unordered_map<std::string, std::string> & vars) -> std::string
{
  std::string result;
  result.reserve(tmpl.size() * 2);
  for (std::size_t i = 0; i < tmpl.size();)
  {
    if (tmpl[i] == '{')
    {
      const auto end = tmpl.find('}', i + 1);
      if (end != std::string::npos)
      {
        const auto key = tmpl.substr(i + 1, end - i - 1);
        const auto it = vars.find(key);
        if (it != vars.end())
        {
          result += it->second;
          i = end + 1;
          continue;
        }
      }
    }
    result += tmpl[i++];
  }
  return result;
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
  static const std::unordered_map<dType, size_t> dtypeToBytes = { { dType::FLOAT, sizeof(float) },     { dType::INT32, sizeof(int32_t) },
                                                                  { dType::UINT32, sizeof(uint32_t) }, { dType::INT8, sizeof(int8_t) },
                                                                  { dType::UINT8, sizeof(uint8_t) },   { dType::INT16, sizeof(int16_t) },
                                                                  { dType::UINT16, sizeof(uint16_t) }, { dType::COMPLEX, sizeof(float) } };

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
  using CommonType = typename std::common_type<float, uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t>::type;

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
    std::cerr << "Error: Invalid Array::Type value, default casting into float " << std::endl;
    return static_cast<CommonType>(value);
    // throw std::invalid_argument("Invalid Array::Type value");
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


/**
 * @brief handle prime numbers
 */
auto inline handle_prime(size_t x, size_t z, std::vector<double> & a, int p) -> void
{
  double log_p = std::log(p);
  int    power = p;

  while (power <= x + z)
  {
    int j = x % power;
    if (j > 0)
    {
      j = power - j;
    }

    while (j < z)
    {
      a[j] += log_p;
      j += power;
    }

    power *= p;
  }
}

/**
 * @brief find the next smooth number
 */
auto inline next_smooth(size_t x) -> size_t
{
  size_t              z = static_cast<size_t>(10 * std::log2(x));
  double              delta = 0.000001;
  std::vector<double> a(z, 0.0);

  constexpr std::array<int, 4> primes = { 2, 3, 5, 7 };
  for (int p : primes)
  {
    handle_prime(x, z, a, p);
  }

  double log_x = std::log(x);
  for (size_t i = 0; i < a.size(); ++i)
  {
    if (a[i] >= log_x - delta)
    {
      return x + i;
    }
  }
  return std::numeric_limits<size_t>::max();
}

/**
 * @brief return the next smooth shape (power of 2) from a given shape for fft operations
 */
auto inline fft_smooth_shape(const std::array<size_t, 3> & shape) -> std::array<size_t, 3>
{
  std::array<size_t, 3> result;
  std::transform(shape.begin(), shape.end(), result.begin(), [](size_t value) { return (value > 1) ? next_smooth(value) : 1; });
  return result;
}


} // namespace cle
