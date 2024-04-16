#include "tier0.hpp"

namespace cle::tier0
{

auto
create_dst(const Array::Pointer & src, Array::Pointer & dst, size_t width, size_t height, size_t depth, dType type)
  -> void
{
  Array::check_ptr(src, "Error: 'src' is null. Please ensure the provided parameters before calling this function.");
  if (dst != nullptr)
  {
    return;
  }
  if (type == dType::UNKNOWN)
  {
    type = src->dtype();
  }
  auto dim = shape_to_dimension(width, height, depth);
  dst = Array::create(width, height, depth, dim, type, src->mtype(), src->device());
}

auto
create_like(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  Array::check_ptr(src, "Error: 'src' is null. Please ensure the provided parameters before calling this function.");
  if (dst != nullptr)
  {
    return;
  }
  if (type == dType::UNKNOWN)
  {
    type = src->dtype();
  }
  dst = Array::create(src->width(), src->height(), src->depth(), src->dimension(), type, src->mtype(), src->device());
}

auto
create_one(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  Array::check_ptr(src, "Error: 'src' is null. Please ensure the provided parameters before calling this function.");
  if (dst != nullptr)
  {
    return;
  }
  if (type == dType::UNKNOWN)
  {
    type = src->dtype();
  }
  dst = Array::create(1, 1, 1, 1, type, mType::BUFFER, src->device());
}

auto
create_vector(const Array::Pointer & src, Array::Pointer & dst, const size_t & size, dType type) -> void
{
  Array::check_ptr(src, "Error: 'src' is null. Please ensure the provided parameters before calling this function.");
  if (dst != nullptr)
  {
    return;
  }
  if (type == dType::UNKNOWN)
  {
    type = src->dtype();
  }
  dst = Array::create(size, 1, 1, 1, type, mType::BUFFER, src->device());
}

auto
create_xy(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  Array::check_ptr(src, "Error: 'src' is null. Please ensure the provided parameters before calling this function.");
  if (dst != nullptr)
  {
    return;
  }
  if (type == dType::UNKNOWN)
  {
    type = src->dtype();
  }
  auto dim = shape_to_dimension(src->width(), src->height(), 1);
  dst = Array::create(src->width(), src->height(), 1, dim, type, src->mtype(), src->device());
}

auto
create_yx(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  Array::check_ptr(src, "Error: 'src' is null. Please ensure the provided parameters before calling this function.");
  if (dst != nullptr)
  {
    return;
  }
  if (type == dType::UNKNOWN)
  {
    type = src->dtype();
  }
  auto dim = shape_to_dimension(src->height(), src->width(), 1);
  dst = Array::create(src->height(), src->width(), 1, dim, type, src->mtype(), src->device());
}

auto
create_zy(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  Array::check_ptr(src, "Error: 'src' is null. Please ensure the provided parameters before calling this function.");
  if (dst != nullptr)
  {
    return;
  }
  if (type == dType::UNKNOWN)
  {
    type = src->dtype();
  }
  auto dim = shape_to_dimension(src->depth(), src->height(), 1);
  dst = Array::create(src->depth(), src->height(), 1, dim, type, src->mtype(), src->device());
}

auto
create_yz(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  Array::check_ptr(src, "Error: 'src' is null. Please ensure the provided parameters before calling this function.");
  if (dst != nullptr)
  {
    return;
  }
  if (type == dType::UNKNOWN)
  {
    type = src->dtype();
  }
  auto dim = shape_to_dimension(src->height(), src->depth(), 1);
  dst = Array::create(src->height(), src->depth(), 1, dim, type, src->mtype(), src->device());
}

auto
create_xz(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  Array::check_ptr(src, "Error: 'src' is null. Please ensure the provided parameters before calling this function.");
  if (dst != nullptr)
  {
    return;
  }
  if (type == dType::UNKNOWN)
  {
    type = src->dtype();
  }
  auto dim = shape_to_dimension(src->width(), src->depth(), 1);
  dst = Array::create(src->width(), src->depth(), 1, dim, type, src->mtype(), src->device());
}

auto
create_zx(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  Array::check_ptr(src, "Error: 'src' is null. Please ensure the provided parameters before calling this function.");
  if (dst != nullptr)
  {
    return;
  }
  if (type == dType::UNKNOWN)
  {
    type = src->dtype();
  }
  auto dim = shape_to_dimension(src->depth(), src->width(), 1);
  dst = Array::create(src->depth(), src->width(), 1, dim, type, src->mtype(), src->device());
}


} // namespace cle::tier0