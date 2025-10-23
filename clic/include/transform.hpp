#ifndef __INCLUDE_TRANSFORM_HPP
#define __INCLUDE_TRANSFORM_HPP

#include <array>

#include "Eigen/Dense"
#include "array.hpp"
#include "execution.hpp"

namespace cle
{

/**
 * @brief Affine transformation class
 *      This class is used to create an affine transformation matrix by applying different transformations
 *      such as scaling, rotation, translation, shearing, deskewing, etc. to the AffineTransform matrix.
 *      The computed matrix is meant to be used to transform an image or a volume using the affine_transform kernel.
 *      The class rely on the Eigen library [1] to perform the matrix operations.
 *
 * @see https://eigen.tuxfamily.org
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
    m_matrix = matrix::Map(array.data()).transpose();
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
    const float epsilon = std::numeric_limits<float>::epsilon();
    const float angle_rad = deg_to_rad(angle_deg);
    const float angle_cos_temp = std::cos(angle_rad);
    const float angle_sin_temp = std::sin(angle_rad);
    const float angle_cos = (std::abs(angle_cos_temp) < epsilon) ? 0 : angle_cos_temp;
    const float angle_sin = (std::abs(angle_sin_temp) < epsilon) ? 0 : angle_sin_temp;

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
    constexpr float pi180 = static_cast<float>(M_PI / 180.0);

    // define shear factor (different computation than the methods below)
    float shear_factor = std::sin((90 - angle_deg) * pi180) * (voxel_size_z / voxel_size_x);
    m_matrix(0, 2) += shear_factor;

    // make voxels isotropic, calculate the new scaling factor for Z after shearing
    // https://github.com/tlambert03/napari-ndtiffs/blob/092acbd92bfdbf3ecb1eb9c7fc146411ad9e6aae/napari_ndtiffs/affine.py#L57
    auto new_dz = std::sin(angle_deg * pi180) * voxel_size_z;
    auto scale_factor_z = (new_dz / voxel_size_x) * scale_factor;

    scale(scale_factor, scale_factor, scale_factor_z);

    // correct orientation so that the new Z-plane goes proximal-distal from the objective.
    rotate_around_y_axis(angle_deg);
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
    constexpr float pi180 = static_cast<float>(M_PI / 180.0);

    // define shear factor (different computation than the methods below)
    float shear_factor = std::sin((90 - angle_deg) * pi180) * (voxel_size_z / voxel_size_y);
    m_matrix(1, 2) += shear_factor;

    // make voxels isotropic, calculate the new scaling factor for Z after shearing
    // https://github.com/tlambert03/napari-ndtiffs/blob/092acbd92bfdbf3ecb1eb9c7fc146411ad9e6aae/napari_ndtiffs/affine.py#L57
    auto new_dz = std::sin(angle_deg * pi180) * voxel_size_z;
    auto scale_factor_z = (new_dz / voxel_size_y) * scale_factor;

    scale(scale_factor, scale_factor, scale_factor_z);

    // correct orientation so that the new Z-plane goes proximal-distal from the objective.
    rotate_around_x_axis(0 - angle_deg);
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
    return static_cast<float>(1.0 / std::tan((90.0 - angle_deg) * M_PI / 180.0));
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


/**
 * @brief Apply deskew transform to a 3D array
 * @param src Source array
 * @param dst Destination array
 * @param transform Affine transform
 * @param deskewing_angle Deskewing angle (in degrees)
 * @param voxel_size_x Voxel size in the x axis
 * @param voxel_size_y Voxel size in the y axis
 * @param voxel_size_z Voxel size in the z axis
 * @param deskew_direction Deskewing direction (0: x, 1: y)
 * @param auto_resize Auto resize flag
 * @return cle::Array::Pointer
 */
auto
apply_affine_transform_deskew_3d(const cle::Array::Pointer &  src,
                                 cle::Array::Pointer          dst,
                                 const cle::AffineTransform & transform,
                                 float                        deskewing_angle,
                                 float                        voxel_size_x,
                                 float                        voxel_size_y,
                                 float                        voxel_size_z,
                                 int                          deskew_direction,
                                 bool                         auto_resize) -> cle::Array::Pointer;

} // namespace cle

#endif // __INCLUDE_TRANSFORM_HPP
