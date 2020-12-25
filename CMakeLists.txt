# Set minimum version of CMake.
cmake_minimum_required(VERSION 3.9)

# Set project name and version
project(CLIc VERSION 0.1.0)

# Set environment variables
include(${PROJECT_SOURCE_DIR}/cmake/SetEnv.cmake)

# Library sources
add_subdirectory(${LIBRARY_FOLDER})

# build and run tests
if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(${LIBRARY_FOLDER}/test)
endif()