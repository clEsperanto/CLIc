
# core_functions_test
add_executable(core_functions_test core_functions_test.cpp)
add_dependencies(core_functions_test Clic) # wait for clic to be compiled
target_link_libraries(core_functions_test Clic)

# add_image_and_scalar_test
add_executable(add_image_and_scalar_test add_image_and_scalar_test.cpp)
add_dependencies(add_image_and_scalar_test Clic) # wait for clic to be compiled
target_link_libraries(add_image_and_scalar_test Clic)

# maximum_z_projection_test
add_executable(maximum_z_projection_test maximum_z_projection_test.cpp)
add_dependencies(maximum_z_projection_test Clic) # wait for clic to be compiled
target_link_libraries(maximum_z_projection_test Clic)

# smaller_or_equal_constant_test
add_executable(smaller_or_equal_constant_test smaller_or_equal_constant_test.cpp)
add_dependencies(smaller_or_equal_constant_test Clic) # wait for clic to be compiled
target_link_libraries(smaller_or_equal_constant_test Clic)

# absolute_test
add_executable(absolute_test absolute_test.cpp)
add_dependencies(absolute_test Clic) # wait for clic to be compiled
target_link_libraries(absolute_test Clic)

# tests
add_test(NAME core_functions_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/core_functions_test)
add_test(NAME add_image_and_scalar_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/add_image_and_scalar_test)
add_test(NAME maximum_z_projection_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/maximum_z_projection_test)
add_test(NAME smaller_or_equal_constant_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/smaller_or_equal_constant_test)
add_test(NAME absolute_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/absolute_test)
