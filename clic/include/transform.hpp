#ifndef __INCLUDE_TRANSFORM_HPP
#define __INCLUDE_TRANSFORM_HPP

#include <array>

#include "Eigen/Dense"

namespace cle
{

class AffineTransform
{
public:
  AffineTransform()
  {
    m_matrix = Eigen::Matrix4f::Identity();
    update();
  }
  ~AffineTransform() = default;

  auto
  scale(float scale_x, float scale_y, float scale_z) -> void
  {
    // add a scaling factor to the matrix (on the diagonal)
    m_matrix(0, 0) *= scale_x;
    m_matrix(1, 1) *= scale_y;
    m_matrix(2, 2) *= scale_z;
    update();
  }

  auto
  rotate(int axis, float angle_deg) -> void
  {
    // create a rotation matrix
    Eigen::Matrix4f rotation_matrix = Eigen::Matrix4f::Identity();

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
    m_matrix *= rotation_matrix;
    update();
  }

  auto
  rotate_around_x_axis(float angle_deg) -> void
  {
    rotate(0, angle_deg);
  }

  auto
  rotate_around_y_axis(float angle_deg) -> void
  {
    rotate(1, angle_deg);
  }

  auto
  rotate_around_z_axis(float angle_deg) -> void
  {
    rotate(2, angle_deg);
  }

  auto
  translate(float translate_x, float translate_y, float translate_z) -> void
  {
    // add a translation to the matrix
    Eigen::Matrix4f translation_matrix = Eigen::Matrix4f::Identity();
    translation_matrix(0, 3) = translate_x;
    translation_matrix(1, 3) = translate_y;
    translation_matrix(2, 3) = translate_z;
    m_matrix *= translation_matrix;
    update();
  }

  auto
  center(std::array<size_t, 3> shape, bool undo)
  {
    int presign = (undo) ? -1 : 1;
    m_matrix(0, 3) += (shape[0] != 1) ? presign * (shape[0] / 2.0) : 0;
    m_matrix(1, 3) += (shape[1] != 1) ? presign * (shape[1] / 2.0) : 0;
    m_matrix(2, 3) += (shape[2] != 1) ? presign * (shape[2] / 2.0) : 0;
    update();
  }

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
    Eigen::Matrix4f shear_matrix = Eigen::Matrix4f::Identity();
    shear_matrix(0, 1) = shear_xz;
    shear_matrix(1, 0) = shear_yz;
    m_matrix = shear_matrix * m_matrix;
    update();
  }

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
    Eigen::Matrix4f shear_matrix = Eigen::Matrix4f::Identity();
    shear_matrix(0, 2) = shear_xy;
    shear_matrix(2, 0) = shear_zy;
    m_matrix = shear_matrix * m_matrix;
    update();
  }

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
    Eigen::Matrix4f shear_matrix = Eigen::Matrix4f::Identity();
    shear_matrix(1, 2) = shear_yx;
    shear_matrix(2, 1) = shear_zx;
    m_matrix = shear_matrix * m_matrix;
    update();
  }

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
    update();
  }

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
    update();
  }

  auto
  getMatrix() const -> const Eigen::Matrix4f &
  {
    return m_matrix;
  }

  auto
  getInverse() const -> const Eigen::Matrix4f &
  {
    return m_inverse;
  }

  auto
  getInverseTranspose() const -> const Eigen::Matrix4f &
  {
    return m_inverse_transpose;
  }

  auto
  toArray() const -> std::array<float, 16>
  {
    std::array<float, 16> array;
    std::copy(m_transpose.data(), m_transpose.data() + 16, array.begin());
    return array;
  }

protected:
  static auto
  deg_to_rad(float angle_deg) -> float
  {
    return angle_deg * static_cast<float>(M_PI / 180.0);
  }

  static auto
  shear_angle_to_shear_factor(float angle_deg) -> float
  {
    return 1.0 / std::tan((90 - angle_deg) * M_PI / 180);
  }

private:
  Eigen::Matrix4f m_matrix;
  Eigen::Matrix4f m_inverse;
  Eigen::Matrix4f m_inverse_transpose;
  Eigen::Matrix4f m_transpose;

  auto
  update() -> void
  {
    m_inverse = m_matrix.inverse();
    m_inverse_transpose = m_inverse.transpose();
    m_transpose = m_matrix.transpose();
  }
};

} // namespace cle

#endif // __INCLUDE_TRANSFORM_HPP
