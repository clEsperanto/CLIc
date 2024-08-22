include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})

## Fetch OpenCL kernel sources
FetchContent_Declare(kernels
    GIT_REPOSITORY https://github.com/clEsperanto/clij-opencl-kernels.git
    GIT_TAG        ${kernel_version_tag}
    BUILD_ALWAYS   OFF
)
FetchContent_Declare(eigen
GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
GIT_TAG        3.4.0
BUILD_ALWAYS   OFF
)

FetchContent_GetProperties(kernels)
FetchContent_GetProperties(eigen)
if(NOT kernels_POPULATED)
    FetchContent_Populate(kernels)
endif()
if(NOT Eigen_POPULATED)
    FetchContent_Populate(eigen)
endif()

## Fetch Eigen library sources
FetchContent_Declare(eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG        ${eigen_lib_version_tag}
    BUILD_ALWAYS   OFF
)
FetchContent_GetProperties(eigen)
if(NOT Eigen_POPULATED)
    FetchContent_Populate(eigen)
endif()

## Define the path to the thirdparty libraries and include directories
set(KERNELS_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/kernels-src CACHE PATH "Path to opencl kernel files (.cl)" FORCE)
mark_as_advanced(KERNELS_SOURCE_DIR)
set(KERNELS_HEADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/kernels CACHE PATH "Path to kernel files (.h)" FORCE)
mark_as_advanced(KERNELS_HEADER_DIR)
set(EIGEN_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/eigen-src CACHE PATH "Path to eigen lib" FORCE)
mark_as_advanced(EIGEN_SOURCE_DIR)

## Define target to generate kernel headers from kernel files (.cl -> .h)
find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_custom_command(
    OUTPUT ${KERNELS_HEADER_DIR}
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/kernel_to_header.py ${KERNELS_SOURCE_DIR} ${KERNELS_HEADER_DIR}
    COMMENT "Generating headers from clesperanto kernels files."
    DEPENDS ${KERNELS_SOURCE_DIR}
)
add_custom_target(
    generate_kernels
    DEPENDS ${KERNELS_HEADER_DIR}
)
