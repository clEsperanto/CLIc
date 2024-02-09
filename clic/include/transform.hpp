#ifndef __INCLUDE_TRANSFORM_HPP
#define __INCLUDE_TRANSFORM_HPP

#include <array>

#include "Eigen/Dense"
#include "array.hpp"
#include "execution.hpp"

#include "cle_affine_transform.h"
#include "cle_affine_transform_interpolate.h"

namespace cle
{

/**
 * @brief Affine transformation class
 *      This class is used to create an affine transformation matrix by applying different transformations
 *      such as scaling, rotation, translation, shearing, deskewing, etc. to the AffineTransform matrix.
 *      The computed matrix is meant to be used to transform an image or a volume using the affine_transform kernel.
 *      The class rely on the Eigen library [1] to perform the matrix operations.
 *
 * @link https://eigen.tuxfamily.org
 */
class AffineTransform
{
public:
  using matrix = Eigen::Matrix4f;

  /**
   * @brief Construct a new Affine Transform object
   */
  AffineTransform()
  {
    m_matrix = matrix::Identity();
    update();
  }

  /**
   * @brief Construct a new Affine Transform object from a 4x4 matrix
   * @param array Array of 16 floats representing a 4x4 matrix
   */
  AffineTransform(const std::array<float, 16> & array)
  {
    m_matrix = matrix::Map(array.data());
    update();
  }

  /**
   * @brief Destructor
   */
  ~AffineTransform() = default;

  /**
   * @brief Scaling transformation
   * @param scale_x Scaling factor for the x axis
   * @param scale_y Scaling factor for the y axis
   * @param scale_z Scaling factor for the z axis
   */
  auto
  scale(float scale_x, float scale_y, float scale_z) -> void
  {
    matrix scale_matrix = matrix::Identity();
    scale_matrix(0, 0) = scale_x;
    scale_matrix(1, 1) = scale_y;
    scale_matrix(2, 2) = scale_z;
    concats(scale_matrix);
  }

  /**
   * @brief Rotation transformation
   * @param axis Axis of rotation (0: x, 1: y, 2: z)
   * @param angle_deg Angle of rotation (in degrees)
   */
  auto
  rotate(int axis, float angle_deg) -> void
  {
    // create a rotation matrix
    matrix rotation_matrix = matrix::Identity();

    // compute rotation values
    const float angle_rad = deg_to_rad(angle_deg);
    const float angle_cos = (std::abs(std::cos(angle_rad)) < 1e-6) ? 0 : std::cos(angle_rad);
    const float angle_sin = (std::abs(std::sin(angle_rad)) < 1e-6) ? 0 : std::sin(angle_rad);

    // populate the rotation matrix based on the axis
    switch (axis)
    {
      case 0: { // x axis
        rotation_matrix(1, 1) = angle_cos;
        rotation_matrix(1, 2) = -angle_sin;
        rotation_matrix(2, 1) = angle_sin;
        rotation_matrix(2, 2) = angle_cos;
        break;
      }
      case 1: { // y axis
        rotation_matrix(0, 0) = angle_cos;
        rotation_matrix(0, 2) = angle_sin;
        rotation_matrix(2, 0) = -angle_sin;
        rotation_matrix(2, 2) = angle_cos;
        break;
      }
      case 2: { // z axis
        rotation_matrix(0, 0) = angle_cos;
        rotation_matrix(0, 1) = -angle_sin;
        rotation_matrix(1, 0) = angle_sin;
        rotation_matrix(1, 1) = angle_cos;
        break;
      }
      default:
        throw std::runtime_error("Invalid axis");
    }

    // add the rotation to the transform matrix
    concats(rotation_matrix);
  }

  /**
   * @brief Rotation transformation around the x axis
   * @param angle_deg Angle of rotation (in degrees)
   */
  auto
  rotate_around_x_axis(float angle_deg) -> void
  {
    rotate(0, angle_deg);
  }

  /**
   * @brief Rotation transformation around the y axis
   * @param angle_deg Angle of rotation (in degrees)
   */
  auto
  rotate_around_y_axis(float angle_deg) -> void
  {
    rotate(1, angle_deg);
  }

  /**
   * @brief Rotation transformation around the z axis
   * @param angle_deg Angle of rotation (in degrees)
   */
  auto
  rotate_around_z_axis(float angle_deg) -> void
  {
    rotate(2, angle_deg);
  }

  /**
   * @brief Translation transformation
   * @param translate_x Translation value for the x axis
   * @param translate_y Translation value for the y axis
   * @param translate_z Translation value for the z axis
   */
  auto
  translate(float translate_x, float translate_y, float translate_z) -> void
  {
    matrix translation_matrix = matrix::Identity();
    translation_matrix(0, 3) = translate_x;
    translation_matrix(1, 3) = translate_y;
    translation_matrix(2, 3) = translate_z;
    concats(translation_matrix);
  }

  /**
   * @brief Centering transformation
   * @param shape Shape of the array to center
   * @param undo Undo flag to apply the reverse transformation
   */
  auto
  center(const std::array<size_t, 3> & shape, bool undo)
  {
    int         presign = (undo) ? 1 : -1;
    const float centering_x = (shape[0] != 1) ? presign * (shape[0] / 2.0) : 0;
    const float centering_y = (shape[1] != 1) ? presign * (shape[1] / 2.0) : 0;
    const float centering_z = (shape[2] != 1) ? presign * (shape[2] / 2.0) : 0;
    translate(centering_x, centering_y, centering_z);
  }

  /**
   * @brief Shearing transformation in the z plane
   * @param shear_x_deg Shear angle in the x axis (in degrees)
   * @param shear_y_deg Shear angle in the y axis (in degrees)
   */
  auto
  shear_in_z_plane(float shear_x_deg, float shear_y_deg) -> void
  {
    // check that the shear angles are between range -90 and 90
    if (shear_x_deg < -90.0 || shear_x_deg > 90.0)
    {
      throw std::runtime_error("Shear X angle must be between -90 and 90 degrees");
    }
    if (shear_y_deg < -90.0 || shear_y_deg > 90.0)
    {
      throw std::runtime_error("Shear Y angle must be between -90 and 90 degrees");
    }

    const float shear_xz = shear_angle_to_shear_factor(shear_x_deg);
    const float shear_yz = shear_angle_to_shear_factor(shear_y_deg);

    // add a shear in the z plane to the matrix
    matrix shear_matrix = matrix::Identity();
    shear_matrix(0, 1) = shear_xz;
    shear_matrix(1, 0) = shear_yz;
    pre_concats(shear_matrix);
  }

  /**
   * @brief Shearing transformation in the y plane
   * @param shear_x_deg Shear angle in the x axis (in degrees)
   * @param shear_z_deg Shear angle in the z axis (in degrees)
   */
  auto
  shear_in_y_plane(float shear_x_deg, float shear_z_deg) -> void
  {
    // check that the shear angles are between range -90 and 90
    if (shear_x_deg < -90.0 || shear_x_deg > 90.0)
    {
      throw std::runtime_error("Shear X angle must be between -90 and 90 degrees");
    }
    if (shear_z_deg < -90.0 || shear_z_deg > 90.0)
    {
      throw std::runtime_error("Shear Z angle must be between -90 and 90 degrees");
    }

    const float shear_xy = shear_angle_to_shear_factor(shear_x_deg);
    const float shear_zy = shear_angle_to_shear_factor(shear_z_deg);

    // add a shear in the y plane to the matrix
    matrix shear_matrix = matrix::Identity();
    shear_matrix(0, 2) = shear_xy;
    shear_matrix(2, 0) = shear_zy;
    pre_concats(shear_matrix);
  }

  /**
   * @brief Shearing transformation in the x plane
   * @param shear_y_deg Shear angle in the y axis (in degrees)
   * @param shear_z_deg Shear angle in the z axis (in degrees)
   */
  auto
  shear_in_x_plane(float shear_y_deg, float shear_z_deg) -> void
  {
    // check that the shear angles are between range -90 and 90
    if (shear_y_deg < -90.0 || shear_y_deg > 90.0)
    {
      throw std::runtime_error("Shear Y angle must be between -90 and 90 degrees");
    }
    if (shear_z_deg < -90.0 || shear_z_deg > 90.0)
    {
      throw std::runtime_error("Shear Z angle must be between -90 and 90 degrees");
    }

    const float shear_yx = shear_angle_to_shear_factor(shear_y_deg);
    const float shear_zx = shear_angle_to_shear_factor(shear_z_deg);

    // add a shear in the x plane to the matrix
    matrix shear_matrix = matrix::Identity();
    shear_matrix(1, 2) = shear_yx;
    shear_matrix(2, 1) = shear_zx;
    pre_concats(shear_matrix);
  }

  /**
   * @brief Deskewing transformation in the x plane
   * @param angle_deg Angle of deskewing (in degrees)
   * @param voxel_size_x Voxel size in the x axis
   * @param voxel_size_y Voxel size in the y axis
   * @param voxel_size_z Voxel size in the z axis
   * @param scale_factor Scaling factor
   */
  auto
  deskew_x(float angle_deg, float voxel_size_x, float voxel_size_y, float voxel_size_z, float scale_factor) -> void
  {
    // define shear factor (different computation than the methods below)
    float shear_factor = std::sin((90 - angle_deg) * static_cast<float>(M_PI / 180.0)) * (voxel_size_z / voxel_size_y);
    m_matrix(0, 2) += shear_factor;

    // make voxels isotropic, calculate the new scaling factor for Z after shearing
    // https://github.com/tlambert03/napari-ndtiffs/blob/092acbd92bfdbf3ecb1eb9c7fc146411ad9e6aae/napari_ndtiffs/affine.py#L57
    auto new_dz = std::sin(angle_deg * static_cast<float>(M_PI / 180.0)) * voxel_size_z;
    auto scale_factor_z = (new_dz / voxel_size_y) * scale_factor;

    scale(scale_factor, scale_factor, scale_factor_z);

    // correct orientation so that the new Z-plane goes proximal-distal from the objective.
    rotate(2, -angle_deg);
  }

  /**
   * @brief Deskewing transformation in the y plane
   * @param angle_deg Angle of deskewing (in degrees)
   * @param voxel_size_x Voxel size in the x axis
   * @param voxel_size_y Voxel size in the y axis
   * @param voxel_size_z Voxel size in the z axis
   * @param scale_factor Scaling factor
   */
  auto
  deskew_y(float angle_deg, float voxel_size_x, float voxel_size_y, float voxel_size_z, float scale_factor) -> void
  {
    // define shear factor (different computation than the methods below)
    float shear_factor = std::sin((90 - angle_deg) * static_cast<float>(M_PI / 180.0)) * (voxel_size_z / voxel_size_y);
    m_matrix(1, 2) += shear_factor;

    // make voxels isotropic, calculate the new scaling factor for Z after shearing
    // https://github.com/tlambert03/napari-ndtiffs/blob/092acbd92bfdbf3ecb1eb9c7fc146411ad9e6aae/napari_ndtiffs/affine.py#L57
    auto new_dz = std::sin(angle_deg * static_cast<float>(M_PI / 180.0)) * voxel_size_z;
    auto scale_factor_z = (new_dz / voxel_size_x) * scale_factor;

    scale(scale_factor, scale_factor, scale_factor_z);

    // correct orientation so that the new Z-plane goes proximal-distal from the objective.
    rotate(1, angle_deg);
  }

  /**
   * @brief return the transformation matrix
   */
  auto
  getMatrix() const -> const Eigen::Matrix4f &
  {
    return m_matrix;
  }

  /**
   * @brief return the inverted transformation matrix
   */
  auto
  getInverse() const -> const Eigen::Matrix4f &
  {
    return m_inverse;
  }

  /**
   * @brief return the transposed transformation matrix
   */
  auto
  getTranspose() const -> const Eigen::Matrix4f &
  {
    return m_transpose;
  }

  /**
   * @brief return the inverse transposed transformation matrix
   */
  auto
  getInverseTranspose() const -> const Eigen::Matrix4f &
  {
    return m_inverse_transpose;
  }

  /**
   * @brief Convert the transformation matrix to an array of 16 floats
   */
  static auto
  toArray(const matrix & mat) -> std::array<float, 16>
  {
    std::array<float, 16> array;
    std::copy(mat.data(), mat.data() + 16, array.begin());
    return array;
  }

protected:
  /**
   * @brief Convert degrees to radians
   */
  static auto
  deg_to_rad(float angle_deg) -> float
  {
    return angle_deg * static_cast<float>(M_PI / 180.0);
  }

  /**
   * @brief Convert shear angle to shear factor
   */
  static auto
  shear_angle_to_shear_factor(float angle_deg) -> float
  {
    return 1.0F / std::tan((90 - angle_deg) * M_PI / 180);
  }

  /**
   * @brief Concatenate a matrix to the transformation matrix
   */
  auto
  concats(const matrix & mat) -> void
  {
    m_matrix = mat * m_matrix;
    update();
  }

  /**
   * @brief Concatenate the transformation matrix to a matrix
   */
  auto
  pre_concats(const matrix & mat) -> void
  {
    m_matrix = m_matrix * mat;
    update();
  }

private:
  matrix m_matrix;
  matrix m_inverse;
  matrix m_inverse_transpose;
  matrix m_transpose;

  /**
   * @brief Update the matrix and its derived matrices
   */
  auto
  update() -> void
  {
    m_inverse = m_matrix.inverse();
    m_inverse_transpose = m_inverse.transpose();
    m_transpose = m_matrix.transpose();
  }
};

/**
 * @brief Prepare the output shape and transform for the given transformation matrix and array shape
 * @param src Source array
 * @param transform Affine transform
 * @return std::tuple<size_t, size_t, size_t, cle::AffineTransform>
 */
auto
prepare_output_shape_and_transform(const cle::Array::Pointer & src, const cle::AffineTransform & transform)
  -> std::tuple<size_t, size_t, size_t, cle::AffineTransform>
{
  using point = Eigen::Vector4f;
  using bounding_box = std::array<point, 8>;

  bounding_box bbox = {
    point{ 0.0F, 0.0F, 0.0F, 1.0F },
    point{ 0.0F, 0.0F, static_cast<float>(src->depth()), 1.0F },
    point{ 0.0F, static_cast<float>(src->height()), 0.0F, 1.0F },
    point{ 0.0F, static_cast<float>(src->height()), static_cast<float>(src->depth()), 1.0F },
    point{ static_cast<float>(src->width()), 0.0F, 0.0F, 1.0F },
    point{ static_cast<float>(src->width()), 0.0F, static_cast<float>(src->depth()), 1.0F },
    point{ static_cast<float>(src->width()), static_cast<float>(src->height()), 0.0F, 1.0F },
    point{ static_cast<float>(src->width()), static_cast<float>(src->height()), static_cast<float>(src->depth()), 1.0F }
  };

  // apply the transform matrix to all the point of the bounding box
  bounding_box updated_bbox;
  for (size_t i = 0; i < 8; i++)
  {
    updated_bbox[i] = transform.getMatrix() * bbox[i];
  }

  // find the min and max values for each axis
  point min = updated_bbox[0];
  point max = updated_bbox[0];
  for (size_t i = 1; i < 8; i++)
  {
    min = min.cwiseMin(updated_bbox[i]);
    max = max.cwiseMax(updated_bbox[i]);
  }

  // compute a new width heigth and depth from the min and max point
  const size_t width = static_cast<size_t>(std::round(max[0] - min[0]));
  const size_t height = static_cast<size_t>(std::round(max[1] - min[1]));
  const size_t depth = static_cast<size_t>(std::round(max[2] - min[2]));

  cle::AffineTransform update_transform(transform);
  update_transform.translate(-min[0], -min[1], -min[2]);

  // return the new width, height, depth and the updated transform
  return std::make_tuple(width, height, depth, update_transform);
}

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
                       const bool                   auto_resize) -> cle::Array::Pointer
{
  // initialize shape and transform from args
  size_t               width = src->width();
  size_t               height = src->height();
  size_t               depth = src->depth();
  cle::AffineTransform new_transform(transform);

  // update shape and transform if auto_resize is true
  if (auto_resize)
  {
    std::tie(width, height, depth, new_transform) = prepare_output_shape_and_transform(src, transform);
  }
  // prepare output if dst is nullptr
  if (dst == nullptr)
  {
    dst = cle::Array::create(width, height, depth, src->dimension(), src->dtype(), src->mtype(), src->device());
  }

  // push the matrix on gpu as the inverse transposed transform matrix
  // - we want the inverse because we go from target to source for reading pixel values
  // - we want the transpose because Eigen uses column major and OpenCL uses row major
  auto mat = cle::Array::create(4, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, src->device());
  mat->write(cle::AffineTransform::toArray(new_transform.getInverseTranspose()).data());

  // execute the kernel
  KernelInfo kernel = { "affine_transform", kernel::affine_transform };
  if (interpolate)
  {
    kernel = { "affine_transform_interpolate", kernel::affine_transform_interpolate };
  }
  const ParameterList params = { { "src", src }, { "dst", dst }, { "mat", mat } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(src->device(), kernel, params, range);
  return dst;
}

} // namespace cle

#endif // __INCLUDE_TRANSFORM_HPP
