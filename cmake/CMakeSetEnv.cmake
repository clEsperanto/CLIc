## set environment variables

# set library name
if(NOT LIBRARY_NAME)
  set(LIBRARY_NAME ${PROJECT_NAME})
endif()

# Define upper and lower case project name
string(TOUPPER ${LIBRARY_NAME} LIBRARY_NAME_UPPERCASE)
string(TOLOWER ${LIBRARY_NAME} LIBRARY_NAME_LOWERCASE)

# set folder properties for IDE
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# set C++ standard and extensions
set(CMAKE_CXX_STANDARD 17)           # C++17, global property instead of target property because of wrapper
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # C++17 is required (no fallback)
set(CMAKE_CXX_EXTENSIONS OFF)        # Don't use e.g. GNU extension (like -std=gnu++11) for portability

# manage build type options (default: Release)
get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(isMultiConfig)
  set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MinSizeRel;RelWithDebInfo" CACHE STRING "" FORCE)
  message(STATUS "CMAKE_CONFIGURATION_TYPES: ${CMAKE_CONFIGURATION_TYPES}")
  message(STATUS "CMAKE_GENERATOR: Multi-config")
else()
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  endif()
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release" "Debug" "MinSizeRel" "RelWithDebInfo")
  message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
  message(STATUS "CMAKE_GENERATOR: Single-config")
endif()
# add _d postfix for debug builds
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_RELEASE_POSTFIX "")

## Defines options for the build

# set build as static or shared library (default: static)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
if (WIN32 AND BUILD_SHARED_LIBS)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# set Code coverage options (default: OFF)
option(BUILD_COVERAGE "Enable coverage reporting" OFF)
message(STATUS "Build project code coverage: ${BUILD_COVERAGE}")
if (BUILD_COVERAGE)
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}  " --coverage -g -O0")
  set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} " --coverage -g -O0")
endif()

# set Test compilation (default: ON)
option(BUILD_TESTING "Build ${LIBRARY_NAME} Tests." ON)
message(STATUS "Build tests: ${BUILD_TESTING}")

# set Doc compilation (default: ON) (NOT IMPLEMENTED)
option(BUILD_DOCUMENTATION "Build ${LIBRARY_NAME} Documentation." ON)
message(STATUS "Build documentation: ${BUILD_DOCUMENTATION} (WIP)")

# set Benchmark compilation (default: ON)
option(BUILD_BENCHMARK "build benchmarks tests" OFF)
message(STATUS "Build benchmark: ${BUILD_BENCHMARK}")

## Define install options and presets

include(GNUInstallDirs)

# CMake target export name
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")

# Always full RPATH (for shared libraries)
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling
if(BUILD_SHARED_LIBS)
  # use, i.e. don't skip the full RPATH for the build tree
  set(CMAKE_SKIP_BUILD_RPATH FALSE)
  # when building, don't use the install RPATH already (but later on when installing)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  # add the automatically determined parts of the RPATH
  # which point to directories outside the build tree to the install RPATH
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  # the RPATH to be used when installing, but only if it's not a system directory
  list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
  if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  endif()
endif()

# CMake Registry
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeRegistery.cmake)
