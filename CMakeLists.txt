cmake_minimum_required(VERSION 3.20)

project(CLIc VERSION 0.10.3)

# if not set, set the default build type to Release
if(NOT CLE_BACKEND_TYPE)
    set(CLE_BACKEND_TYPE "OPENCL" CACHE STRING "Backend to use (CUDA or OCL)")
endif()


# set environment variables
include(${PROJECT_SOURCE_DIR}/cmake/CMakeSetEnv.cmake)
# find library dependencies
include(${PROJECT_SOURCE_DIR}/cmake/CMakeBackends.cmake)


# managed third party libraries
add_subdirectory(clic)

# Build and run tests
if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif(BUILD_TESTING)

# Build documentation
if (BUILD_DOCUMENTATION)
    add_subdirectory(${PROJECT_SOURCE_DIR}/docs)
endif()
