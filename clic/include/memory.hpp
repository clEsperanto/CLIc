#ifndef __INCLUDE_MEMORY_HPP
#define __INCLUDE_MEMORY_HPP

#include "array.hpp"
#include "device.hpp"
#include "utils.hpp"

namespace cle::memory
{

template <typename T>
static auto
create(const size_t & width, const size_t & height, const size_t & depth, const Device::Pointer & device) -> Array
{
  return Array{ width, height, depth, toType<T>(), mType::BUFFER, device };
}

static auto
create_like(const Array & arr) -> Array
{
  return Array{ arr };
}

static auto
create_2d_xy(const Array & arr) -> Array
{
  return Array{ arr.width(), arr.height(), 1, arr.dtype(), mType::BUFFER, arr.device() };
}

static auto
create_2d_zy(const Array & arr) -> Array
{
  return Array{ arr.depth(), arr.height(), 1, arr.dtype(), mType::BUFFER, arr.device() };
}

static auto
create_2d_xz(const Array & arr) -> Array
{
  return Array{ arr.width(), arr.depth(), 1, arr.dtype(), mType::BUFFER, arr.device() };
}

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

template <typename T>
static auto
pull(const Array & arr, T * host_arr) -> void
{
  arr.read(host_arr);
}

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
