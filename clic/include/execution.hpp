#ifndef __INCLUDE_EXECUTION_HPP
#define __INCLUDE_EXECUTION_HPP

#include "array.hpp"
#include "device.hpp"

#include <array>
#include <variant>
#include <vector>

namespace cle
{

using ConstantType = int;
using ParameterType = std::variant<Array::Pointer, float, int>;
using ParameterList = std::vector<std::pair<std::string, ParameterType>>;
using ConstantList = std::vector<std::pair<std::string, ConstantType>>;
using KernelInfo = std::pair<std::string, std::string>;
using RangeArray = std::array<size_t, 3>;

auto
execute(const Device::Pointer & device,
        const KernelInfo &      kernel_func,
        const ParameterList &   parameters,
        const RangeArray &      global_range = { 1, 1, 1 },
        const ConstantList &    constants = {}) -> void;

auto
native_execute(const Device::Pointer & device,
               const KernelInfo &      kernel_func,
               const ParameterList &   parameters,
               const RangeArray &      global_range = { 1, 1, 1 },
               const RangeArray &      local_range = { 1, 1, 1 }) -> void;

} // namespace cle

#endif // __INCLUDE_EXECUTION_HPP
