# Branch or Release tag to retrive the ocl/cu files
set(CLESPERANTO_KERNEL_TAG e6bc757500d848e3136aa8b9de8770207f5a3e58)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})

## Manage OpenCL Kernel Sources
FetchContent_Declare(kernels
    GIT_REPOSITORY https://github.com/clEsperanto/clij-opencl-kernels.git
    GIT_TAG        ${CLESPERANTO_KERNEL_TAG}
    BUILD_ALWAYS   OFF
)
if(NOT kernels_POPULATED)
    FetchContent_Populate(kernels)
endif()

set(KERNELS_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/kernels-src CACHE PATH "Path to opencl kernel files (.cl)" FORCE)
mark_as_advanced(OCL_KERNELS_DIR)
set(KERNELS_HEADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/kernels CACHE PATH "Path to kernel files (.h)" FORCE)
mark_as_advanced(OCL_KERNELS_DIR)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_custom_target(
    generate_kernels 
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/kernel_to_header.py ${KERNELS_SOURCE_DIR} ${KERNELS_HEADER_DIR}
    BYPRODUCTS ${KERNELS_HEADER_DIR}
    COMMENT "Generating headers from clesperanto kernels files."
)


FetchContent_Declare(eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG        3.4.0
    BUILD_ALWAYS   OFF
)
if(NOT Eigen_POPULATED)
    FetchContent_Populate(eigen)
endif()
set(EIGEN_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/eigen-src CACHE PATH "Path to eigen lib" FORCE)
mark_as_advanced(EIGEN_SOURCE_DIR)
