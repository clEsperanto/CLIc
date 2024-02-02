#ifndef __INCLUDE_MEMORY_HPP
#define __INCLUDE_MEMORY_HPP

#include "array.hpp"
#include "device.hpp"
#include "utils.hpp"

namespace cle::memory
{

/**
 * @brief Create a new array on the device
 * @param width Width of the array
 * @param height Height of the array
 * @param depth Depth of the array
 * @param device Device pointer on which the array will be created
 * @return Array
 */
template <typename T>
static auto
create(const size_t & width, const size_t & height, const size_t & depth, const Device::Pointer & device) -> Array
{
  return Array{ width, height, depth, toType<T>(), mType::BUFFER, device };
}

/**
 * @brief Create a new array on the device using the array as a reference
 * @param arr Array reference
 * @return Array
 */
static auto
create_like(const Array & arr) -> Array
{
  return Array{ arr };
}

/**
 * @brief Create a new 2D array on the device using the array as a reference
 * @param arr Array reference
 * @return Array
 */
static auto
create_2d_xy(const Array & arr) -> Array
{
  return Array{ arr.width(), arr.height(), 1, arr.dtype(), mType::BUFFER, arr.device() };
}

/**
 * @brief Create a new 2D array on the device using the array as a reference
 * @param arr Array reference
 * @return Array
 */
static auto
create_2d_zy(const Array & arr) -> Array
{
  return Array{ arr.depth(), arr.height(), 1, arr.dtype(), mType::BUFFER, arr.device() };
}

/**
 * @brief Create a new 2D array on the device using the array as a reference
 * @param arr Array reference
 * @return Array
 */
static auto
create_2d_xz(const Array & arr) -> Array
{
  return Array{ arr.width(), arr.depth(), 1, arr.dtype(), mType::BUFFER, arr.device() };
}

/**
 * @brief Push a host array to the device
 * @param host_data Host array
 * @param width Width of the array
 * @param height Height of the array
 * @param depth Depth of the array
 * @param device Device pointer on which the array will be created
 * @return Array
 */
template <typename T>
static auto
push(const T *               host_data,
     const size_t &          width,
     const size_t &          height,
     const size_t &          depth,
     const Device::Pointer & device) -> Array
{
  return Array{ width, height, depth, toType<T>(), mType::BUFFER, host_data, device };
}

/**
 * @brief Pull an array from the device to the host
 * @param arr Array
 * @param host_arr Host array
 */
template <typename T>
static auto
pull(const Array & arr, T * host_arr) -> void
{
  arr.read(host_arr);
}

/**
 * @brief Copy an array on the device to another array on the device
 * @param src Source array
 * @param dst Destination array
 */
template <typename T>
static auto
copy(const Array & src) -> Array
{
  Array dst{ src.width(), src.height(), src.depth(), toType<T>(), mType::BUFFER, src.device() };
  src.copy(dst);
  return dst;
}

} // namespace cle::memory

#endif // __INCLUDE_MEMORY_HPP
