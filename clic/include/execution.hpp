#ifndef __INCLUDE_EXECUTION_HPP
#define __INCLUDE_EXECUTION_HPP

#include "array.hpp"
#include "device.hpp"

#include <array>
#include <variant>
#include <vector>

namespace cle
{

using ConstantType = std::variant<int, float, std::string>;
using ParameterType = std::variant<Array::Pointer, float, int, unsigned int, size_t>;
using ParameterList = std::vector<std::pair<std::string, ParameterType>>;
using ConstantList = std::vector<std::pair<std::string, ConstantType>>;
using KernelInfo = std::pair<std::string, std::string>;
using RangeArray = std::array<size_t, 3>;

/**
 * @brief Execute a kernel using CLIJ code
 * @param device Device pointer
 * @param kernel_func Kernel function name and code
 * @param parameters List of parameters to be passed to the kernel
 * @param global_range Global range of the kernel
 * @param local_range Local range of the kernel
 * @param constants List of constants to be defined in the kernel
 */
auto
execute(const Device::Pointer & device,
        const KernelInfo &      kernel_func,
        const ParameterList &   parameters,
        const RangeArray &      global_range = { 1, 1, 1 },
        const RangeArray &      local_range = { 0, 0, 0 },
        const ConstantList &    constants = {}) -> void;


/**
 * @brief Execution function for separable kernel operation
 * @param device Device pointer
 * @param kernel Kernel function name and code
 * @param src Source array
 * @param dst Destination array
 * @param sigma Sigma value for the kernel
 * @param radius Radius value for the kernel
 */
auto
execute_separable(const Device::Pointer &      device,
                  const KernelInfo &           kernel,
                  const Array::Pointer &       src,
                  const Array::Pointer &       dst,
                  const std::array<float, 3> & sigma,
                  const std::array<int, 3> &   radius) -> void;

/**
 * @brief Execution function for separable kernel operation (with with extra parameters)
 *
 * This is currently only used for Gaussian derivative kernels.
 * to be fused / generalized with the previous function in future
 *
 * @param device Device pointer
 * @param kernel Kernel function name and code
 * @param src Source array
 * @param dst Destination array
 * @param sigma Sigma value for the kernel
 * @param radius Radius value for the kernel
 * @param orders Orders for the kernel operation
 */
auto
execute_separable(const Device::Pointer &      device,
                  const KernelInfo &           kernel,
                  const Array::Pointer &       src,
                  const Array::Pointer &       dst,
                  const std::array<float, 3> & sigma,
                  const std::array<int, 3> &   radius,
                  const std::array<int, 3> &   orders) -> void;

/**
 * @brief Execute a kernel using native OpenCL code
 * @param device Device pointer
 * @param kernel_func Kernel function name and code
 * @param parameters List of parameters to be passed to the kernel
 * @param global_range Global range of the kernel
 * @param local_range Local range of the kernel
 */
auto
native_execute(const Device::Pointer & device,
               const KernelInfo &      kernel_func,
               const ParameterList &   parameters,
               const RangeArray &      global_range = { 1, 1, 1 },
               const RangeArray &      local_range = { 0, 0, 0 }) -> void;

} // namespace cle

#endif // __INCLUDE_EXECUTION_HPP
