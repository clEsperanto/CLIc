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

class AffineTransform
{
public:
  using matrix = Eigen::Matrix4f;

  AffineTransform()
  {
    m_matrix = matrix::Identity();
    update();
  }
  AffineTransform(const std::array<float, 16> & array)
  {
    m_matrix = matrix::Map(array.data());
    update();
  }
  ~AffineTransform() = default;

  auto
  scale(float scale_x, float scale_y, float scale_z) -> void
  {
    matrix scale_matrix = matrix::Identity();
    scale_matrix(0, 0) = scale_x;
    scale_matrix(1, 1) = scale_y;
    scale_matrix(2, 2) = scale_z;
    concats(scale_matrix);
  }

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
    matrix translation_matrix = matrix::Identity();
    translation_matrix(0, 3) = translate_x;
    translation_matrix(1, 3) = translate_y;
    translation_matrix(2, 3) = translate_z;
    concats(translation_matrix);
  }

  auto
  center(const std::array<size_t, 3> & shape, bool undo)
  {
    int         presign = (undo) ? 1 : -1;
    const float centering_x = (shape[0] != 1) ? presign * (shape[0] / 2.0) : 0;
    const float centering_y = (shape[1] != 1) ? presign * (shape[1] / 2.0) : 0;
    const float centering_z = (shape[2] != 1) ? presign * (shape[2] / 2.0) : 0;
    translate(centering_x, centering_y, centering_z);
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
    matrix shear_matrix = matrix::Identity();
    shear_matrix(0, 1) = shear_xz;
    shear_matrix(1, 0) = shear_yz;
    pre_concats(shear_matrix);
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
    matrix shear_matrix = matrix::Identity();
    shear_matrix(0, 2) = shear_xy;
    shear_matrix(2, 0) = shear_zy;
    pre_concats(shear_matrix);
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
    matrix shear_matrix = matrix::Identity();
    shear_matrix(1, 2) = shear_yx;
    shear_matrix(2, 1) = shear_zx;
    pre_concats(shear_matrix);
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
  getTranspose() const -> const Eigen::Matrix4f &
  {
    return m_transpose;
  }

  auto
  getInverseTranspose() const -> const Eigen::Matrix4f &
  {
    return m_inverse_transpose;
  }

  static auto
  toArray(const matrix & mat) -> std::array<float, 16>
  {
    std::array<float, 16> array;
    std::copy(mat.data(), mat.data() + 16, array.begin());
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
    return 1.0F / std::tan((90 - angle_deg) * M_PI / 180);
  }

  auto
  concats(const matrix & mat) -> void
  {
    m_matrix = mat * m_matrix;
    update();
  }

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

  auto
  update() -> void
  {
    m_inverse = m_matrix.inverse();
    m_inverse_transpose = m_inverse.transpose();
    m_transpose = m_matrix.transpose();
  }
};

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
