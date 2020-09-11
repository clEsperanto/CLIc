
## Define target and links ####################################


# Create target as executable
add_executable(add_image_and_scalar_test ${source} ${header} add_image_and_scalar_test.cpp)

# Link libraries to target
target_link_libraries(add_image_and_scalar_test OpenCL::OpenCL TIFF::TIFF)

# Define executable path destination
set_target_properties(add_image_and_scalar_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_dir})

# Define global variable
target_compile_definitions(add_image_and_scalar_test PRIVATE -DCLI_PATH=\"${CLI_Path}\")
target_compile_definitions(add_image_and_scalar_test PRIVATE -DCLP_PATH=\"${CLP_Path}\")
target_compile_definitions(add_image_and_scalar_test PRIVATE -DOCL_MAJOR_VERSION=${OpenCL_VERSION_MAJOR})



# Create target as executable
add_executable(maximum_z_projection_test ${source} ${header} maximum_z_projection_test.cpp)

# Link libraries to target
target_link_libraries(maximum_z_projection_test OpenCL::OpenCL TIFF::TIFF)

# Define executable path destination
set_target_properties(maximum_z_projection_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_dir})

# Define global variable
target_compile_definitions(maximum_z_projection_test PRIVATE -DCLI_PATH=\"${CLI_Path}\")
target_compile_definitions(maximum_z_projection_test PRIVATE -DCLP_PATH=\"${CLP_Path}\")
target_compile_definitions(maximum_z_projection_test PRIVATE -DOCL_MAJOR_VERSION=${OpenCL_VERSION_MAJOR})




## Tests ####################################
add_test(NAME add_image_and_scalar_test COMMAND ${bin_dir}/add_image_and_scalar_test )
add_test(NAME maximum_z_projection_test COMMAND ${bin_dir}/maximum_z_projection_test )