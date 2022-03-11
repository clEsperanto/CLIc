# Set minimum version of CMake.
cmake_minimum_required(VERSION 3.20)

# Set project name and version
project(CLIc VERSION 0.5.2)

# Set environment variables
include(${PROJECT_SOURCE_DIR}/cmake/SetEnv.cmake)

# find required package
find_package(OpenCL REQUIRED)
find_package(Python COMPONENTS Interpreter Development REQUIRED)

# Library sources
add_subdirectory(${THIRDPARTY_DIR})
add_subdirectory(${LIBRARY_DIR})

# Build and run tests
if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(${LIBRARY_DIR}/tests)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif(BUILD_TESTING)

# Build and run benchmark
if(BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif(BUILD_BENCHMARK)


