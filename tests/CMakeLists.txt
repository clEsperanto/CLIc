add_executable(core_gpu_test core_gpu_test.cpp)
add_dependencies(core_gpu_test CLIc)
target_link_libraries(core_gpu_test PRIVATE CLIc::CLIc)
set_target_properties(core_gpu_test PROPERTIES FOLDER "Tests")
target_compile_features(core_gpu_test PRIVATE cxx_std_17)

add_executable(core_scalar_test core_scalar_test.cpp)
add_dependencies(core_scalar_test CLIc)
target_link_libraries(core_scalar_test PRIVATE CLIc::CLIc)
set_target_properties(core_scalar_test PROPERTIES FOLDER "Tests")
target_compile_features(core_scalar_test PRIVATE cxx_std_17)

add_executable(core_buffer_test core_buffer_test.cpp)
add_dependencies(core_buffer_test CLIc)
target_link_libraries(core_buffer_test PRIVATE CLIc::CLIc)
set_target_properties(core_buffer_test PROPERTIES FOLDER "Tests")
target_compile_features(core_buffer_test PRIVATE cxx_std_17)

add_executable(core_image_test core_image_test.cpp)
add_dependencies(core_image_test CLIc)
target_link_libraries(core_image_test PRIVATE CLIc::CLIc)
set_target_properties(core_image_test PROPERTIES FOLDER "Tests")
target_compile_features(core_image_test PRIVATE cxx_std_17)

add_executable(core_kernel_test core_kernel_test.cpp)
add_dependencies(core_kernel_test CLIc)
target_link_libraries(core_kernel_test PRIVATE CLIc::CLIc)
set_target_properties(core_kernel_test PROPERTIES FOLDER "Tests")
target_compile_features(core_kernel_test PRIVATE cxx_std_17)

add_executable(absolute_test absolute_test.cpp)
add_dependencies(absolute_test CLIc)
target_link_libraries(absolute_test PRIVATE CLIc::CLIc)
set_target_properties(absolute_test PROPERTIES FOLDER "Tests")
target_compile_features(absolute_test PRIVATE cxx_std_17)

add_executable(add_image_and_scalar_test add_image_and_scalar_test.cpp)
add_dependencies(add_image_and_scalar_test CLIc)
target_link_libraries(add_image_and_scalar_test PRIVATE CLIc::CLIc)
set_target_properties(add_image_and_scalar_test PROPERTIES FOLDER "Tests")
target_compile_features(add_image_and_scalar_test PRIVATE cxx_std_17)

add_executable(add_image_weighted_test add_image_weighted_test.cpp)
add_dependencies(add_image_weighted_test CLIc)
target_link_libraries(add_image_weighted_test PRIVATE CLIc::CLIc)
set_target_properties(add_image_weighted_test PROPERTIES FOLDER "Tests")
target_compile_features(add_image_weighted_test PRIVATE cxx_std_17)

add_executable(binary_and_test binary_and_test.cpp)
add_dependencies(binary_and_test CLIc)
target_link_libraries(binary_and_test PRIVATE CLIc::CLIc)
set_target_properties(binary_and_test PROPERTIES FOLDER "Tests")
target_compile_features(binary_and_test PRIVATE cxx_std_17)

add_executable(binary_not_test binary_not_test.cpp)
add_dependencies(binary_not_test CLIc)
target_link_libraries(binary_not_test PRIVATE CLIc::CLIc)
set_target_properties(binary_not_test PROPERTIES FOLDER "Tests")
target_compile_features(binary_not_test PRIVATE cxx_std_17)

add_executable(binary_or_test binary_or_test.cpp)
add_dependencies(binary_or_test CLIc)
target_link_libraries(binary_or_test PRIVATE CLIc::CLIc)
set_target_properties(binary_or_test PROPERTIES FOLDER "Tests")
target_compile_features(binary_or_test PRIVATE cxx_std_17)

add_executable(binary_subtract_test binary_subtract_test.cpp)
add_dependencies(binary_subtract_test CLIc)
target_link_libraries(binary_subtract_test PRIVATE CLIc::CLIc)
set_target_properties(binary_subtract_test PROPERTIES FOLDER "Tests")
target_compile_features(binary_subtract_test PRIVATE cxx_std_17)

add_executable(binary_xor_test binary_xor_test.cpp)
add_dependencies(binary_xor_test CLIc)
target_link_libraries(binary_xor_test PRIVATE CLIc::CLIc)
set_target_properties(binary_xor_test PROPERTIES FOLDER "Tests")
target_compile_features(binary_xor_test PRIVATE cxx_std_17)

add_executable(block_enumerate_test block_enumerate_test.cpp)
add_dependencies(block_enumerate_test CLIc)
target_link_libraries(block_enumerate_test PRIVATE CLIc::CLIc)
set_target_properties(block_enumerate_test PROPERTIES FOLDER "Tests")
target_compile_features(block_enumerate_test PRIVATE cxx_std_17)

add_executable(close_indexgaps_in_labelmap_test close_indexgaps_in_labelmap_test.cpp)
add_dependencies(close_indexgaps_in_labelmap_test CLIc)
target_link_libraries(close_indexgaps_in_labelmap_test PRIVATE CLIc::CLIc)
set_target_properties(close_indexgaps_in_labelmap_test PROPERTIES FOLDER "Tests")
target_compile_features(close_indexgaps_in_labelmap_test PRIVATE cxx_std_17)

add_executable(connected_component_labeling_box_test connected_component_labeling_box_test.cpp)
add_dependencies(connected_component_labeling_box_test CLIc)
target_link_libraries(connected_component_labeling_box_test PRIVATE CLIc::CLIc)
set_target_properties(connected_component_labeling_box_test PROPERTIES FOLDER "Tests")
target_compile_features(connected_component_labeling_box_test PRIVATE cxx_std_17)

add_executable(copy_test copy_test.cpp)
add_dependencies(copy_test CLIc)
target_link_libraries(copy_test PRIVATE CLIc::CLIc)
set_target_properties(copy_test PROPERTIES FOLDER "Tests")
target_compile_features(copy_test PRIVATE cxx_std_17)

add_executable(detect_maxima_test detect_maxima_test.cpp)
add_dependencies(detect_maxima_test CLIc)
target_link_libraries(detect_maxima_test PRIVATE CLIc::CLIc)
set_target_properties(detect_maxima_test PROPERTIES FOLDER "Tests")
target_compile_features(detect_maxima_test PRIVATE cxx_std_17)

add_executable(difference_of_gaussian_test difference_of_gaussian_test.cpp)
add_dependencies(difference_of_gaussian_test CLIc)
target_link_libraries(difference_of_gaussian_test PRIVATE CLIc::CLIc)
set_target_properties(difference_of_gaussian_test PROPERTIES FOLDER "Tests")
target_compile_features(difference_of_gaussian_test PRIVATE cxx_std_17)

add_executable(dilate_sphere_test dilate_sphere_test.cpp)
add_dependencies(dilate_sphere_test CLIc)
target_link_libraries(dilate_sphere_test PRIVATE CLIc::CLIc)
set_target_properties(dilate_sphere_test PROPERTIES FOLDER "Tests")
target_compile_features(dilate_sphere_test PRIVATE cxx_std_17)

add_executable(equal_constant_test equal_constant_test.cpp)
add_dependencies(equal_constant_test CLIc)
target_link_libraries(equal_constant_test PRIVATE CLIc::CLIc)
set_target_properties(equal_constant_test PROPERTIES FOLDER "Tests")
target_compile_features(equal_constant_test PRIVATE cxx_std_17)

add_executable(equal_test equal_test.cpp)
add_dependencies(equal_test CLIc)
target_link_libraries(equal_test PRIVATE CLIc::CLIc)
set_target_properties(equal_test PROPERTIES FOLDER "Tests")
target_compile_features(equal_test PRIVATE cxx_std_17)

add_executable(erode_sphere_test erode_sphere_test.cpp)
add_dependencies(erode_sphere_test CLIc)
target_link_libraries(erode_sphere_test PRIVATE CLIc::CLIc)
set_target_properties(erode_sphere_test PROPERTIES FOLDER "Tests")
target_compile_features(erode_sphere_test PRIVATE cxx_std_17)

add_executable(extend_labeling_via_voronoi_test extend_labeling_via_voronoi_test.cpp)
add_dependencies(extend_labeling_via_voronoi_test CLIc)
target_link_libraries(extend_labeling_via_voronoi_test PRIVATE CLIc::CLIc)
set_target_properties(extend_labeling_via_voronoi_test PROPERTIES FOLDER "Tests")
target_compile_features(extend_labeling_via_voronoi_test PRIVATE cxx_std_17)

add_executable(flag_existing_labels_test flag_existing_labels_test.cpp)
add_dependencies(flag_existing_labels_test CLIc)
target_link_libraries(flag_existing_labels_test PRIVATE CLIc::CLIc)
set_target_properties(flag_existing_labels_test PROPERTIES FOLDER "Tests")
target_compile_features(flag_existing_labels_test PRIVATE cxx_std_17)

add_executable(gaussian_blur_test gaussian_blur_test.cpp)
add_dependencies(gaussian_blur_test CLIc)
target_link_libraries(gaussian_blur_test PRIVATE CLIc::CLIc)
set_target_properties(gaussian_blur_test PROPERTIES FOLDER "Tests")
target_compile_features(gaussian_blur_test PRIVATE cxx_std_17)

add_executable(greater_constant_test greater_constant_test.cpp)
add_dependencies(greater_constant_test CLIc)
target_link_libraries(greater_constant_test PRIVATE CLIc::CLIc)
set_target_properties(greater_constant_test PROPERTIES FOLDER "Tests")
target_compile_features(greater_constant_test PRIVATE cxx_std_17)

add_executable(greater_or_equal_constant_test greater_or_equal_constant_test.cpp)
add_dependencies(greater_or_equal_constant_test CLIc)
target_link_libraries(greater_or_equal_constant_test PRIVATE CLIc::CLIc)
set_target_properties(greater_or_equal_constant_test PROPERTIES FOLDER "Tests")
target_compile_features(greater_or_equal_constant_test PRIVATE cxx_std_17)

add_executable(greater_or_equal_test greater_or_equal_test.cpp)
add_dependencies(greater_or_equal_test CLIc)
target_link_libraries(greater_or_equal_test PRIVATE CLIc::CLIc)
set_target_properties(greater_or_equal_test PROPERTIES FOLDER "Tests")
target_compile_features(greater_or_equal_test PRIVATE cxx_std_17)

add_executable(greater_test greater_test.cpp)
add_dependencies(greater_test CLIc)
target_link_libraries(greater_test PRIVATE CLIc::CLIc)
set_target_properties(greater_test PROPERTIES FOLDER "Tests")
target_compile_features(greater_test PRIVATE cxx_std_17)

add_executable(histogram_test histogram_test.cpp)
add_dependencies(histogram_test CLIc)
target_link_libraries(histogram_test PRIVATE CLIc::CLIc)
set_target_properties(histogram_test PROPERTIES FOLDER "Tests")
target_compile_features(histogram_test PRIVATE cxx_std_17)

add_executable(mask_test mask_test.cpp)
add_dependencies(mask_test CLIc)
target_link_libraries(mask_test PRIVATE CLIc::CLIc)
set_target_properties(mask_test PROPERTIES FOLDER "Tests")
target_compile_features(mask_test PRIVATE cxx_std_17)

add_executable(masked_voronoi_labeling_test masked_voronoi_labeling_test.cpp)
add_dependencies(masked_voronoi_labeling_test CLIc)
target_link_libraries(masked_voronoi_labeling_test PRIVATE CLIc::CLIc)
set_target_properties(masked_voronoi_labeling_test PROPERTIES FOLDER "Tests")
target_compile_features(masked_voronoi_labeling_test PRIVATE cxx_std_17)

add_executable(maximum_all_pixels_test maximum_all_pixels_test.cpp)
add_dependencies(maximum_all_pixels_test CLIc)
target_link_libraries(maximum_all_pixels_test PRIVATE CLIc::CLIc)
set_target_properties(maximum_all_pixels_test PROPERTIES FOLDER "Tests")
target_compile_features(maximum_all_pixels_test PRIVATE cxx_std_17)

add_executable(maximum_box_test maximum_box_test.cpp)
add_dependencies(maximum_box_test CLIc)
target_link_libraries(maximum_box_test PRIVATE CLIc::CLIc)
set_target_properties(maximum_box_test PROPERTIES FOLDER "Tests")
target_compile_features(maximum_box_test PRIVATE cxx_std_17)

add_executable(maximum_x_projection_test maximum_x_projection_test.cpp)
add_dependencies(maximum_x_projection_test CLIc)
target_link_libraries(maximum_x_projection_test PRIVATE CLIc::CLIc)
set_target_properties(maximum_x_projection_test PROPERTIES FOLDER "Tests")
target_compile_features(maximum_x_projection_test PRIVATE cxx_std_17)

add_executable(maximum_y_projection_test maximum_y_projection_test.cpp)
add_dependencies(maximum_y_projection_test CLIc)
target_link_libraries(maximum_y_projection_test PRIVATE CLIc::CLIc)
set_target_properties(maximum_y_projection_test PROPERTIES FOLDER "Tests")
target_compile_features(maximum_y_projection_test PRIVATE cxx_std_17)

add_executable(maximum_z_projection_test maximum_z_projection_test.cpp)
add_dependencies(maximum_z_projection_test CLIc)
target_link_libraries(maximum_z_projection_test PRIVATE CLIc::CLIc)
set_target_properties(maximum_z_projection_test PROPERTIES FOLDER "Tests")
target_compile_features(maximum_z_projection_test PRIVATE cxx_std_17)

add_executable(mean_box_test mean_box_test.cpp)
add_dependencies(mean_box_test CLIc)
target_link_libraries(mean_box_test PRIVATE CLIc::CLIc)
set_target_properties(mean_box_test PROPERTIES FOLDER "Tests")
target_compile_features(mean_box_test PRIVATE cxx_std_17)

add_executable(mean_sphere_test mean_sphere_test.cpp)
add_dependencies(mean_sphere_test CLIc)
target_link_libraries(mean_sphere_test PRIVATE CLIc::CLIc)
set_target_properties(mean_sphere_test PROPERTIES FOLDER "Tests")
target_compile_features(mean_sphere_test PRIVATE cxx_std_17)

add_executable(minimum_all_pixels_test minimum_all_pixels_test.cpp)
add_dependencies(minimum_all_pixels_test CLIc)
target_link_libraries(minimum_all_pixels_test PRIVATE CLIc::CLIc)
set_target_properties(minimum_all_pixels_test PROPERTIES FOLDER "Tests")
target_compile_features(minimum_all_pixels_test PRIVATE cxx_std_17)

add_executable(minimum_box_test minimum_box_test.cpp)
add_dependencies(minimum_box_test CLIc)
target_link_libraries(minimum_box_test PRIVATE CLIc::CLIc)
set_target_properties(minimum_box_test PROPERTIES FOLDER "Tests")
target_compile_features(minimum_box_test PRIVATE cxx_std_17)

add_executable(minimum_x_projection_test minimum_x_projection_test.cpp)
add_dependencies(minimum_x_projection_test CLIc)
target_link_libraries(minimum_x_projection_test PRIVATE CLIc::CLIc)
set_target_properties(minimum_x_projection_test PROPERTIES FOLDER "Tests")
target_compile_features(minimum_x_projection_test PRIVATE cxx_std_17)

add_executable(minimum_y_projection_test minimum_y_projection_test.cpp)
add_dependencies(minimum_y_projection_test CLIc)
target_link_libraries(minimum_y_projection_test PRIVATE CLIc::CLIc)
set_target_properties(minimum_y_projection_test PROPERTIES FOLDER "Tests")
target_compile_features(minimum_y_projection_test PRIVATE cxx_std_17)

add_executable(minimum_z_projection_test minimum_z_projection_test.cpp)
add_dependencies(minimum_z_projection_test CLIc)
target_link_libraries(minimum_z_projection_test PRIVATE CLIc::CLIc)
set_target_properties(minimum_z_projection_test PROPERTIES FOLDER "Tests")
target_compile_features(minimum_z_projection_test PRIVATE cxx_std_17)

add_executable(nonzero_minimum_box_test nonzero_minimum_box_test.cpp)
add_dependencies(nonzero_minimum_box_test CLIc)
target_link_libraries(nonzero_minimum_box_test PRIVATE CLIc::CLIc)
set_target_properties(nonzero_minimum_box_test PROPERTIES FOLDER "Tests")
target_compile_features(nonzero_minimum_box_test PRIVATE cxx_std_17)

add_executable(not_equal_constant_test not_equal_constant_test.cpp)
add_dependencies(not_equal_constant_test CLIc)
target_link_libraries(not_equal_constant_test PRIVATE CLIc::CLIc)
set_target_properties(not_equal_constant_test PROPERTIES FOLDER "Tests")
target_compile_features(not_equal_constant_test PRIVATE cxx_std_17)

add_executable(not_equal_test not_equal_test.cpp)
add_dependencies(not_equal_test CLIc)
target_link_libraries(not_equal_test PRIVATE CLIc::CLIc)
set_target_properties(not_equal_test PROPERTIES FOLDER "Tests")
target_compile_features(not_equal_test PRIVATE cxx_std_17)

add_executable(onlyzero_overwrite_maximum_box_test onlyzero_overwrite_maximum_box_test.cpp)
add_dependencies(onlyzero_overwrite_maximum_box_test CLIc)
target_link_libraries(onlyzero_overwrite_maximum_box_test PRIVATE CLIc::CLIc)
set_target_properties(onlyzero_overwrite_maximum_box_test PROPERTIES FOLDER "Tests")
target_compile_features(onlyzero_overwrite_maximum_box_test PRIVATE cxx_std_17)

add_executable(onlyzero_overwrite_maximum_diamond_test onlyzero_overwrite_maximum_diamond_test.cpp)
add_dependencies(onlyzero_overwrite_maximum_diamond_test CLIc)
target_link_libraries(onlyzero_overwrite_maximum_diamond_test PRIVATE CLIc::CLIc)
set_target_properties(onlyzero_overwrite_maximum_diamond_test PROPERTIES FOLDER "Tests")
target_compile_features(onlyzero_overwrite_maximum_diamond_test PRIVATE cxx_std_17)

add_executable(replace_intensities_test replace_intensities_test.cpp)
add_dependencies(replace_intensities_test CLIc)
target_link_libraries(replace_intensities_test PRIVATE CLIc::CLIc)
set_target_properties(replace_intensities_test PROPERTIES FOLDER "Tests")
target_compile_features(replace_intensities_test PRIVATE cxx_std_17)

add_executable(replace_intensity_test replace_intensity_test.cpp)
add_dependencies(replace_intensity_test CLIc)
target_link_libraries(replace_intensity_test PRIVATE CLIc::CLIc)
set_target_properties(replace_intensity_test PROPERTIES FOLDER "Tests")
target_compile_features(replace_intensity_test PRIVATE cxx_std_17)

add_executable(set_column_test set_column_test.cpp)
add_dependencies(set_column_test CLIc)
target_link_libraries(set_column_test PRIVATE CLIc::CLIc)
set_target_properties(set_column_test PROPERTIES FOLDER "Tests")
target_compile_features(set_column_test PRIVATE cxx_std_17)

add_executable(set_nonzero_pixels_to_pixelindex_test set_nonzero_pixels_to_pixelindex_test.cpp)
add_dependencies(set_nonzero_pixels_to_pixelindex_test CLIc)
target_link_libraries(set_nonzero_pixels_to_pixelindex_test PRIVATE CLIc::CLIc)
set_target_properties(set_nonzero_pixels_to_pixelindex_test PROPERTIES FOLDER "Tests")
target_compile_features(set_nonzero_pixels_to_pixelindex_test PRIVATE cxx_std_17)

add_executable(set_test set_test.cpp)
add_dependencies(set_test CLIc)
target_link_libraries(set_test PRIVATE CLIc::CLIc)
set_target_properties(set_test PROPERTIES FOLDER "Tests")
target_compile_features(set_test PRIVATE cxx_std_17)

add_executable(smaller_constant_test smaller_constant_test.cpp)
add_dependencies(smaller_constant_test CLIc)
target_link_libraries(smaller_constant_test PRIVATE CLIc::CLIc)
set_target_properties(smaller_constant_test PROPERTIES FOLDER "Tests")
target_compile_features(smaller_constant_test PRIVATE cxx_std_17)

add_executable(smaller_or_equal_constant_test smaller_or_equal_constant_test.cpp)
add_dependencies(smaller_or_equal_constant_test CLIc)
target_link_libraries(smaller_or_equal_constant_test PRIVATE CLIc::CLIc)
set_target_properties(smaller_or_equal_constant_test PROPERTIES FOLDER "Tests")
target_compile_features(smaller_or_equal_constant_test PRIVATE cxx_std_17)

add_executable(smaller_or_equal_test smaller_or_equal_test.cpp)
add_dependencies(smaller_or_equal_test CLIc)
target_link_libraries(smaller_or_equal_test PRIVATE CLIc::CLIc)
set_target_properties(smaller_or_equal_test PROPERTIES FOLDER "Tests")
target_compile_features(smaller_or_equal_test PRIVATE cxx_std_17)

add_executable(smaller_test smaller_test.cpp)
add_dependencies(smaller_test CLIc)
target_link_libraries(smaller_test PRIVATE CLIc::CLIc)
set_target_properties(smaller_test PROPERTIES FOLDER "Tests")
target_compile_features(smaller_test PRIVATE cxx_std_17)

add_executable(sobel_test sobel_test.cpp)
add_dependencies(sobel_test CLIc)
target_link_libraries(sobel_test PRIVATE CLIc::CLIc)
set_target_properties(sobel_test PROPERTIES FOLDER "Tests")
target_compile_features(sobel_test PRIVATE cxx_std_17)

add_executable(sum_all_pixels_test sum_all_pixels_test.cpp)
add_dependencies(sum_all_pixels_test CLIc)
target_link_libraries(sum_all_pixels_test PRIVATE CLIc::CLIc)
set_target_properties(sum_all_pixels_test PROPERTIES FOLDER "Tests")
target_compile_features(sum_all_pixels_test PRIVATE cxx_std_17)

add_executable(sum_reduction_x_test sum_reduction_x_test.cpp)
add_dependencies(sum_reduction_x_test CLIc)
target_link_libraries(sum_reduction_x_test PRIVATE CLIc::CLIc)
set_target_properties(sum_reduction_x_test PROPERTIES FOLDER "Tests")
target_compile_features(sum_reduction_x_test PRIVATE cxx_std_17)

add_executable(sum_x_projection_test sum_x_projection_test.cpp)
add_dependencies(sum_x_projection_test CLIc)
target_link_libraries(sum_x_projection_test PRIVATE CLIc::CLIc)
set_target_properties(sum_x_projection_test PROPERTIES FOLDER "Tests")
target_compile_features(sum_x_projection_test PRIVATE cxx_std_17)

add_executable(sum_y_projection_test sum_y_projection_test.cpp)
add_dependencies(sum_y_projection_test CLIc)
target_link_libraries(sum_y_projection_test PRIVATE CLIc::CLIc)
set_target_properties(sum_y_projection_test PROPERTIES FOLDER "Tests")
target_compile_features(sum_y_projection_test PRIVATE cxx_std_17)

add_executable(sum_z_projection_test sum_z_projection_test.cpp)
add_dependencies(sum_z_projection_test CLIc)
target_link_libraries(sum_z_projection_test PRIVATE CLIc::CLIc)
set_target_properties(sum_z_projection_test PROPERTIES FOLDER "Tests")
target_compile_features(sum_z_projection_test PRIVATE cxx_std_17)

add_executable(threshold_otsu_test threshold_otsu_test.cpp)
add_dependencies(threshold_otsu_test CLIc)
target_link_libraries(threshold_otsu_test PRIVATE CLIc::CLIc)
set_target_properties(threshold_otsu_test PROPERTIES FOLDER "Tests")
target_compile_features(threshold_otsu_test PRIVATE cxx_std_17)

add_executable(voronoi_otsu_labeling_test voronoi_otsu_labeling_test.cpp)
add_dependencies(voronoi_otsu_labeling_test CLIc)
target_link_libraries(voronoi_otsu_labeling_test PRIVATE CLIc::CLIc)
set_target_properties(voronoi_otsu_labeling_test PROPERTIES FOLDER "Tests")
target_compile_features(voronoi_otsu_labeling_test PRIVATE cxx_std_17)

add_test(NAME core_gpu_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/core_gpu_test)
add_test(NAME core_scalar_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/core_scalar_test)
add_test(NAME core_buffer_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/core_buffer_test)
add_test(NAME core_image_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/core_image_test)
add_test(NAME core_kernel_test_buffer WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/core_kernel_test buffer)
# add_test(NAME core_kernel_test_image WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/core_kernel_test image)
add_test(NAME absolute_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND absolute_test)
add_test(NAME add_image_and_scalar_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND add_image_and_scalar_test)
add_test(NAME add_image_weighted_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND add_image_weighted_test)
add_test(NAME binary_and_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND binary_and_test)
add_test(NAME binary_not_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND binary_not_test)
add_test(NAME binary_or_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND binary_or_test)
add_test(NAME binary_subtract_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND binary_subtract_test)
add_test(NAME binary_xor_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND binary_xor_test)
add_test(NAME block_enumerate_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND block_enumerate_test)
add_test(NAME close_indexgaps_in_labelmap_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND close_indexgaps_in_labelmap_test)
add_test(NAME connected_component_labeling_box_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND connected_component_labeling_box_test)
add_test(NAME copy_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND copy_test)
add_test(NAME detect_maxima_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND detect_maxima_test)
add_test(NAME difference_of_gaussian_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND difference_of_gaussian_test)
add_test(NAME dilate_sphere_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND dilate_sphere_test)
add_test(NAME equal_constant_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND equal_constant_test)
add_test(NAME equal_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND equal_test)
add_test(NAME erode_sphere_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND erode_sphere_test)
add_test(NAME extend_labeling_via_voronoi_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND extend_labeling_via_voronoi_test)
add_test(NAME flag_existing_labels_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND flag_existing_labels_test)
add_test(NAME gaussian_blur_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND gaussian_blur_test)
add_test(NAME greater_constant_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND greater_constant_test)
add_test(NAME greater_or_equal_constant_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND greater_or_equal_constant_test)
add_test(NAME greater_or_equal_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND greater_or_equal_test)
add_test(NAME greater_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND greater_test)
add_test(NAME histogram_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND histogram_test)
add_test(NAME mask_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND mask_test)
add_test(NAME masked_voronoi_labeling_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND masked_voronoi_labeling_test)
add_test(NAME maximum_all_pixels_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND maximum_all_pixels_test)
add_test(NAME maximum_box_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND maximum_box_test)
add_test(NAME maximum_x_projection_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND maximum_x_projection_test)
add_test(NAME maximum_y_projection_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND maximum_y_projection_test)
add_test(NAME maximum_z_projection_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND maximum_z_projection_test)
add_test(NAME mean_box_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND mean_box_test)
add_test(NAME mean_sphere_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND mean_sphere_test)
add_test(NAME minimum_all_pixels_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND minimum_all_pixels_test)
add_test(NAME minimum_box_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND minimum_box_test)
add_test(NAME minimum_x_projection_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND minimum_x_projection_test)
add_test(NAME minimum_y_projection_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND minimum_y_projection_test)
add_test(NAME minimum_z_projection_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND minimum_z_projection_test)
add_test(NAME nonzero_minimum_box_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND nonzero_minimum_box_test)
add_test(NAME not_equal_constant_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND not_equal_constant_test)
add_test(NAME not_equal_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND not_equal_test)
add_test(NAME onlyzero_overwrite_maximum_box_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND onlyzero_overwrite_maximum_box_test)
add_test(NAME onlyzero_overwrite_maximum_diamond_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND onlyzero_overwrite_maximum_diamond_test)
add_test(NAME replace_intensities_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND replace_intensities_test)
add_test(NAME replace_intensity_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND replace_intensity_test)
add_test(NAME set_column_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND set_column_test)
add_test(NAME set_nonzero_pixels_to_pixelindex_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND set_nonzero_pixels_to_pixelindex_test)
add_test(NAME set_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND set_test)
add_test(NAME smaller_constant_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND smaller_constant_test)
add_test(NAME smaller_or_equal_constant_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND smaller_or_equal_constant_test)
add_test(NAME smaller_or_equal_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND smaller_or_equal_test)
add_test(NAME smaller_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND smaller_test)
add_test(NAME sobel_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND sobel_test)
add_test(NAME sum_all_pixels_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND sum_all_pixels_test)
add_test(NAME sum_reduction_x_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND sum_reduction_x_test)
add_test(NAME sum_x_projection_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND sum_x_projection_test)
add_test(NAME sum_y_projection_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND sum_y_projection_test)
add_test(NAME sum_z_projection_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND sum_z_projection_test)
add_test(NAME threshold_otsu_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND threshold_otsu_test)
add_test(NAME voronoi_otsu_labeling_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND voronoi_otsu_labeling_test)

set_tests_properties(
    core_gpu_test
    core_scalar_test
    core_buffer_test
    # core_image_test
    core_kernel_test_buffer
    # core_kernel_test_image
    PROPERTIES LABELS "CORE" WILL_FAIL FALSE
)

set_tests_properties(absolute_test
    add_image_and_scalar_test
    add_image_weighted_test
    binary_and_test
    binary_not_test
    binary_or_test
    binary_subtract_test
    binary_xor_test
    block_enumerate_test
    close_indexgaps_in_labelmap_test
    connected_component_labeling_box_test
    copy_test
    detect_maxima_test
    difference_of_gaussian_test
    dilate_sphere_test
    equal_constant_test
    equal_test
    erode_sphere_test
    extend_labeling_via_voronoi_test
    flag_existing_labels_test
    gaussian_blur_test
    greater_constant_test
    greater_or_equal_constant_test
    greater_or_equal_test
    greater_test
    histogram_test
    mask_test
    masked_voronoi_labeling_test
    maximum_all_pixels_test
    maximum_box_test
    maximum_x_projection_test
    maximum_y_projection_test
    maximum_z_projection_test
    mean_box_test
    mean_sphere_test
    minimum_all_pixels_test
    minimum_box_test
    minimum_x_projection_test
    minimum_y_projection_test
    minimum_z_projection_test
    nonzero_minimum_box_test
    not_equal_constant_test
    not_equal_test
    onlyzero_overwrite_maximum_box_test
    onlyzero_overwrite_maximum_diamond_test
    replace_intensities_test
    replace_intensity_test
    set_column_test
    set_nonzero_pixels_to_pixelindex_test
    set_test
    smaller_constant_test
    smaller_or_equal_constant_test
    smaller_or_equal_test
    smaller_test
    sobel_test
    sum_all_pixels_test
    sum_reduction_x_test
    sum_x_projection_test
    sum_y_projection_test
    sum_z_projection_test
    threshold_otsu_test
    voronoi_otsu_labeling_test PROPERTIES LABELS "KERNELS" WILL_FAIL FALSE)