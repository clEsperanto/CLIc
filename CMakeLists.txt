cmake_minimum_required(VERSION 3.20)

project(CLIc VERSION 0.25.0 LANGUAGES C CXX)

# CLIc is built on its own (development, CI) or as a subproject of another project (pyclesperanto, clesperantoj, ...).
# Settings that would change the parent project are only applied when CLIc is the top-level project.
# (same as PROJECT_IS_TOP_LEVEL, which needs CMake 3.21)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(CLIC_IS_TOP_LEVEL ON)
else()
    set(CLIC_IS_TOP_LEVEL OFF)
endif()

# Dependency versions. Normal variables, so that a new default here is picked up by existing build trees;
# override them with -D<name>=<tag>.
if(NOT DEFINED CLIC_KERNELS_TAG)
    set(CLIC_KERNELS_TAG "3.7.0")   # clEsperanto kernels (clij-opencl-kernels)
endif()
if(NOT DEFINED CLIC_EIGEN_TAG)
    set(CLIC_EIGEN_TAG "5.0.1")     # Eigen (downloaded only when no suitable Eigen is found, see clic/thirdparty)
endif()
if(NOT DEFINED CLIC_VKFFT_TAG)
    set(CLIC_VKFFT_TAG "v1.3.4")    # VkFFT
endif()

# GPU backend, chosen at configure time
if(NOT CLE_BACKEND)
    set(CLE_BACKEND "OPENCL" CACHE STRING "Backend to use (OPENCL, CUDA or METAL)")
endif()
if(DEFINED CACHE{CLE_BACKEND})
    set_property(CACHE CLE_BACKEND PROPERTY STRINGS OPENCL CUDA METAL)
endif()

# set environment variables and options
include(${PROJECT_SOURCE_DIR}/cmake/CMakeSetEnv.cmake)
# find library dependencies
include(${PROJECT_SOURCE_DIR}/cmake/CMakeBackends.cmake)

# build clic library
add_subdirectory(clic)

# Tests are only built when CLIc is the top-level project (-DBUILD_TESTING=OFF to skip them)
if(CLIC_IS_TOP_LEVEL AND BUILD_TESTING)
    include(CTest)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif()
