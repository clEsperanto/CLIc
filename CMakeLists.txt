## Project Initialisation ####################################

cmake_minimum_required(VERSION 3.0)

# Project name
project(CLIc)

# define compilation flag
set(CMAKE_C_STANDARD 11)

# defines architecture paths
set(src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(tests_dir ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(bin_dir ${CMAKE_CURRENT_SOURCE_DIR}/bin)

## Libraries management ####################################

# OpenCL Dependency
find_package(OpenCL REQUIRED)
if(OpenCL_FOUND)
  include_directories(${OpenCL_INCLUDE_DIR})
endif()

# TIFF Dependency
find_package(TIFF REQUIRED)
if(TIFF_FOUND)
  include_directories(${TIFF_INCLUDE_DIR})
endif()

## Project files ####################################

# List sources
set(source
${src_dir}/clgpu.cpp
${src_dir}/clbuffer.cpp
${src_dir}/tiffreader.cpp
${src_dir}/tiffwriter.cpp
${src_dir}/utils.cpp
)

# List header
set(header 
${include_dir}/clbuffer.h
${include_dir}/tiffreader.h
${include_dir}/tiffwriter.h
${include_dir}/utils.h
)

# main
set(main
${src_dir}/main.cpp
)

# Include directories
include_directories(${include_dir})

## Define target and links ####################################

# Create target as executable
add_executable(CLIc ${source} ${header} ${main})

# Link libraries to target
target_link_libraries(CLIc OpenCL::OpenCL TIFF::TIFF)

# Define executable path destination
set_target_properties(CLIc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_dir})

## Define global information ####################################

# Declare CMake variable parameters
set(CLI_Path ${CMAKE_CURRENT_SOURCE_DIR}/clij-opencl-kernels/src/main/java/net/haesleinhuepf/clij/kernels CACHE PATH "Path to CLI kernels folder")
set(CLP_Path ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE PATH "Path to CLI preamble folder")

# Define global variable
target_compile_definitions(CLIc PRIVATE -DCLI_PATH=\"${CLI_Path}\")
target_compile_definitions(CLIc PRIVATE -DCLP_PATH=\"${CLP_Path}\")
target_compile_definitions(CLIc PRIVATE -DOCL_MAJOR_VERSION=${OpenCL_VERSION_MAJOR})

## Tests ####################################
include(CTest) # set BUILD_TESTING ON by default

if(BUILD_TESTING)
  add_subdirectory(${tests_dir})
endif()
