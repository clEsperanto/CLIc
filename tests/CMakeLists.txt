
set(clic_tests
add_image_and_scalar_2d_test.cpp
)

## Define target and links ####################################

# Create target as executable
add_executable(add_image_and_scalar_2d_test ${source} ${header} ${clic_tests})

# Link libraries to target
target_link_libraries(add_image_and_scalar_2d_test OpenCL::OpenCL TIFF::TIFF)

# Define executable path destination
set_target_properties(add_image_and_scalar_2d_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_dir})

# Define global variable
target_compile_definitions(add_image_and_scalar_2d_test PRIVATE -DCLI_PATH=\"${CLI_Path}\")
target_compile_definitions(add_image_and_scalar_2d_test PRIVATE -DCLP_PATH=\"${CLP_Path}\")
target_compile_definitions(add_image_and_scalar_2d_test PRIVATE -DOCL_MAJOR_VERSION=${OpenCL_VERSION_MAJOR})

## Tests ####################################

add_test(NAME add_image_and_scalar_2d_test COMMAND ${bin_dir}/add_image_and_scalar_2d_test )