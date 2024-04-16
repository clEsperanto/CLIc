#include "transform.hpp"

#include "cle_affine_transform.h"
#include "cle_affine_transform_interpolate.h"

namespace cle
{

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
