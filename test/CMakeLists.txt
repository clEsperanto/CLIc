
# core_functions_test
add_executable(data_test data_test.cpp)
add_dependencies(data_test CLIc) # wait for CLIc to be compiled
target_link_libraries(data_test CLIc)

# core_functions_test
add_executable(push_functions_test push_functions_test.cpp)
add_dependencies(push_functions_test CLIc) # wait for CLIc to be compiled
target_link_libraries(push_functions_test CLIc)

# core_functions_test
add_executable(create_functions_test create_functions_test.cpp)
add_dependencies(create_functions_test CLIc) # wait for CLIc to be compiled
target_link_libraries(create_functions_test CLIc)

# core_functions_test
add_executable(pull_functions_test pull_functions_test.cpp)
add_dependencies(pull_functions_test CLIc) # wait for CLIc to be compiled
target_link_libraries(pull_functions_test CLIc)

# add_image_and_scalar_test
add_executable(add_image_and_scalar_test add_image_and_scalar_test.cpp)
add_dependencies(add_image_and_scalar_test CLIc) # wait for CLIc to be compiled
target_link_libraries(add_image_and_scalar_test CLIc)

# equal_test
add_executable(equal_test equal_test.cpp)
add_dependencies(equal_test CLIc) # wait for CLIc to be compiled
target_link_libraries(equal_test CLIc)

# equal_constant_test
add_executable(equal_constant_test equal_constant_test.cpp)
add_dependencies(equal_constant_test CLIc) # wait for CLIc to be compiled
target_link_libraries(equal_constant_test CLIc)

# greater_test
add_executable(greater_test greater_test.cpp)
add_dependencies(greater_test CLIc) # wait for CLIc to be compiled
target_link_libraries(greater_test CLIc)

# greater_or_equal_test
add_executable(greater_or_equal_test greater_or_equal_test.cpp)
add_dependencies(greater_or_equal_test CLIc) # wait for CLIc to be compiled
target_link_libraries(greater_or_equal_test CLIc)

# greater_constant_test
add_executable(greater_constant_test greater_constant_test.cpp)
add_dependencies(greater_constant_test CLIc) # wait for CLIc to be compiled
target_link_libraries(greater_constant_test CLIc)

# greater_or_equal_constant_test
add_executable(greater_or_equal_constant_test greater_or_equal_constant_test.cpp)
add_dependencies(greater_or_equal_constant_test CLIc) # wait for CLIc to be compiled
target_link_libraries(greater_or_equal_constant_test CLIc)

# maximum_z_projection_test
add_executable(maximum_z_projection_test maximum_z_projection_test.cpp)
add_dependencies(maximum_z_projection_test CLIc) # wait for CLIc to be compiled
target_link_libraries(maximum_z_projection_test CLIc)

# maximum_y_projection_test
add_executable(maximum_y_projection_test maximum_y_projection_test.cpp)
add_dependencies(maximum_y_projection_test CLIc) # wait for CLIc to be compiled
target_link_libraries(maximum_y_projection_test CLIc)

# maximum_x_projection_test
add_executable(maximum_x_projection_test maximum_x_projection_test.cpp)
add_dependencies(maximum_x_projection_test CLIc) # wait for CLIc to be compiled
target_link_libraries(maximum_x_projection_test CLIc)

# maximum_all_pixels_test
add_executable(maximum_all_pixels_test maximum_all_pixels_test.cpp)
add_dependencies(maximum_all_pixels_test CLIc) # wait for CLIc to be compiled
target_link_libraries(maximum_all_pixels_test CLIc)

# minimum_x_projection_test
add_executable(minimum_x_projection_test minimum_x_projection_test.cpp)
add_dependencies(minimum_x_projection_test CLIc) # wait for CLIc to be compiled
target_link_libraries(minimum_x_projection_test CLIc)

# minimum_y_projection_test
add_executable(minimum_y_projection_test minimum_y_projection_test.cpp)
add_dependencies(minimum_y_projection_test CLIc) # wait for CLIc to be compiled
target_link_libraries(minimum_y_projection_test CLIc)

# minimum_z_projection_test
add_executable(minimum_z_projection_test minimum_z_projection_test.cpp)
add_dependencies(minimum_z_projection_test CLIc) # wait for CLIc to be compiled
target_link_libraries(minimum_z_projection_test CLIc)

# minimum_all_pixels_test
add_executable(minimum_all_pixels_test minimum_all_pixels_test.cpp)
add_dependencies(minimum_all_pixels_test CLIc) # wait for CLIc to be compiled
target_link_libraries(minimum_all_pixels_test CLIc)

# mean2d_sphere_test
add_executable(mean2d_sphere_test mean2d_sphere_test.cpp)
add_dependencies(mean2d_sphere_test CLIc) # wait for CLIc to be compiled
target_link_libraries(mean2d_sphere_test CLIc)

# nonzero_minimum_box_test
add_executable(nonzero_minimum_box_test nonzero_minimum_box_test.cpp)
add_dependencies(nonzero_minimum_box_test CLIc) # wait for CLIc to be compiled
target_link_libraries(nonzero_minimum_box_test CLIc)

# not_equal_test
add_executable(not_equal_test not_equal_test.cpp)
add_dependencies(not_equal_test CLIc) # wait for CLIc to be compiled
target_link_libraries(not_equal_test CLIc)

# equal_constant_test
add_executable(not_equal_constant_test not_equal_constant_test.cpp)
add_dependencies(not_equal_constant_test CLIc) # wait for CLIc to be compiled
target_link_libraries(not_equal_constant_test CLIc)

# smaller_test
add_executable(smaller_test smaller_test.cpp)
add_dependencies(smaller_test CLIc) # wait for CLIc to be compiled
target_link_libraries(smaller_test CLIc)

# set_test
add_executable(set_test set_test.cpp)
add_dependencies(set_test CLIc) # wait for CLIc to be compiled
target_link_libraries(set_test CLIc)

# set_test
add_executable(set_nonzero_pixels_to_pixelindex_test set_nonzero_pixels_to_pixelindex_test.cpp)
add_dependencies(set_nonzero_pixels_to_pixelindex_test CLIc) # wait for CLIc to be compiled
target_link_libraries(set_nonzero_pixels_to_pixelindex_test CLIc)

# smaller_or_equal_test
add_executable(smaller_or_equal_test smaller_or_equal_test.cpp)
add_dependencies(smaller_or_equal_test CLIc) # wait for CLIc to be compiled
target_link_libraries(smaller_or_equal_test CLIc)

# smaller_constant_test
add_executable(smaller_constant_test smaller_constant_test.cpp)
add_dependencies(smaller_constant_test CLIc) # wait for CLIc to be compiled
target_link_libraries(smaller_constant_test CLIc)

# smaller_or_equal_constant_test
add_executable(smaller_or_equal_constant_test smaller_or_equal_constant_test.cpp)
add_dependencies(smaller_or_equal_constant_test CLIc) # wait for CLIc to be compiled
target_link_libraries(smaller_or_equal_constant_test CLIc)

# absolute_test
add_executable(absolute_test absolute_test.cpp)
add_dependencies(absolute_test CLIc) # wait for CLIc to be compiled
target_link_libraries(absolute_test CLIc)

# sobel_test
add_executable(sobel_test sobel_test.cpp)
add_dependencies(sobel_test CLIc) # wait for CLIc to be compiled
target_link_libraries(sobel_test CLIc)

# maximum_test
add_executable(maximum_test maximum_test.cpp)
add_dependencies(maximum_test CLIc) # wait for CLIc to be compiled
target_link_libraries(maximum_test CLIc)

# minimum_test
add_executable(minimum_test minimum_test.cpp)
add_dependencies(minimum_test CLIc) # wait for CLIc to be compiled
target_link_libraries(minimum_test CLIc)

# tests
add_test(NAME data_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/data_test)
add_test(NAME push_functions_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/push_functions_test)
add_test(NAME create_functions_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/create_functions_test)
add_test(NAME pull_functions_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/pull_functions_test)
add_test(NAME add_image_and_scalar_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/add_image_and_scalar_test)
add_test(NAME maximum_z_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/maximum_z_projection_test)
add_test(NAME maximum_y_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/maximum_y_projection_test)
add_test(NAME maximum_x_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/maximum_x_projection_test)
add_test(NAME maximum_all_pixels_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/maximum_all_pixels_test)
add_test(NAME minimum_all_pixels_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/minimum_all_pixels_test)
add_test(NAME minimum_z_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/minimum_z_projection_test)
add_test(NAME minimum_y_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/minimum_y_projection_test)
add_test(NAME minimum_x_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/minimum_x_projection_test)
add_test(NAME mean2d_sphere_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/mean2d_sphere_test)
add_test(NAME absolute_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/absolute_test)
add_test(NAME sobel_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/sobel_test)
add_test(NAME equal_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/equal_test)
add_test(NAME equal_constant_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/equal_constant_test)
add_test(NAME greater_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/greater_test)
add_test(NAME greater_or_equal_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/greater_or_equal_test)
add_test(NAME greater_constant_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/greater_constant_test)
add_test(NAME greater_or_equal_constant_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/greater_or_equal_constant_test)
add_test(NAME nonzero_minimum_box_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/nonzero_minimum_box_test)
add_test(NAME not_equal_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/not_equal_test)
add_test(NAME not_equal_constant_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/not_equal_constant_test)
add_test(NAME smaller_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/smaller_test)
add_test(NAME set_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/set_test)
add_test(NAME set_nonzero_pixels_to_pixelindex_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/set_nonzero_pixels_to_pixelindex_test)
add_test(NAME smaller_or_equal_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/smaller_or_equal_test)
add_test(NAME smaller_constant_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/smaller_constant_test)
add_test(NAME maximum_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/maximum_test)
add_test(NAME minimum_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/minimum_test)
