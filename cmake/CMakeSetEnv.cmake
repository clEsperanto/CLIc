## set environment variables
#
# Variables set here are directory-scoped: they apply to CLIc, its tests and its fetched dependencies, not to a
# parent project. Cache variables and global properties are only set when CLIc is the top-level project.

# add the /MP flag for MSVC compiler
if(MSVC)
  add_compile_options(/MP16)
endif()

if(APPLE)
  # vkFFT and Eigen come with several warnings, silencing them for now
  add_compile_options(-Wno-deprecated-declarations -Wno-comment -Wno-switch)
endif()

# set C++ standard and extensions (consumers get C++17 from the CLIc target: target_compile_features)
set(CMAKE_CXX_STANDARD 17)           # C++17 is required (no fallback)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)        # Don't use e.g. GNU extension (like -std=gnu++11) for portability
add_compile_definitions(CL_TARGET_OPENCL_VERSION=120)

# Windows: no Fortran compiler lookup (quick fix for windows CI). Not forced, so a parent project using Fortran keeps it.
if(NOT DEFINED CMAKE_Fortran_COMPILER)
  set(CMAKE_Fortran_COMPILER NOTFOUND CACHE FILEPATH "Fortran compiler")
endif()

## Top-level project only: build type, IDE folders and developer options
if(CLIC_IS_TOP_LEVEL)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # manage build type options (default: Release)
  get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
  if(isMultiConfig)
    set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MinSizeRel;RelWithDebInfo" CACHE STRING "" FORCE)
    message(STATUS "CMAKE_CONFIGURATION_TYPES: ${CMAKE_CONFIGURATION_TYPES}")
  else()
    if(NOT CMAKE_BUILD_TYPE)
      set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    endif()
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release" "Debug" "MinSizeRel" "RelWithDebInfo")
    message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
  endif()

  option(BUILD_TESTING "Build CLIc tests" ON)
  message(STATUS "Build tests: ${BUILD_TESTING}")

  option(BUILD_COVERAGE "Enable coverage reporting" OFF)
  message(STATUS "Build project code coverage: ${BUILD_COVERAGE}")
  if(BUILD_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
  endif()

  option(BUILD_BENCHMARK "build benchmarks tests" OFF)

  # Kernel disk cache: 'cmake --build <dir> --target clic_clear_cache' deletes it,
  # -DCLEAR_KERNEL_CACHE=ON deletes it at configure time
  if(WIN32)
    set(CLIC_KERNEL_CACHE_DIR "$ENV{LOCALAPPDATA}/clesperanto")
  else()
    set(CLIC_KERNEL_CACHE_DIR "$ENV{HOME}/.cache/clesperanto")
  endif()
  add_custom_target(clic_clear_cache
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${CLIC_KERNEL_CACHE_DIR}"
    COMMENT "Deleting kernel cache ${CLIC_KERNEL_CACHE_DIR}"
  )
  option(CLEAR_KERNEL_CACHE "remove kernel binaries cache from the system" OFF)
  if(CLEAR_KERNEL_CACHE)
    message(STATUS "Deleting kernel cache: ${CLIC_KERNEL_CACHE_DIR}")
    file(REMOVE_RECURSE "${CLIC_KERNEL_CACHE_DIR}")
  endif()
endif()

## Library type: shared by default. As a subproject, follow the parent's BUILD_SHARED_LIBS and,
## if it has none, default to shared without adding the option to the parent's cache.
if(CLIC_IS_TOP_LEVEL)
  option(BUILD_SHARED_LIBS "Build shared libraries" ON)
elseif(NOT DEFINED BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS ON)
endif()
if(WIN32 AND BUILD_SHARED_LIBS)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

## Define install options and presets
include(GNUInstallDirs)

# CMake target export name
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")

# RPATH of the installed shared library (the build tree RPATH is CMake's default)
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling
if(BUILD_SHARED_LIBS)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
  # add the directories of linked libraries outside the build tree
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

# CMake Registry
include(${PROJECT_SOURCE_DIR}/cmake/CMakeRegistery.cmake)
