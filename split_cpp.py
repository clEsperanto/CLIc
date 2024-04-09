import re
import os


include_list = [
'#include "cle_absolute.h"',
'#include "cle_add_image_and_scalar.h"',
'#include "cle_add_images_weighted.h"',
'#include "cle_binary_and.h"',
'#include "cle_binary_edge_detection.h"',
'#include "cle_binary_not.h"',
'#include "cle_binary_or.h"',
'#include "cle_binary_subtract.h"',
'#include "cle_binary_xor.h"',
'#include "cle_block_enumerate.h"',
'#include "cle_convolve.h"',
'#include "cle_copy.h"',
'#include "cle_copy_horizontal_slice_from.h"',
'#include "cle_copy_horizontal_slice_to.h"',
'#include "cle_copy_slice_from.h"',
'#include "cle_copy_slice_to.h"',
'#include "cle_copy_vertical_slice_from.h"',
'#include "cle_copy_vertical_slice_to.h"',
'#include "cle_crop.h"',
'#include "cle_cubic_root.h"',
'#include "cle_detect_label_edges.h"',
'#include "cle_dilate_box.h"',
'#include "cle_dilate_sphere.h"',
'#include "cle_divide_images.h"',
'#include "cle_divide_scalar_by_image.h"',
'#include "cle_equal.h"',
'#include "cle_equal_constant.h"',
'#include "cle_erode_box.h"',
'#include "cle_erode_sphere.h"',
'#include "cle_exponential.h"',
'#include "cle_flip.h"',
'#include "cle_gaussian_blur_separable.h"',
'#include "cle_generate_distance_matrix.h"',
'#include "cle_gradient_x.h"',
'#include "cle_gradient_y.h"',
'#include "cle_gradient_z.h"',
'#include "cle_greater.h"',
'#include "cle_greater_constant.h"',
'#include "cle_greater_or_equal.h"',
'#include "cle_greater_or_equal_constant.h"',
'#include "cle_hessian_eigenvalues.h"',
'#include "cle_laplace_box.h"',
'#include "cle_laplace_diamond.h"',
'#include "cle_local_cross_correlation.h"',
'#include "cle_logarithm.h"',
'#include "cle_mask.h"',
'#include "cle_mask_label.h"',
'#include "cle_maximum_image_and_scalar.h"',
'#include "cle_maximum_images.h"',
'#include "cle_maximum_separable.h"',
'#include "cle_maximum_x_projection.h"',
'#include "cle_maximum_y_projection.h"',
'#include "cle_maximum_z_projection.h"',
'#include "cle_mean_separable.h"',
'#include "cle_mean_sphere.h"',
'#include "cle_mean_x_projection.h"',
'#include "cle_mean_y_projection.h"',
'#include "cle_mean_z_projection.h"',
'#include "cle_median_box.h"',
'#include "cle_median_sphere.h"',
'#include "cle_minimum_separable.h"',
'#include "cle_minimum_image_and_scalar.h"',
'#include "cle_minimum_images.h"',
'#include "cle_minimum_of_masked_pixels_reduction.h"',
'#include "cle_minimum_x_projection.h"',
'#include "cle_minimum_y_projection.h"',
'#include "cle_minimum_z_projection.h"',
'#include "cle_mode_box.h"',
'#include "cle_mode_sphere.h"',
'#include "cle_modulo_images.h"',
'#include "cle_multiply_image_and_position.h"',
'#include "cle_multiply_image_and_scalar.h"',
'#include "cle_multiply_images.h"',
'#include "cle_nan_to_num.h"',
'#include "cle_nonzero_maximum_box.h"',
'#include "cle_nonzero_maximum_diamond.h"',
'#include "cle_nonzero_minimum_box.h"',
'#include "cle_nonzero_minimum_diamond.h"',
'#include "cle_not_equal.h"',
'#include "cle_not_equal_constant.h"',
'#include "cle_onlyzero_overwrite_maximum_box.h"',
'#include "cle_onlyzero_overwrite_maximum_diamond.h"',
'#include "cle_paste.h"',
'#include "cle_power.h"',
'#include "cle_power_images.h"',
'#include "cle_range.h"',
'#include "cle_read_values_from_positions.h"',
'#include "cle_replace_value.h"',
'#include "cle_replace_values.h"',
'#include "cle_maximum_sphere.h"',
'#include "cle_minimum_sphere.h"',
'#include "cle_multiply_matrix.h"',
'#include "cle_reciprocal.h"',
'#include "cle_set.h"',
'#include "cle_set_column.h"',
'#include "cle_set_image_borders.h"',
'#include "cle_set_nonzero_pixels_to_pixelindex.h"',
'#include "cle_set_plane.h"',
'#include "cle_set_ramp_x.h"',
'#include "cle_set_ramp_y.h"',
'#include "cle_set_ramp_z.h"',
'#include "cle_set_row.h"',
'#include "cle_set_where_x_equals_y.h"',
'#include "cle_set_where_x_greater_than_y.h"',
'#include "cle_set_where_x_smaller_than_y.h"',
'#include "cle_sign.h"',
'#include "cle_smaller.h"',
'#include "cle_smaller_constant.h"',
'#include "cle_smaller_or_equal.h"',
'#include "cle_smaller_or_equal_constant.h"',
'#include "cle_sobel.h"',
'#include "cle_square_root.h"',
'#include "cle_std_z_projection.h"',
'#include "cle_subtract_image_from_scalar.h"',
'#include "cle_sum_reduction_x.h"',
'#include "cle_sum_x_projection.h"',
'#include "cle_sum_y_projection.h"',
'#include "cle_sum_z_projection.h"',
'#include "cle_transpose_xy.h"',
'#include "cle_transpose_xz.h"',
'#include "cle_transpose_yz.h"',
'#include "cle_undefined_to_zero.h"',
'#include "cle_variance_box.h"',
'#include "cle_variance_sphere.h"',
'#include "cle_write_values_to_positions.h"',
'#include "cle_x_position_of_maximum_x_projection.h"',
'#include "cle_x_position_of_minimum_x_projection.h"',
'#include "cle_y_position_of_maximum_y_projection.h"',
'#include "cle_y_position_of_minimum_y_projection.h"',
'#include "cle_z_position_of_maximum_z_projection.h"',
'#include "cle_z_position_of_minimum_z_projection.h"',
]



hpp_template = """{tiers_includes} 

#include "utils.hpp"

namespace cle::tier{tier_number} 
{{

{function}  

}}

"""

    

# def extract_function(text):
#     patterns = []
#     start_index = 0

#     while True:
#         start_index = text.find("_func(", start_index)
#         if start_index == -1:
#             break
#         # Find the end pattern
#         end_index_auto = text.find("auto\n", start_index)
#         end_index_namespace = text.find("} // namespace", start_index)
#         end_index = min(end_index_auto, end_index_namespace) if end_index_auto != -1 and end_index_namespace != -1 else max(end_index_auto, end_index_namespace)
#         if end_index == -1:
#             break
#         pattern = text[start_index:end_index + len("_func(")]
#         patterns.append(pattern)
#         start_index = end_index

#     return patterns


def find_pattern(text):
    lines = text.split('\n')
    matches = []
    pattern = "_func\(const"
    for i in range(len(lines)):
        if re.search(pattern, lines[i]):
            start = max(0, i-1)
            end = i+1
            while end < len(lines) and not re.search(pattern, lines[end]):
                end += 1
            if end == len(lines):
                matches.append('\n'.join(lines[start:]))
            else:
                matches.append('\n'.join(lines[start:end-1]))
    return matches

def find_name(text):
    pattern = r'\b(\w+)_func\('  # Regular expression pattern to match {something}_func(
    matches = re.findall(pattern, text)
    return matches[0]

def remove_namespace_lines(text):
    lines = text.split('\n')
    lines = [line for line in lines if "} // namespace " not in line]
    return '\n'.join(lines)


def create_folder(folder):
    if not os.path.exists(folder):
        os.makedirs(folder)

def save_file(file, content):
    with open(file, 'w') as f:
        f.write(content)


def find_include(text):
    # find all the include from the include_list that contains the text
    includes = []
    for include in include_list:
        if text in include:
            includes.append(include)
    return "\n".join(includes)            
        


if __name__ == '__main__':
    # Read the source file
    file = './clic/src/tier2.cpp'
    with open(file, 'r') as f:
        sources = f.read()

    folder_name = file.split('/')[-1].split('.')[0]
    folder_path = f'./clic/src/{folder_name}/'
    create_folder(folder_path)

    # extract the number in folder_name tier{number}
    tier_number = int(folder_name[-1])
    tiers = []
    for i in range(0, tier_number + 1):
        tiers.append(f'#include "tier{i}.hpp"')
    tiers = '\n'.join(tiers)

    functions = find_pattern(sources)
    for function in functions:
        # print(function)
        code = re.sub(r'// auto.*\n', '', function)
        code = remove_namespace_lines(code)
        code = code.strip()
        name = find_name(function)
        # inc = find_include(name)
        source_name = name.lower()
        cpp_source = hpp_template.format(function=code, tier_number=tier_number, tiers_includes=tiers)
        save_file(folder_path + f'{source_name}.cpp', cpp_source)
        print(name)
