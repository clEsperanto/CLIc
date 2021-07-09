# Set minimum version of CMake.
cmake_minimum_required(VERSION 3.20)

# Set project name and version
project(CLIc VERSION 0.3.0)

# Set environment variables
include(${PROJECT_SOURCE_DIR}/cmake/SetEnv.cmake)

# find required package
find_package(OpenCL REQUIRED)
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# Library sources
add_subdirectory(${LIBRARY_DIR})

# Build and run tests
if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(${LIBRARY_DIR}/test)
endif()

# Build and run benchmark
if(BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()

