#include "tier0.hpp"
#include "tier1.hpp"
#include "tier2.hpp"
#include "tier3.hpp"
#include "tier4.hpp"
#include "tier5.hpp"

#include "utils.hpp"

namespace cle::tier5
{

auto
merge_touching_labels_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  // generate touching matrix
  auto touching_matrix = tier3::generate_touch_matrix_func(device, src, nullptr);
  tier1::set_column_func(device, touching_matrix, 0, 0);
  tier1::set_row_func(device, touching_matrix, 0, 0);

  // check if there are touching labels
  if (tier2::maximum_of_all_pixels_func(device, touching_matrix) == 0)
  {
    return src;
  }
  tier0::create_like(src, dst, dType::LABEL);

  // prepare the touching matrix for processing
  tier1::set_where_x_equals_y_func(device, touching_matrix, 1);
  tier1::set_column_func(device, touching_matrix, 0, 0);
  tier1::set_row_func(device, touching_matrix, 0, 0);

  // make a touch-matrix where intensities correspond to the label-ID
  auto label_id_vector = Array::create(1, touching_matrix->height(), 1, 1, dType::LABEL, mType::BUFFER, device);
  tier1::set_ramp_y_func(device, label_id_vector);
  auto touching_matrix_id = Array::create(touching_matrix->width(), touching_matrix->height(), 1, 2, dType::LABEL, mType::BUFFER, device);
  tier1::multiply_images_func(device, touching_matrix, label_id_vector, touching_matrix_id);

  // new list of labels corresponding to maximum x projection of touching matrix
  // e.g. if label 2 and 3 are touching, both will have value to 3 in the list
  label_id_vector = tier1::maximum_y_projection_func(device, touching_matrix_id, nullptr);

  // renumber labels sequentially and replace intensities in the label image with the new labels
  auto new_labels = tier4::relabel_sequential_func(device, label_id_vector, nullptr, 4096);
  tier1::replace_intensities_func(device, src, new_labels, dst);

  // Recall again to make sure all are merged (do we want to do this?)
  return merge_touching_labels_func(device, dst, nullptr);
}

} // namespace cle::tier5
