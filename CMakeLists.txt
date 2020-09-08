## Project Initialisation ####################################

cmake_minimum_required(VERSION 3.0)

# Project name
project(CLIc)

# define compilation flag
set(CMAKE_C_STANDARD 11)

## Libraries management ####################################

# OpenCL Dependency
find_package(OpenCL)
if(OpenCL_FOUND)
  include_directories(${OpenCL_INCLUDE_DIR})
endif()

# TIFF Dependency
find_package(TIFF)
if(TIFF_FOUND)
  include_directories(${TIFF_INCLUDE_DIR})
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

