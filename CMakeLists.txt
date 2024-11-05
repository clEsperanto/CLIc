cmake_minimum_required(VERSION 3.20)

project(CLIc)

set(kernel_version_tag "3.0.2" CACHE STRING "clEsperanto kernel version tag")
set(eigen_lib_version_tag "3.4.0" CACHE STRING "Eigen library version tag")

# if not set, set the default build type to Release
if(NOT CLE_BACKEND_TYPE)
    set(CLE_BACKEND_TYPE "OPENCL" CACHE STRING "Backend to use (CUDA or OCL)")
endif()

# set environment variables
include(${PROJECT_SOURCE_DIR}/cmake/CMakeSetEnv.cmake)
# find library dependencies
include(${PROJECT_SOURCE_DIR}/cmake/CMakeBackends.cmake)

# build clic library
add_subdirectory(clic)

# Build and run tests if BUILD_TESTING is ON (-DBUILD_TESTING=ON)
if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif(BUILD_TESTING)

# Build documentation if BUILD_DOCUMENTATION is ON (-DBUILD_DOCUMENTATION=ON)
if (BUILD_DOCUMENTATION)
    add_subdirectory(${PROJECT_SOURCE_DIR}/docs)
endif()
