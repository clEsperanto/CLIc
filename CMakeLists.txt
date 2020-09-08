## Project Initialisation ####################################

cmake_minimum_required(VERSION 3.0)

# Project name
project(CLIc)

# define compilation flag
set(CMAKE_C_STANDARD 11)

## Libraries management ####################################


# OpenCL Dependency
find_package(OpenCL REQUIRED)
if(OpenCL_FOUND)
  include_directories(${OpenCL_INCLUDE_DIR})
else()
  if(APPLE)  
    set(local_opencl_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/apple/opencl)
    set(OpenCL_LIBRARY ${local_opencl_PATH}/libOpenCL.so) 
  elseif(WIN32)
    set(local_opencl_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/win32/opencl)
    set(OpenCL_LIBRARY ${local_opencl_PATH}/libOpenCL.lib)
  elseif(UNIX)
    set(local_opencl_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/unix/opencl)
    set(OpenCL_LIBRARY ${local_opencl_PATH}/libOpenCL.so)
  endif()
  set(OpenCL_INCLUDE_DIR ${local_opencl_PATH}/include)
  include_directories(${OpenCL_INCLUDE_DIR}) 
  find_package(OpenCL REQUIRED)
endif()

# TIFF Dependency
find_package(TIFF REQUIRED)
if(TIFF_FOUND)
  include_directories(${TIFF_INCLUDE_DIR})
else()
  if(APPLE)
    set(local_opencl_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/apple/libtiff)
    set(OpenCL_LIBRARY ${local_libtiff_PATH}/libtiff.so) 
  elseif(WIN32)
    set(local_opencl_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/win32/libtiff)
    set(OpenCL_LIBRARY ${local_libtiff_PATH}/libtiff.lib) 
  elseif(UNIX)
    set(local_opencl_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/unix/libtiff)
    set(OpenCL_LIBRARY ${local_libtiff_PATH}/libtiff.so) 
  endif()
  set(OpenCL_INCLUDE_DIR ${local_libtiff_PATH}/include)  
  include_directories(${TIFF_INCLUDE_DIR})
  find_package(TIFF REQUIRED)
endif()

## Project files ####################################

# defines paths
set(src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)

# List sources
set(source 
${src_dir}/main.cpp
${src_dir}/clbuffer.cpp
${src_dir}/tiffreader.cpp
${src_dir}/tiffwriter.cpp
)

# List header
set(header 
${include_dir}/clbuffer.h
${include_dir}/tiffreader.h
${include_dir}/tiffwriter.h
${include_dir}/utils.h
)

# Include directories
include_directories(${include_dir})

## Define target and links ####################################

# Create target as executable
add_executable(CLIc ${source} ${header})

# Link libraries to target
target_link_libraries(CLIc OpenCL::OpenCL TIFF::TIFF)

# Define executable path destination
set_target_properties(CLIc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

## Define global information ####################################

# Declare CMake variable parameters
set(CLI_Path ${CMAKE_CURRENT_SOURCE_DIR}/clij-opencl-kernels/src/main/java/net/haesleinhuepf/clij/kernels CACHE PATH "Path to CLI kernels folder")
set(CLP_Path ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE PATH "Path to CLI preamble folder")

# Define global variable
target_compile_definitions(CLIc PRIVATE -DCLI_PATH=\"${CLI_Path}\")
target_compile_definitions(CLIc PRIVATE -DCLP_PATH=\"${CLP_Path}\")
target_compile_definitions(CLIc PRIVATE -DOCL_MAJOR_VERSION=${OpenCL_VERSION_MAJOR})

