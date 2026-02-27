#pragma once

#include "array.hpp"
#include "device.hpp"
#include "execution.hpp"

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
create_dst(const Array::Pointer & src, Array::Pointer & dst, size_t width, size_t height, size_t depth, dType type = dType::UNKNOWN)
  -> void;

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
create_vector(const Array::Pointer & src, Array::Pointer & dst, const size_t & size = 1, dType type = dType::UNKNOWN) -> void;

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


} // namespace cle::tier0
