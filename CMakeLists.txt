# Set minimum version of CMake.
cmake_minimum_required(VERSION 3.20)

# Set project name and version
project(CLIc VERSION 0.6.0)

# Set environment variables
include(${PROJECT_SOURCE_DIR}/cmake/SetEnv.cmake)

# find required package
if(NOT OpenCL_FOUND AND NOT DEFINED OpenCL_LIBRARY)
    find_package(OpenCL REQUIRED)
    # find_package(OpenCLHeaders REQUIRED)
    # find_package(OpenCLICDLoader REQUIRED)
endif()
message(STATUS "OpenCL_LIBRARY : ${OpenCL_LIBRARY}")

if(NOT Python3_Interpreter_FOUND)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
endif()

# Library sources
add_subdirectory(${THIRDPARTY_DIR})
add_subdirectory(${LIBRARY_DIR})

# Build and run tests
if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif(BUILD_TESTING)

# Build and run benchmark
if(BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif(BUILD_BENCHMARK)

if(BUILD_DOCUMENTATION)
    # find_package(Doxygen)
    # if(DOXYGEN_FOUND)
    # # set input and output files
    # set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    # set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    # # request to configure the file
    # configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    # message("Doxygen build started")
    # # note the option ALL which allows to build the docs together with the application
    # add_custom_target( doc_doxygen ALL
    # COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    # WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    # COMMENT "Generating API documentation with Doxygen"
    # VERBATIM )
    # else(DOXYGEN_FOUND)
    # message("Doxygen need to be installed to generate the documentation")
    # endif(DOXYGEN_FOUND)
endif(BUILD_DOCUMENTATION)
