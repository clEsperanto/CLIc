## CMake Initialisation

cmake_minimum_required(VERSION 3.0)

project(CLIc)

## Find and manage libraries and include path
#
find_package(OpenCL REQUIRED)
if(OpenCL_FOUND)
  include_directories(${OpenCL_INCLUDE_DIR})
endif()

find_package(TIFF REQUIRED)
if(TIFF_FOUND)
 include_directories(${TIFF_INCLUDE_DIR})
endif()

## List project sources, header, and include directories
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(source 
src/main.cpp
src/clbuffer.cpp
src/tiffreader.cpp
src/tiffwriter.cpp
)

set(header 
include/clbuffer.h
include/tiffreader.h
include/tiffwriter.h
include/utils.h
)

## Define target and link
#
add_executable(CLIc ${source} ${header})

target_link_libraries(CLIc OpenCL::OpenCL TIFF::TIFF)
set_target_properties(CLIc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

## Define Path to kernels directory
#
set(CLI_Path ${CMAKE_CURRENT_SOURCE_DIR}/clij-opencl-kernels/src/main/java/net/haesleinhuepf/clij/kernels CACHE PATH "Path to CLI kernels folder")
set(CLP_Path ${CMAKE_CURRENT_SOURCE_DIR}/ressources CACHE PATH "Path to CLI preamble folder")

target_compile_definitions(CLIc PRIVATE -DCLI_PATH=\"${CLI_Path}\")
target_compile_definitions(CLIc PRIVATE -DCLP_PATH=\"${CLP_Path}\")
target_compile_definitions(CLIc PRIVATE -DOCL_MAJOR_VERSION=${OpenCL_VERSION_MAJOR})

