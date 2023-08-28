#ifndef __INCLUDE_TIER0_HPP
#define __INCLUDE_TIER0_HPP

#include "array.hpp"
#include "device.hpp"
#include "execution.hpp"

namespace cle::tier0
{

auto
create_dst(const Array::Pointer & src,
           Array::Pointer &       dst,
           size_t                 width,
           size_t                 height,
           size_t                 depth,
           dType                  type = dType::UNKNOWN) -> void;

auto
create_like(const Array::Pointer & src, Array::Pointer & dst, dType type = dType::UNKNOWN) -> void;

auto
create_one(const Array::Pointer & src, Array::Pointer & dst, dType type = dType::UNKNOWN) -> void;

auto
create_vector(const Array::Pointer & src, Array::Pointer & dst, const size_t & size = 1, dType type = dType::UNKNOWN)
  -> void;

auto
create_xy(const Array::Pointer & src, Array::Pointer & dst, dType type = dType::UNKNOWN) -> void;

auto
create_zy(const Array::Pointer & src, Array::Pointer & dst, dType type = dType::UNKNOWN) -> void;

auto
create_xz(const Array::Pointer & src, Array::Pointer & dst, dType type = dType::UNKNOWN) -> void;

auto
execute_separable_func(const Device::Pointer &      device,
                       const KernelInfo &           kernel,
                       const Array::Pointer &       src,
                       const Array::Pointer &       dst,
                       const std::array<float, 3> & sigma,
                       const std::array<int, 3> &   radius) -> void;


} // namespace cle::tier0

#endif // __INCLUDE_TIER0_HPP
