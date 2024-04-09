#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"
#include "tier6.hpp"
#include "tier7.hpp"

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
  auto transform = AffineTransform(transform_matrix_arr);
  return tier0::apply_affine_transform(src, dst, transform, interpolate, resize);
}

} // namespace cle::tier7
