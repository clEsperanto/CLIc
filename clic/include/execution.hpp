#pragma once

#include "array.hpp"
#include "device.hpp"

#include <array>
#include <set>
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
 * @param orders Orders for the kernel operation (default: {0,0,0})
 */
auto
execute_separable(const Device::Pointer &      device,
                  const KernelInfo &           kernel,
                  const Array::Pointer &       src,
                  const Array::Pointer &       dst,
                  const std::array<float, 3> & sigma,
                  const std::array<int, 3> &   radius,
                  const std::array<int, 3> &   orders = { 0, 0, 0 }) -> void;

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







/**
 * @brief Evaluate a mathematical expression element-wise on GPU arrays
 *
 * Generates and executes a pure OpenCL/CUDA 1D kernel from the given expression.
 * Variable names in the expression are automatically mapped by position to the
 * parameters vector. All values (array elements and scalars) are cast to float
 * for computation, and the result is cast to the output array's data type.
 *
 * The expression uses standard C/OpenCL math syntax. Built-in math functions
 * (sin, cos, exp, pow, sqrt, fabs, fmin, fmax, etc.) are supported.
 *
 * Variable names are extracted from the expression in order of first appearance
 * and bound to parameters by index:
 *   evaluate(device, "a + b * s", {img1, img2, 2.5f}, result);
 *   // a -> img1 (index 0), b -> img2 (index 1), s -> 2.5f (index 2)
 *
 * @param device Device pointer
 * @param expression Mathematical expression string (e.g. "a + b * s")
 * @param parameters Positional list of input arrays and scalars
 * @param output Destination array (must be pre-allocated, same size as input arrays)
 */
auto
evaluate(const Device::Pointer &            device,
         const std::string &                expression,
         const std::vector<ParameterType> & parameters,
         const Array::Pointer &             output) -> void;

} // namespace cle

