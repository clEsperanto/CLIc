#ifndef __INCLUDE_TIER0_HPP
#define __INCLUDE_TIER0_HPP

#include "array.hpp"
#include "device.hpp"
#include "execution.hpp"
#include "transform.hpp"

/**
 * @namespace cle::tier0
 * @brief Contains all the functions of the tier 0 category.
 *
 */
namespace cle::tier0
{

/**
 * @brief Manage the creation of a destination array if it does not exist already
 * @param src Source array
 * @param dst Destination array
 * @param width Width of the destination array
 * @param height Height of the destination array
 * @param depth Depth of the destination array
 * @param type Data type of the destination array (optional)
 */
auto
create_dst(const Array::Pointer & src,
           Array::Pointer &       dst,
           size_t                 width,
           size_t                 height,
           size_t                 depth,
           dType                  type = dType::UNKNOWN) -> void;

/**
 * @brief Manage the creation of a destination array similar to the source array if it does not exist already.
 * @param src Source array
 * @param dst Destination array
 * @param type Data type of the destination array (optional)
 */
auto
create_like(const Array::Pointer & src, Array::Pointer & dst, dType type = dType::UNKNOWN) -> void;

/**
 * @brief Manage the creation of a destination array as a scalar if it does not exist already.
 * @param src Source array
 * @param dst Destination array
 * @param type Data type of the destination array (optional)
 */
auto
create_one(const Array::Pointer & src, Array::Pointer & dst, dType type = dType::UNKNOWN) -> void;

/**
 * @brief Manage the creation of a destination array as a vector if it does not exist already.
 * @param src Source array
 * @param dst Destination array
 * @param size Size of the destination array
 * @param type Data type of the destination array (optional)
 */
auto
create_vector(const Array::Pointer & src, Array::Pointer & dst, const size_t & size = 1, dType type = dType::UNKNOWN)
  -> void;

/**
 * @brief Manage the creation of a destination array from XY transposed source it does not exist already.
 * @param src Source array
 * @param dst Destination array
 * @param type Data type of the destination array (optional)
 */
auto
create_xy(const Array::Pointer & src, Array::Pointer & dst, dType type = dType::UNKNOWN) -> void;

/**
 * @brief Manage the creation of a destination array from YX transposed source it does not exist already.
 * @param src Source array
 * @param dst Destination array
 * @param type Data type of the destination array (optional)
 */
auto
create_yx(const Array::Pointer & src, Array::Pointer & dst, dType type = dType::UNKNOWN) -> void;

/**
 * @brief Manage the creation of a destination array from ZY transposed source it does not exist already.
 * @param src Source array
 * @param dst Destination array
 * @param type Data type of the destination array (optional)
 */
auto
create_zy(const Array::Pointer & src, Array::Pointer & dst, dType type = dType::UNKNOWN) -> void;

/**
 * @brief Manage the creation of a destination array from YZ transposed source it does not exist already.
 * @param src Source array
 * @param dst Destination array
 * @param type Data type of the destination array (optional)
 */
auto
create_yz(const Array::Pointer & src, Array::Pointer & dst, dType type = dType::UNKNOWN) -> void;

/**
 * @brief Manage the creation of a destination array from XZ transposed source it does not exist already.
 * @param src Source array
 * @param dst Destination array
 * @param type Data type of the destination array (optional)
 */
auto
create_xz(const Array::Pointer & src, Array::Pointer & dst, dType type = dType::UNKNOWN) -> void;

/**
 * @brief Manage the creation of a destination array from ZX transposed source it does not exist already.
 * @param src Source array
 * @param dst Destination array
 * @param type Data type of the destination array (optional)
 */
auto
create_zx(const Array::Pointer & src, Array::Pointer & dst, dType type = dType::UNKNOWN) -> void;

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
execute_separable_func(const Device::Pointer &      device,
                       const KernelInfo &           kernel,
                       const Array::Pointer &       src,
                       const Array::Pointer &       dst,
                       const std::array<float, 3> & sigma,
                       const std::array<int, 3> &   radius) -> void;

/**
 * @brief Prepare the output shape and transform for the given transformation matrix and array shape
 * @param src Source array
 * @param transform Affine transform
 * @return std::tuple<size_t, size_t, size_t, cle::AffineTransform>
 */
auto
prepare_output_shape_and_transform(const cle::Array::Pointer & src, const cle::AffineTransform & transform)
  -> std::tuple<size_t, size_t, size_t, cle::AffineTransform>;

/**
 * @brief Apply the transform matrix to an array
 * @param src Source array
 * @param dst Destination array
 * @param transform Affine transform
 * @param interpolate Interpolation flag
 * @param auto_resize Auto resize flag
 * @return cle::Array::Pointer
 */
auto
apply_affine_transform(const cle::Array::Pointer &  src,
                       cle::Array::Pointer          dst,
                       const cle::AffineTransform & transform,
                       const bool                   interpolate,
                       const bool                   auto_resize) -> cle::Array::Pointer;

} // namespace cle::tier0

#endif // __INCLUDE_TIER0_HPP
