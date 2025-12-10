#include "transform.hpp"

#include "cle_affine_transform.h"
#include "cle_affine_transform_deskew_x.h"
#include "cle_affine_transform_deskew_y.h"
#include "cle_affine_transform_interpolate.h"

namespace cle
{

auto
prepare_output_shape_and_transform(const cle::Array::Pointer & src, const cle::AffineTransform & transform)
  -> std::tuple<size_t, size_t, size_t, cle::AffineTransform>
{
  using point = Eigen::Vector4f;
  using bounding_box = std::array<point, 8>;

  bounding_box bbox = { point{ 0.0F, 0.0F, 0.0F, 1.0F },
                        point{ 0.0F, 0.0F, static_cast<float>(src->depth()), 1.0F },
                        point{ 0.0F, static_cast<float>(src->height()), 0.0F, 1.0F },
                        point{ static_cast<float>(src->width()), 0.0F, 0.0F, 1.0F },
                        point{ static_cast<float>(src->width()), static_cast<float>(src->height()), 0.0F, 1.0F },
                        point{ 0.0F, static_cast<float>(src->height()), static_cast<float>(src->depth()), 1.0F },
                        point{ static_cast<float>(src->width()), 0.0F, static_cast<float>(src->depth()), 1.0F },
                        point{
                          static_cast<float>(src->width()), static_cast<float>(src->height()), static_cast<float>(src->depth()), 1.0F } };

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
  cle::AffineTransform update_transform(transform);
  const auto           width = static_cast<size_t>(std::round(max[0] - min[0]));
  const auto           height = static_cast<size_t>(std::round(max[1] - min[1]));
  const auto           depth = static_cast<size_t>(std::round(max[2] - min[2]));
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
  cle::AffineTransform new_transform(transform);
  auto                 width = src->width();
  auto                 height = src->height();
  auto                 depth = src->depth();

  // update shape and transform if auto_resize is true
  if (auto_resize)
  {
    std::tie(width, height, depth, new_transform) = prepare_output_shape_and_transform(src, transform);
  }
  // prepare output if dst is nullptr
  if (dst == nullptr)
  {
    auto dtype = (interpolate) ? cle::dType::FLOAT : src->dtype();
    dst = cle::Array::create(width, height, depth, src->dimension(), dtype, src->mtype(), src->device());
  }

  // push the matrix on gpu as the inverse transposed transform matrix
  auto mat = cle::Array::create(4, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, src->device());
  mat->writeFrom(cle::AffineTransform::toArray(new_transform.getInverseTranspose()).data());

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
      std::cerr << "Warning: Device does not support Image type, interpolation is not available." << std::endl;
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
apply_affine_transform_deskew_3d(const cle::Array::Pointer &  src,
                                 cle::Array::Pointer          dst,
                                 const cle::AffineTransform & transform,
                                 float                        deskewing_angle,
                                 float                        voxel_size_x,
                                 float                        voxel_size_y,
                                 float                        voxel_size_z,
                                 int                          deskew_direction,
                                 bool                         auto_resize) -> cle::Array::Pointer
{

  if (src->depth() == 1)
  {
    throw std::runtime_error("Deskewing is only available for 3D images.");
  }

  // update shape and transform
  cle::AffineTransform new_transform(transform);
  auto                 width = src->width();
  auto                 height = src->height();
  auto                 depth = src->depth();

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

  // interpolate is only available for image type, we copy src into an image if it is not already
  cle::Array::Pointer image = nullptr;
  if (src->mtype() == mType::IMAGE)
  {
    image = src;
  }
  else
  {
    try
    {
      image = cle::Array::create(src->width(), src->height(), src->depth(), src->dimension(), src->dtype(), mType::IMAGE, src->device());
      src->copyTo(image);
    }
    catch (const std::exception & e)
    {
      std::cerr << "Warning: Device does not support Image type. Deskewing is not available, falling back to "
                   "non-deskewed transform."
                << std::endl;
      dst = apply_affine_transform(src, dst, new_transform, false, false);
      return dst;
    }
  }

  // push the matrix on gpu as the inverse transposed transform matrix
  auto mat = cle::Array::create(4, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, src->device());
  mat->writeFrom(cle::AffineTransform::toArray(new_transform.getInverseTranspose()).data());

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
    { "src", image },         { "dst", dst },           { "mat", mat },           { "pixel_step", pixel_step },
    { "tantheta", tantheta }, { "costheta", costheta }, { "sintheta", sintheta },
  };

  // execute the kernel
  execute(src->device(), kernel, params, range);
  return dst;
}

} // namespace cle
