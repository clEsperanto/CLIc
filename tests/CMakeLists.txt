
project(clic_tests)

# find clic to be made
set(CLIC_LIBRARIES ${COMPILED_LIBRARY_DIR}/libClic.so)
set(CLIC_INCLUDE_DIRS ${INCLUDE_DIR} CACHE PATH "clic include folder")
include_directories(${CLIC_INCLUDE_DIRS})

## Define target and links ####################################

# Create target as executable
add_executable(add_image_and_scalar_test add_image_and_scalar_test.cpp)
add_dependencies(add_image_and_scalar_test Clic) # wait for clic to be compiled
target_link_libraries(add_image_and_scalar_test ${CLIC_LIBRARIES} OpenCL::OpenCL TIFF::TIFF)
set_target_properties(add_image_and_scalar_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${COMPILED_BINARY_DIR})

# Create target as executable
add_executable(maximum_z_projection_test maximum_z_projection_test.cpp)
add_dependencies(maximum_z_projection_test Clic) # wait for clic to be compiled
target_link_libraries(maximum_z_projection_test ${CLIC_LIBRARIES} OpenCL::OpenCL TIFF::TIFF)
set_target_properties(maximum_z_projection_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${COMPILED_BINARY_DIR})

# Create target as executable
add_executable(smaller_or_equal_constant_test smaller_or_equal_constant_test.cpp)
add_dependencies(smaller_or_equal_constant_test Clic) # wait for clic to be compiled
target_link_libraries(smaller_or_equal_constant_test ${CLIC_LIBRARIES} OpenCL::OpenCL TIFF::TIFF)
set_target_properties(smaller_or_equal_constant_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${COMPILED_BINARY_DIR})

# ## Tests ####################################
add_test(NAME add_image_and_scalar_test COMMAND ${COMPILED_BINARY_DIR}/add_image_and_scalar_test)
add_test(NAME maximum_z_projection_test COMMAND ${COMPILED_BINARY_DIR}/maximum_z_projection_test)
add_test(NAME smaller_or_equal_constant_test COMMAND ${COMPILED_BINARY_DIR}/smaller_or_equal_constant_test)