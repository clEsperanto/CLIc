# Branch or Release tag to retrive the ocl/cu files
set(CLESPERANTO_KERNEL_TAG clesperanto_kernels) # e6bc757500d848e3136aa8b9de8770207f5a3e58)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})

## Manage OpenCL Kernel Sources
FetchContent_Declare(kernels
    GIT_REPOSITORY https://github.com/clEsperanto/clij-opencl-kernels.git
    GIT_TAG        ${CLESPERANTO_KERNEL_TAG}
    BUILD_ALWAYS   OFF
)
FetchContent_GetProperties(kernels)
if(NOT kernels_POPULATED)
    FetchContent_Populate(kernels)
endif()

set(KERNELS_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/kernels-src CACHE PATH "Path to opencl kernel files (.cl)" FORCE)
mark_as_advanced(KERNELS_SOURCE_DIR)
set(KERNELS_HEADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/kernels CACHE PATH "Path to kernel files (.h)" FORCE)
mark_as_advanced(KERNELS_HEADER_DIR)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
# Define the command to generate kernels
add_custom_command(
    OUTPUT ${KERNELS_HEADER_DIR}
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/kernel_to_header.py ${KERNELS_SOURCE_DIR} ${KERNELS_HEADER_DIR}
    COMMENT "Generating headers from clesperanto kernels files."
    DEPENDS ${KERNELS_SOURCE_DIR}
)
# Define the target that depends on the output of the custom command
add_custom_target(
    generate_kernels
    DEPENDS ${KERNELS_HEADER_DIR}
)

FetchContent_Declare(eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG        3.4.0
    BUILD_ALWAYS   OFF
)
FetchContent_GetProperties(eigen)
if(NOT Eigen_POPULATED)
    FetchContent_Populate(eigen)
endif()
set(EIGEN_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/eigen-src CACHE PATH "Path to eigen lib" FORCE)
mark_as_advanced(EIGEN_SOURCE_DIR)
