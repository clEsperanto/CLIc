#include "tier0.hpp"

namespace cle::tier0
{

/**
 * @brief Check ptr and set type if unknown
 * @param src source array pointer
 * @param dst destination array pointer
 * @param type data type
 * @return true if dst is not null
*/
auto
check_and_set(const Array::Pointer & src, Array::Pointer & dst, dType & type) -> bool
{
  if (dst != nullptr)
  {
    return true;
  }
  if(src == nullptr)
  {
    throw std::invalid_argument("Error: Cannot generate output Array because the provided 'src' is null.");
  }
  if (type == dType::UNKNOWN)
  {
    type = src->dtype();
  }
  return false;
}

/**
 * @brief Create a destination array with the provided parameters
 * @param src source array pointer
 * @param dst destination array pointer
 * @param width width of the array
 * @param height height of the array
 * @param depth depth of the array
 * @param type data type
 * @return void
*/
auto
create_dst(const Array::Pointer & src, Array::Pointer & dst, size_t width, size_t height, size_t depth, dType type)
  -> void
{
  if ( check_and_set(src, dst, type) ) { return;  }
  auto dim = shape_to_dimension(width, height, depth);
  dst = Array::create(width, height, depth, dim, type, src->mtype(), src->device());
}

/**
 * @brief Create a destination array identical to the source array
 * @param src source array pointer
 * @param dst destination array pointer
 * @param type data type
 * @return void
*/
auto
create_like(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  if ( check_and_set(src, dst, type) ) { return;  }
  dst = Array::create(src->width(), src->height(), src->depth(), src->dimension(), type, src->mtype(), src->device());
}

/**
 * @brief Create a destination array with a single element
 * @param src source array pointer
 * @param dst destination array pointer
 * @param type data type
 * @return void
*/
auto
create_one(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  if ( check_and_set(src, dst, type) ) { return;  }
  dst = Array::create(1, 1, 1, 1, type, mType::BUFFER, src->device());
}

/**
 * @brief Create a destination array of dimension 1
 * @param src source array pointer
 * @param dst destination array pointer
 * @param size size of the array
 * @param type data type
 * @return void
*/
auto
create_vector(const Array::Pointer & src, Array::Pointer & dst, const size_t & size, dType type) -> void
{
  if ( check_and_set(src, dst, type) ) { return;  }
  dst = Array::create(size, 1, 1, 1, type, mType::BUFFER, src->device());
}

/**
 * @brief Create a destination array with the (x,y,1) as the source array
 * @param src source array pointer
 * @param dst destination array pointer
 * @param type data type
 * @return void
*/
auto
create_xy(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  if ( check_and_set(src, dst, type) ) { return;  }
  auto dim = shape_to_dimension(src->width(), src->height(), 1);
  dst = Array::create(src->width(), src->height(), 1, dim, type, src->mtype(), src->device());
}

/**
 * @brief Create a destination array with the (y,x,1) as the source array
 * @param src source array pointer
 * @param dst destination array pointer
 * @param type data type
 * @return void
*/
auto
create_yx(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  if ( check_and_set(src, dst, type) ) { return;  }
  auto dim = shape_to_dimension(src->height(), src->width(), 1);
  dst = Array::create(src->height(), src->width(), 1, dim, type, src->mtype(), src->device());
}

/**
 * @brief Create a destination array with the (z,y,1) as the source array
 * @param src source array pointer
 * @param dst destination array pointer
 * @param type data type
 * @return void
*/
auto
create_zy(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  if ( check_and_set(src, dst, type) ) { return;  }
  auto dim = shape_to_dimension(src->depth(), src->height(), 1);
  dst = Array::create(src->depth(), src->height(), 1, dim, type, src->mtype(), src->device());
}

/**
 * @brief Create a destination array with the (y,z,1) as the source array
 * @param src source array pointer
 * @param dst destination array pointer
 * @param type data type
 * @return void
*/
auto
create_yz(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  if ( check_and_set(src, dst, type) ) { return;  }
  auto dim = shape_to_dimension(src->height(), src->depth(), 1);
  dst = Array::create(src->height(), src->depth(), 1, dim, type, src->mtype(), src->device());
}

/**
 * @brief Create a destination array with the (x,z,1) as the source array
 * @param src source array pointer
 * @param dst destination array pointer
 * @param type data type
 * @return void
*/
auto
create_xz(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  if ( check_and_set(src, dst, type) ) { return;  }
  auto dim = shape_to_dimension(src->width(), src->depth(), 1);
  dst = Array::create(src->width(), src->depth(), 1, dim, type, src->mtype(), src->device());
}

/**
 * @brief Create a destination array with the (z,x,1) as the source array
 * @param src source array pointer
 * @param dst destination array pointer
 * @param type data type
 * @return void
*/
auto
create_zx(const Array::Pointer & src, Array::Pointer & dst, dType type) -> void
{
  if ( check_and_set(src, dst, type) ) { return;  }
  auto dim = shape_to_dimension(src->depth(), src->width(), 1);
  dst = Array::create(src->depth(), src->width(), 1, dim, type, src->mtype(), src->device());
}


} // namespace cle::tier0
