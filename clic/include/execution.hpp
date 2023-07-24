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
using ParameterType = std::variant<Array::Pointer, const float, const int>;
using ParameterList = std::vector<std::pair<std::string, ParameterType>>;
using ConstantList = std::vector<std::pair<std::string, ConstantType>>;
using KernelInfo = std::pair<std::string, std::string>;
using RangeArray = std::array<size_t, 3>;

auto
replaceWord(std::string & sentence, const std::string_view & wordToReplace, const std::string_view & replacement)
  -> void;

auto
srcOpenclToCuda(std::string opencl_code) -> std::string;

auto
cudaDefines(const ParameterList & parameter_list, const ConstantList & constant_list) -> std::string;

auto
oclDefines(const ParameterList & parameter_list, const ConstantList & constant_list) -> std::string;

auto
execute(const Device::Pointer & device,
        const KernelInfo &      kernel_func,
        const ParameterList &   parameters,
        const RangeArray &      global_range = { 1, 1, 1 },
        const ConstantList &    constants = {}) -> void;

auto
loadSource(const std::string & source_path) -> std::string;

} // namespace cle

#endif // __INCLUDE_EXECUTION_HPP
