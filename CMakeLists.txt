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
    add_subdirectory(${LIBRARY_DIR}/tests)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif()

# Build and run benchmark
if(BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()

if(BUILD_CODE_COVERAGE)
    set(GCOVR_ADDITIONAL_ARGS "--exclude-throw-branches")
    setup_target_for_coverage_lcov(
        NAME coverage
        EXECUTABLE ctest -C ${CMAKE_BUILD_TYPE} -T test -T coverage
        BASE_DIRECTORY ${PROJECT_SOURCE_DIR}
        EXCLUDE "/benchmark/*"
        )
    setup_target_for_coverage_gcovr_html(
        NAME coverage_html
        EXECUTABLE ctest -C ${CMAKE_BUILD_TYPE} -T test -T coverage
        BASE_DIRECTORY ${PROJECT_SOURCE_DIR}
        EXCLUDE "/benchmark/*"
        )
    setup_target_for_coverage_gcovr_xml(
        NAME coverage_xml
        EXECUTABLE ctest -C ${CMAKE_BUILD_TYPE} -T test -T coverage
        BASE_DIRECTORY ${PROJECT_SOURCE_DIR}
        EXCLUDE "/benchmark/*"
        )
endif()
