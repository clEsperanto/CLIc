#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"
#include "tier7.hpp"

#include "transform.hpp"
#include "utils.hpp"

namespace cle::tier7
{

auto
affine_transform_func(const Device::Pointer & device,
                      const Array::Pointer &  src,
                      Array::Pointer          dst,
                      std::vector<float> *    transform_matrix,
                      bool                    interpolate,
                      bool                    resize) -> Array::Pointer
{
  if (transform_matrix == nullptr)
  {
    transform_matrix = new std::vector<float>({ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 });
  }
  if (transform_matrix->size() != 16 && transform_matrix->size() != 9)
  {
    throw std::runtime_error("Error: Transformation matrix size must be 9 or 16.");
  }
  std::array<float, 16> transform_matrix_arr;
  if (transform_matrix->size() == 9)
  {
    // Fill the array with the 3x3 matrix and the extra row and column for the 4x4 matrix
    transform_matrix_arr = { (*transform_matrix)[0],
                             (*transform_matrix)[1],
                             0,
                             (*transform_matrix)[2],
                             (*transform_matrix)[3],
                             (*transform_matrix)[4],
                             0,
                             (*transform_matrix)[5],
                             (*transform_matrix)[6],
                             (*transform_matrix)[7],
                             1,
                             0, // transform_matrix[8],
                             0,
                             0,
                             0,
                             1 };
  }
  else
  {
    // If the matrix is already 4x4, just copy the values
    std::copy(transform_matrix->begin(), transform_matrix->end(), transform_matrix_arr.begin());
  }
  auto transform = cle::transform::TransformMatrix(transform_matrix_arr);
  return cle::transform::affine_transform(src, dst, transform, interpolate, resize);
}


auto
rigid_transform_func(const Device::Pointer & device,
                     const Array::Pointer &  src,
                     Array::Pointer          dst,
                     float                   translate_x,
                     float                   translate_y,
                     float                   translate_z,
                     float                   angle_x,
                     float                   angle_y,
                     float                   angle_z,
                     bool                    centered,
                     bool                    interpolate,
                     bool                    resize) -> Array::Pointer
{
  auto transform = cle::transform::TransformMatrix();
  if (centered)
  {
    transform.center({ src->width(), src->height(), src->depth() }, false);
  }
  if (angle_x != 0)
  {
    transform.rotate(0, angle_x);
  }
  if (angle_y != 0)
  {
    transform.rotate(1, angle_y);
  }
  if (angle_z != 0)
  {
    transform.rotate(2, angle_z);
  }
  if (centered)
  {
    transform.center({ src->width(), src->height(), src->depth() }, true);
  }
  transform.translate(translate_x, translate_y, translate_z);
  return cle::transform::affine_transform(src, dst, transform, interpolate, resize);
}


auto
rotate_func(const Device::Pointer & device,
            const Array::Pointer &  src,
            Array::Pointer          dst,
            float                   angle_x,
            float                   angle_y,
            float                   angle_z,
            bool                    centered,
            bool                    interpolate,
            bool                    resize) -> Array::Pointer
{
  auto transform = cle::transform::TransformMatrix();
  if (centered)
  {
    transform.center({ src->width(), src->height(), src->depth() }, false);
  }
  if (angle_x != 0)
  {
    transform.rotate(0, angle_x);
  }
  if (angle_y != 0)
  {
    transform.rotate(1, angle_y);
  }
  if (angle_z != 0)
  {
    transform.rotate(2, angle_z);
  }
  if (centered)
  {
    transform.center({ src->width(), src->height(), src->depth() }, true);
  }
  return cle::transform::affine_transform(src, dst, transform, interpolate, resize);
}


auto
scale_func(const Device::Pointer & device,
           const Array::Pointer &  src,
           Array::Pointer          dst,
           float                   factor_x,
           float                   factor_y,
           float                   factor_z,
           bool                    centered,
           bool                    interpolate,
           bool                    resize) -> Array::Pointer
{
  auto transform = cle::transform::TransformMatrix();
  if (centered && !resize)
  {
    transform.center({ src->width(), src->height(), src->depth() }, false);
  }
  transform.scale(factor_x, factor_y, factor_z);
  if (centered && !resize)
  {
    transform.center({ src->width(), src->height(), src->depth() }, true);
  }
  return cle::transform::affine_transform(src, dst, transform, interpolate, resize);
}


auto
translate_func(const Device::Pointer & device,
               const Array::Pointer &  src,
               Array::Pointer          dst,
               float                   translate_x,
               float                   translate_y,
               float                   translate_z,
               bool                    interpolate) -> Array::Pointer
{
  auto transform = cle::transform::TransformMatrix();
  transform.translate(translate_x, translate_y, translate_z);
  return cle::transform::affine_transform(src, dst, transform, interpolate, false);
}


auto
deskew_x_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              float                   angle,
              float                   voxel_size_x,
              float                   voxel_size_y,
              float                   voxel_size_z,
              float                   scale_factor) -> Array::Pointer
{
  auto transform = cle::transform::TransformMatrix();
  transform.deskew_x(angle, voxel_size_x, voxel_size_y, voxel_size_z, scale_factor);

  return cle::transform::affine_transform_deskew_3d(src, dst, transform, angle, voxel_size_x, voxel_size_y, voxel_size_z, 0, true);
}


auto
deskew_y_func(const Device::Pointer & device,
              const Array::Pointer &  src,
              Array::Pointer          dst,
              float                   angle,
              float                   voxel_size_x,
              float                   voxel_size_y,
              float                   voxel_size_z,
              float                   scale_factor) -> Array::Pointer
{
  auto transform = cle::transform::TransformMatrix();
  transform.deskew_y(angle, voxel_size_x, voxel_size_y, voxel_size_z, scale_factor);

  return cle::transform::affine_transform_deskew_3d(src, dst, transform, angle, voxel_size_x, voxel_size_y, voxel_size_z, 1, true);
}

} // namespace cle::tier7
