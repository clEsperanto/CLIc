


add_executable(operations_test operations_test.cpp)
add_dependencies(operations_test CLIc) 
target_link_libraries(operations_test PRIVATE CLIc::CLIc)
add_test(NAME operations_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/operations_test)

add_executable(add_image_and_scalar_test add_image_and_scalar_test.cpp)
add_dependencies(add_image_and_scalar_test CLIc) 
target_link_libraries(add_image_and_scalar_test PRIVATE CLIc::CLIc)
add_test(NAME add_image_and_scalar_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/add_image_and_scalar_test)

add_executable(copy_test copy_test.cpp)
add_dependencies(copy_test CLIc) 
target_link_libraries(copy_test PRIVATE CLIc::CLIc)
add_test(NAME copy_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/copy_test)

add_executable(gaussian_blur_test gaussian_blur_test.cpp)
add_dependencies(gaussian_blur_test CLIc) 
target_link_libraries(gaussian_blur_test PRIVATE CLIc::CLIc)
add_test(NAME gaussian_blur_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gaussian_blur_test)

add_executable(maximum_x_projection_test maximum_x_projection_test.cpp)
add_dependencies(maximum_x_projection_test CLIc) 
target_link_libraries(maximum_x_projection_test PRIVATE CLIc::CLIc)
add_test(NAME maximum_x_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/maximum_x_projection_test)

add_executable(maximum_y_projection_test maximum_y_projection_test.cpp)
add_dependencies(maximum_y_projection_test CLIc) 
target_link_libraries(maximum_y_projection_test PRIVATE CLIc::CLIc)
add_test(NAME maximum_y_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/maximum_y_projection_test)

add_executable(maximum_z_projection_test maximum_z_projection_test.cpp)
add_dependencies(maximum_z_projection_test CLIc) 
target_link_libraries(maximum_z_projection_test PRIVATE CLIc::CLIc)
add_test(NAME maximum_z_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/maximum_z_projection_test)

add_executable(maximum_all_pixels_test maximum_all_pixels_test.cpp)
add_dependencies(maximum_all_pixels_test CLIc) 
target_link_libraries(maximum_all_pixels_test PRIVATE CLIc::CLIc)
add_test(NAME maximum_all_pixels_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/maximum_all_pixels_test)

add_executable(mean_sphere_test mean_sphere_test.cpp)
add_dependencies(mean_sphere_test CLIc) 
target_link_libraries(mean_sphere_test PRIVATE CLIc::CLIc)
add_test(NAME mean_sphere_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/mean_sphere_test)

add_executable(mean_box_test mean_box_test.cpp)
add_dependencies(mean_box_test CLIc) 
target_link_libraries(mean_box_test PRIVATE CLIc::CLIc)
add_test(NAME mean_box_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/mean_box_test)

add_executable(block_enumerate_test block_enumerate_test.cpp)
add_dependencies(block_enumerate_test CLIc) 
target_link_libraries(block_enumerate_test PRIVATE CLIc::CLIc)
add_test(NAME block_enumerate_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/block_enumerate_test)

add_executable(connected_component_labelling_box_test connected_component_labelling_box_test.cpp)
add_dependencies(connected_component_labelling_box_test CLIc) 
target_link_libraries(connected_component_labelling_box_test PRIVATE CLIc::CLIc)
add_test(NAME connected_component_labelling_box_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/connected_component_labelling_box_test)

add_executable(detect_maxima_test detect_maxima_test.cpp)
add_dependencies(detect_maxima_test CLIc) 
target_link_libraries(detect_maxima_test PRIVATE CLIc::CLIc)
add_test(NAME detect_maxima_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/detect_maxima_test)

add_executable(difference_of_gaussian_test difference_of_gaussian_test.cpp)
add_dependencies(difference_of_gaussian_test CLIc) 
target_link_libraries(difference_of_gaussian_test PRIVATE CLIc::CLIc)
add_test(NAME difference_of_gaussian_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/difference_of_gaussian_test)

add_executable(equal_constant_test equal_constant_test.cpp)
add_dependencies(equal_constant_test CLIc) 
target_link_libraries(equal_constant_test PRIVATE CLIc::CLIc)
add_test(NAME equal_constant_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/equal_constant_test)

add_executable(equal_test equal_test.cpp)
add_dependencies(equal_test CLIc) 
target_link_libraries(equal_test PRIVATE CLIc::CLIc)
add_test(NAME equal_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/equal_test)

add_executable(flag_existing_labels_test flag_existing_labels_test.cpp)
add_dependencies(flag_existing_labels_test CLIc) 
target_link_libraries(flag_existing_labels_test PRIVATE CLIc::CLIc)
add_test(NAME flag_existing_labels_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/flag_existing_labels_test)

add_executable(greater_constant_test greater_constant_test.cpp)
add_dependencies(greater_constant_test CLIc) 
target_link_libraries(greater_constant_test PRIVATE CLIc::CLIc)
add_test(NAME greater_constant_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/greater_constant_test)

add_executable(greater_or_equal_constant_test greater_or_equal_constant_test.cpp)
add_dependencies(greater_or_equal_constant_test CLIc) 
target_link_libraries(greater_or_equal_constant_test PRIVATE CLIc::CLIc)
add_test(NAME greater_or_equal_constant_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/greater_or_equal_constant_test)

add_executable(greater_test greater_test.cpp)
add_dependencies(greater_test CLIc) 
target_link_libraries(greater_test PRIVATE CLIc::CLIc)
add_test(NAME greater_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/greater_test)

add_executable(minimum_all_pixels_test minimum_all_pixels_test.cpp)
add_dependencies(minimum_all_pixels_test CLIc) 
target_link_libraries(minimum_all_pixels_test PRIVATE CLIc::CLIc)
add_test(NAME minimum_all_pixels_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/minimum_all_pixels_test)

add_executable(minimum_test minimum_test.cpp)
add_dependencies(minimum_test CLIc) 
target_link_libraries(minimum_test PRIVATE CLIc::CLIc)
add_test(NAME minimum_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/minimum_test)

add_executable(minimum_x_projection_test minimum_x_projection_test.cpp)
add_dependencies(minimum_x_projection_test CLIc) 
target_link_libraries(minimum_x_projection_test PRIVATE CLIc::CLIc)
add_test(NAME minimum_x_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/minimum_x_projection_test)

add_executable(minimum_y_projection_test minimum_y_projection_test.cpp)
add_dependencies(minimum_y_projection_test CLIc) 
target_link_libraries(minimum_y_projection_test PRIVATE CLIc::CLIc)
add_test(NAME minimum_y_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/minimum_y_projection_test)

add_executable(minimum_z_projection_test minimum_z_projection_test.cpp)
add_dependencies(minimum_z_projection_test CLIc) 
target_link_libraries(minimum_z_projection_test PRIVATE CLIc::CLIc)
add_test(NAME minimum_z_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/minimum_z_projection_test)

add_executable(nonzero_minimum_box_test nonzero_minimum_box_test.cpp)
add_dependencies(nonzero_minimum_box_test CLIc) 
target_link_libraries(nonzero_minimum_box_test PRIVATE CLIc::CLIc)
add_test(NAME nonzero_minimum_box_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/nonzero_minimum_box_test)

add_executable(not_equal_constant_test not_equal_constant_test.cpp)
add_dependencies(not_equal_constant_test CLIc) 
target_link_libraries(not_equal_constant_test PRIVATE CLIc::CLIc)
add_test(NAME not_equal_constant_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/not_equal_constant_test)

add_executable(not_equal_test not_equal_test.cpp)
add_dependencies(not_equal_test CLIc) 
target_link_libraries(not_equal_test PRIVATE CLIc::CLIc)
add_test(NAME not_equal_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/not_equal_test)

add_executable(replace_intensities_test replace_intensities_test.cpp)
add_dependencies(replace_intensities_test CLIc) 
target_link_libraries(replace_intensities_test PRIVATE CLIc::CLIc)
add_test(NAME replace_intensities_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/replace_intensities_test)

add_executable(replace_intensity_test replace_intensity_test.cpp)
add_dependencies(replace_intensity_test CLIc) 
target_link_libraries(replace_intensity_test PRIVATE CLIc::CLIc)
add_test(NAME replace_intensity_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/replace_intensity_test)

add_executable(set_column_test set_column_test.cpp)
add_dependencies(set_column_test CLIc) 
target_link_libraries(set_column_test PRIVATE CLIc::CLIc)
add_test(NAME set_column_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/set_column_test)

add_executable(set_nonzero_pixels_to_pixelindex_test set_nonzero_pixels_to_pixelindex_test.cpp)
add_dependencies(set_nonzero_pixels_to_pixelindex_test CLIc) 
target_link_libraries(set_nonzero_pixels_to_pixelindex_test PRIVATE CLIc::CLIc)
add_test(NAME set_nonzero_pixels_to_pixelindex_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/set_nonzero_pixels_to_pixelindex_test)

add_executable(set_test set_test.cpp)
add_dependencies(set_test CLIc) 
target_link_libraries(set_test PRIVATE CLIc::CLIc)
add_test(NAME set_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/set_test)

add_executable(smaller_constant_test smaller_constant_test.cpp)
add_dependencies(smaller_constant_test CLIc) 
target_link_libraries(smaller_constant_test PRIVATE CLIc::CLIc)
add_test(NAME smaller_constant_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/smaller_constant_test)

add_executable(smaller_or_equal_constant_test smaller_or_equal_constant_test.cpp)
add_dependencies(smaller_or_equal_constant_test CLIc) 
target_link_libraries(smaller_or_equal_constant_test PRIVATE CLIc::CLIc)
add_test(NAME smaller_or_equal_constant_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/smaller_or_equal_constant_test)

add_executable(smaller_or_equal_test smaller_or_equal_test.cpp)
add_dependencies(smaller_or_equal_test CLIc) 
target_link_libraries(smaller_or_equal_test PRIVATE CLIc::CLIc)
add_test(NAME smaller_or_equal_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/smaller_or_equal_test)

add_executable(smaller_test smaller_test.cpp)
add_dependencies(smaller_test CLIc) 
target_link_libraries(smaller_test PRIVATE CLIc::CLIc)
add_test(NAME smaller_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/smaller_test)

add_executable(sobel_test sobel_test.cpp)
add_dependencies(sobel_test CLIc) 
target_link_libraries(sobel_test PRIVATE CLIc::CLIc)
add_test(NAME sobel_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/sobel_test)

add_executable(sum_all_pixels_test sum_all_pixels_test.cpp)
add_dependencies(sum_all_pixels_test CLIc) 
target_link_libraries(sum_all_pixels_test PRIVATE CLIc::CLIc)
add_test(NAME sum_all_pixels_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/sum_all_pixels_test)

add_executable(sum_reduction_x_test sum_reduction_x_test.cpp)
add_dependencies(sum_reduction_x_test CLIc) 
target_link_libraries(sum_reduction_x_test PRIVATE CLIc::CLIc)
add_test(NAME sum_reduction_x_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/sum_reduction_x_test)

add_executable(sum_x_projection_test sum_x_projection_test.cpp)
add_dependencies(sum_x_projection_test CLIc) 
target_link_libraries(sum_x_projection_test PRIVATE CLIc::CLIc)
add_test(NAME sum_x_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/sum_x_projection_test)

add_executable(sum_y_projection_test sum_y_projection_test.cpp)
add_dependencies(sum_y_projection_test CLIc) 
target_link_libraries(sum_y_projection_test PRIVATE CLIc::CLIc)
add_test(NAME sum_y_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/sum_y_projection_test)

add_executable(sum_z_projection_test sum_z_projection_test.cpp)
add_dependencies(sum_z_projection_test CLIc) 
target_link_libraries(sum_z_projection_test PRIVATE CLIc::CLIc)
add_test(NAME sum_z_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/sum_z_projection_test)