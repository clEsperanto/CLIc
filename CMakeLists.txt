# cmake minimum version
cmake_minimum_required(VERSION 3.0)

# project definition
project(CLIc)
set(CLIc_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(CLIc_VERSION_MINOR 1 CACHE STRING "minor version" FORCE)
set(CLIc_VERSION_PATCH 0 CACHE STRING "patch version" FORCE)
set(CLIc_VERSION ${CLIc_VERSION_MAJOR}.${CLIc_VERSION_MINOR}.${CLIc_VERSION_PATCH} CACHE STRING "version" FORCE)

# project build options
option(BUILD_SHARED_LIBS "build CLIc as shared library" ON)
option(BUILD_TESTING "build and enable tests" ON)

# define cmake options
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/installed)
if(WIN32)
  option(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
set(DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Find required libraries 
# OpenCL
find_package(OpenCL REQUIRED)
if(OpenCL_FOUND)
  include_directories(${OpenCL_INCLUDE_DIR})
  add_compile_definitions(OpenCL_VERSION=${OpenCL_VERSION_MAJOR})
endif()
# LibTiff
find_package(TIFF REQUIRED)
if(TIFF_FOUND)
  include_directories(${TIFF_INCLUDE_DIR})
endif()

# Define required variables for CLIc execution
set(CLIc_KERNELS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/clij-opencl-kernels/src/main/java/net/haesleinhuepf/clij/kernels CACHE PATH "path to CLI kernels folder")
file(GLOB KERNELS ${CLIc_KERNELS_PATH}/*.cl)
add_compile_definitions(KERNELS_PATH="${CLIc_KERNELS_PATH}")
set(CLIc_PREAMBLE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/include/preamble.cl CACHE FILEPATH "path to CLI preamble file")
add_compile_definitions(PREAMBLE_FILE="${CLIc_PREAMBLE_FILE}")

# project install structure
set(INSTALL_DIR CLIc-${CLIc_VERSION})
set(INCLUDE_INSTALL_DIR ${INSTALL_DIR}/include/ )
set(KERNELS_INSTALL_DIR ${INSTALL_DIR}/kernels/ )
set(LIBRARY_INSTALL_DIR ${INSTALL_DIR}/lib/ )

# subdiretories 
add_subdirectory(include)
add_subdirectory(src)

# define library target
add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} OpenCL::OpenCL TIFF::TIFF)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CLIc_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> 
  $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}> 
  $<INSTALL_INTERFACE:${LIBRARY_INSTALL_DIR}> 
  )
    
# build and run tests
if(BUILD_TESTING)
  include(CTest)
  add_subdirectory(test)
endif()

# define install rule with config.cmake and targets.cmake files
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
  ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}
  )
  
install(FILES ${HEADERS} ${CLIc_PREAMBLE_FILE} 
  DESTINATION ${INCLUDE_INSTALL_DIR}
  )
install(FILES ${KERNELS} 
  DESTINATION ${KERNELS_INSTALL_DIR}
  )

include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CLIcConfig.cmake.in 
  ${CMAKE_CURRENT_BINARY_DIR}/CLIcConfig.cmake
  INSTALL_DESTINATION ${INSTALL_DIR}/cmake
  PATH_VARS INSTALL_DIR INCLUDE_INSTALL_DIR KERNELS_INSTALL_DIR LIBRARY_INSTALL_DIR
    )
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/CLIcConfigVersion.cmake
  VERSION ${CLIc_VERSION}
  COMPATIBILITY SameMajorVersion 
  )
install(FILES 
  ${CMAKE_CURRENT_BINARY_DIR}/CLIcConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/CLIcConfigVersion.cmake
  DESTINATION ${INSTALL_DIR}/cmake
  )
install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${INSTALL_DIR}/cmake
  )






