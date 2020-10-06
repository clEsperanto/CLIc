# cmake minimum version
cmake_minimum_required(VERSION 3.0)

# project definition
project(Clic)
set(Clic_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(Clic_VERSION_MINOR 1 CACHE STRING "minor version" FORCE)
set(Clic_VERSION_STRING ${Clic_VERSION_MAJOR}.${Clic_VERSION_MINOR} CACHE STRING "version" FORCE)

# project options
option(Clic_DEBUG_MODE "activate debug mode (WIP)" OFF)
option(BUILD_SHARED_LIBS "build CLIc as shared library" ON)
option(BUILD_TESTING "build and enable tests" ON)
option(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CLE_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/clij-opencl-kernels/src/main/java/net/haesleinhuepf/clij/kernels CACHE PATH "path to CLI kernels folder")
set(CLE_PREAMBLE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/include/preamble.cl CACHE FILEPATH "path to CLI preamble file")

# project destination path
set(include_dest include/Clic-${Clic_VERSION_STRING})
set(main_lib_dest lib/Clic-${Clic_VERSION_STRING})
set(lib_dest ${main_lib_dest}/${CMAKE_BUILD_TYPE})

# cmake options
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/installed)

# Managed required libraries 
find_package(OpenCL REQUIRED)
if(OpenCL_FOUND)
  include_directories(${OpenCL_INCLUDE_DIR})
endif()
find_package(TIFF REQUIRED)
if(TIFF_FOUND)
  include_directories(${TIFF_INCLUDE_DIR})
endif()

# subdiretories 
add_subdirectory(src)
if(BUILD_TESTING)
  include(CTest)
  add_subdirectory(test)
endif()

# installation
install(FILES cmake/Clic-config.cmake DESTINATION ${main_lib_dest})
install(EXPORT ${PROJECT_NAME} DESTINATION ${lib_dest})
