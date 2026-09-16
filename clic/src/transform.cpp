#include "transform.hpp"

#include "cle_affine_transform.h"
#include "cle_affine_transform_deskew_x.h"
#include "cle_affine_transform_deskew_y.h"
#include "cle_affine_transform_interpolate.h"

namespace cle::transform
{

auto
prepare_output_shape_and_transform(const size_t                            width,
                                   const size_t                            height,
                                   const size_t                            depth,
                                   const cle::transform::TransformMatrix & transform)
  -> std::tuple<size_t, size_t, size_t, cle::transform::TransformMatrix>
{
  using point = Eigen::Vector4f;
  using bounding_box = std::array<point, 8>;

  bounding_box bbox = { point{ 0.0F, 0.0F, 0.0F, 1.0F },
                        point{ 0.0F, 0.0F, static_cast<float>(depth), 1.0F },
                        point{ 0.0F, static_cast<float>(height), 0.0F, 1.0F },
                        point{ static_cast<float>(width), 0.0F, 0.0F, 1.0F },
                        point{ static_cast<float>(width), static_cast<float>(height), 0.0F, 1.0F },
                        point{ 0.0F, static_cast<float>(height), static_cast<float>(depth), 1.0F },
                        point{ static_cast<float>(width), 0.0F, static_cast<float>(depth), 1.0F },
                        point{ static_cast<float>(width), static_cast<float>(height), static_cast<float>(depth), 1.0F } };

  // apply the transform matrix to all the point of the bounding box
  bounding_box updated_bbox;
  std::transform(bbox.begin(), bbox.end(), updated_bbox.begin(), [&](const point & p) { return transform.getMatrix() * p; });

  // find the min and max values for each axis
  point min = updated_bbox[0];
  point max = updated_bbox[0];
  for (const auto & point : updated_bbox)
  {
    min = min.cwiseMin(point);
    max = max.cwiseMax(point);
  }

  // compute a new width heigth and depth from the min and max point
  cle::transform::TransformMatrix update_transform(transform);
  const auto                      new_width = static_cast<size_t>(std::round(max[0] - min[0]));
  const auto                      new_height = static_cast<size_t>(std::round(max[1] - min[1]));
  const auto                      new_depth = static_cast<size_t>(std::round(max[2] - min[2]));
  update_transform.translate(-min[0], -min[1], -min[2]);

  // return the new width, height, depth and the updated transform
  return std::make_tuple(new_width, new_height, new_depth, update_transform);
}


auto
affine_transform(const cle::Array::Pointer &             src,
                 cle::Array::Pointer                     dst,
                 const cle::transform::TransformMatrix & transform,
                 const bool                              interpolate,
                 const bool                              auto_resize) -> cle::Array::Pointer
{
  cle::transform::TransformMatrix new_transform(transform);
  auto                            width = src->width();
  auto                            height = src->height();
  auto                            depth = src->depth();

  // update shape and transform if auto_resize is true
  if (auto_resize)
  {
    std::tie(width, height, depth, new_transform) =
      prepare_output_shape_and_transform(src->width(), src->height(), src->depth(), transform);
  }
  // prepare output if dst is nullptr
  if (dst == nullptr)
  {
    auto dtype = (interpolate) ? cle::dType::FLOAT : src->dtype();
    dst = cle::Array::create(width, height, depth, src->dimension(), dtype, src->mtype(), src->device());
  }

  // push the matrix on gpu as the inverse transposed transform matrix
  auto mat = cle::Array::create(4, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, src->device());
  mat->writeFrom(cle::transform::TransformMatrix::toArray(new_transform.getInverseTranspose()).data());

  cle::Array::Pointer image = src;
  if (interpolate && src->mtype() != mType::IMAGE)
  {
    // interpolate is only available for image type, we copy src into an image if it is not already
    try
    {
      image = cle::Array::create(src->width(), src->height(), src->depth(), src->dimension(), dType::FLOAT, mType::IMAGE, src->device());
      cle::tier1::copy_func(src->device(), src, image);
    }
    catch (const std::exception & e)
    {
      if (src->device()->getType() == Device::Type::CUDA || src->device()->getType() == Device::Type::METAL)
      {
        std::cerr << "Warning: Interpolation is NOT supported for CUDA and METAL devices, please use the OPENCL backend for this."
                  << std::endl;
      }
      else
      {
        std::cerr << "Warning: Device does not support hardware interpolation." << std::endl;
      }
      std::cerr << "-> Falling back to non-interpolated transform." << std::endl;
    }
  }

  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const ParameterList params = { { "src", image }, { "dst", dst }, { "mat", mat } };
  const KernelInfo    kernel =
    (interpolate && image->mtype() == mType::IMAGE ? KernelInfo{ "affine_transform_interpolate", kernel::affine_transform_interpolate }
                                                   : KernelInfo{ "affine_transform", kernel::affine_transform });

  // execute the kernel
  execute(src->device(), kernel, params, range);
  return dst;
}


auto
affine_transform_deskew_3d(const cle::Array::Pointer &             src,
                           cle::Array::Pointer                     dst,
                           const cle::transform::TransformMatrix & transform,
                           float                                   deskewing_angle,
                           float                                   voxel_size_x,
                           float                                   voxel_size_y,
                           float                                   voxel_size_z,
                           int                                     deskew_direction,
                           bool                                    auto_resize) -> cle::Array::Pointer
{

  if (src->depth() == 1)
  {
    throw std::runtime_error("Deskewing is only available for 3D images.");
  }

  // update shape and transform
  cle::transform::TransformMatrix new_transform(transform);
  auto                            width = src->width();
  auto                            height = src->height();
  auto                            depth = src->depth();

  // update shape and transform if auto_resize is true
  if (auto_resize)
  {
    std::tie(width, height, depth, new_transform) =
      prepare_output_shape_and_transform(src->width(), src->height(), src->depth(), transform);
  }

  // prepare output if dst is nullptr
  if (dst == nullptr)
  {
    dst = cle::Array::create(width, height, depth, src->dimension(), src->dtype(), src->mtype(), src->device());
  }

  // push the matrix on gpu as the inverse transposed transform matrix
  auto mat = cle::Array::create(4, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, src->device());
  mat->writeFrom(cle::transform::TransformMatrix::toArray(new_transform.getInverseTranspose()).data());

  // precalculate these functions that are dependent on deskewing angle
  float tantheta = static_cast<float>(tan(deskewing_angle * M_PI / 180.0f));
  float sintheta = static_cast<float>(sin(deskewing_angle * M_PI / 180.0f));
  float costheta = static_cast<float>(cos(deskewing_angle * M_PI / 180.0f));

  float      pixel_step;
  KernelInfo kernel;
  switch (deskew_direction)
  {
    case 0: // deskew along x axis
    {
      kernel = { "affine_transform_deskew_x", kernel::affine_transform_deskew_x };
      pixel_step = static_cast<float>(voxel_size_z / voxel_size_y);
      break;
    }
    case 1: // deskew along y axis
    {
      kernel = { "affine_transform_deskew_y", kernel::affine_transform_deskew_y };
      pixel_step = static_cast<float>(voxel_size_z / voxel_size_x);
      break;
    }
    default:
      break;
  }

  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  const ParameterList params = {
    { "src", src },           { "dst", dst },           { "mat", mat },           { "pixel_step", pixel_step },
    { "tantheta", tantheta }, { "costheta", costheta }, { "sintheta", sintheta },
  };

  // execute the kernel
  execute(src->device(), kernel, params, range);
  return dst;
}

} // namespace cle::transform
