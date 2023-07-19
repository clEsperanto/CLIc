#ifndef __INCLUDE_TIER4_HPP
#define __INCLUDE_TIER4_HPP

#include "array.hpp"
#include "device.hpp"
#include "execution.hpp"

namespace cle::tier4
{

auto
relabel_sequential_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, int blocksize)
  -> Array::Pointer;


} // namespace cle::tier4

#endif // __INCLUDE_TIER4_HPP
