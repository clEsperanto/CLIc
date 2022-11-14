# ## Project environement variables

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# C++ compilation settings
# set(CMAKE_CXX_STANDARD 14) # Use C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Require (at least) it
set(CMAKE_CXX_EXTENSIONS OFF) # Don't use e.g. GNU extension (like -std=gnu++11) for portability

# Define upper and lower case project name
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPERCASE)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWERCASE)

# Define library name (by default is the project name)
if(NOT LIBRARY_NAME)
  set(LIBRARY_NAME ${PROJECT_NAME})
endif()

# Define library folder name (by default is the project name in lowercase)
if(NOT LIBRARY_DIR)
  set(LIBRARY_DIR ${PROJECT_NAME_LOWERCASE})
endif()

# Define additional directory needed by the project
if(NOT THIRDPARTY_DIR)
  set(THIRDPARTY_DIR "${PROJECT_SOURCE_DIR}/thirdparty" CACHE PATH "Third party libraries")
endif()

if(NOT UTILITIES_DIR)
  set(UTILITIES_DIR "${PROJECT_SOURCE_DIR}/utilities" CACHE PATH "Utilities folder")
endif()

# # Configuration and Build options
# Set Code coverage options (default: OFF)
option(BUILD_COVERAGE "Enable coverage reporting" OFF)
message(STATUS "Build project code coverage: ${BUILD_COVERAGE}")
mark_as_advanced(BUILD_COVERAGE)

# Set library type optiONs (default: STATIC)
option(BUILD_SHARED_LIBS "Build ${LIBRARY_NAME} as a shared library." OFF)
message(STATUS "Build ${LIBRARY_NAME} as shared lib: ${BUILD_SHARED_LIBS}")

# Set Test compilation (default: ON)
option(BUILD_TESTING "Build ${LIBRARY_NAME} Tests." ON)
message(STATUS "Build tests: ${BUILD_TESTING}")

# Set Doc compilation (default: ON) (NOT IMPLEMENTED)
option(BUILD_DOCUMENTATION "Build ${LIBRARY_NAME} Documentation." ON)
message(STATUS "Build documentation: ${BUILD_DOCUMENTATION} (WIP)")

# Set Benchmark compilation (default: ON)
option(BUILD_BENCHMARK "build benchmarks tests" ON)
message(STATUS "Build benchmark: ${BUILD_BENCHMARK}")

# Manage build type options (default: RELEASE)
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

message(STATUS "CMAKE_GENERATOR: ${CMAKE_GENERATOR}")

# Configurations tag to avoid compilation colliding
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_RELEASE_POSTFIX "")

# Coverage flags and includes
if(BUILD_COVERAGE)
  set(COVERAGE_TAG " -O0 -g --coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ${COVERAGE_TAG}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_TAG}")
endif()

# ## Install preset:
# - CMAKE_INSTALL_LIBDIR
# - CMAKE_INSTALL_BINDIR
# - CMAKE_INSTALL_INCLUDEDIR
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

# ## CMake Registry
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeRegistry.cmake)
