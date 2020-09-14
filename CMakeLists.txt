## Project Initialisation ####################################

cmake_minimum_required(VERSION 3.0)

# Project name
project(Clic VERSION 0.1 DESCRIPTION "OpenCL Image C++ Library")

# define cmake compilation flag
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(CTest)

## Project architecture ####################################

# defines architecture paths
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(COMPILED_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(COMPILED_LIBRARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib)

## Project required libraries ####################################

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

## Project options and configuration ####################################

option(BUILD_SHARED_LIBS "Make CLIc dynamic" ON)
option(BUILD_TESTING "Build and enable tests" ON)

# Declare CMake variable parameters
set(CLE_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/clij-opencl-kernels/src/main/java/net/haesleinhuepf/clij/kernels CACHE PATH "Path to CLI kernels folder")
set(CLE_PREAMBLE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE PATH "Path to CLI preamble folder")
configure_file("cleConfig.h.in" ${INCLUDE_DIR}/cleConfig.h)

## Project source and headers ####################################

# List sources
set(SOURCE
${SRC_DIR}/cleGPU.cpp
${SRC_DIR}/cleKernel.cpp
${SRC_DIR}/cleAddImageAndScalar.cpp
${SRC_DIR}/cleSmallerOrEqualConstant.cpp
${SRC_DIR}/cleMaximumZProjection.cpp
${SRC_DIR}/cleBuffer.cpp
${SRC_DIR}/tiffreader.cpp
${SRC_DIR}/tiffwriter.cpp
${SRC_DIR}/utils.cpp
)

# List header
set(HEADER 
${INCLUDE_DIR}/cleGPU.h
${INCLUDE_DIR}/cleKernel.h
${INCLUDE_DIR}/cleAddImageAndScalar.h
${INCLUDE_DIR}/cleSmallerOrEqualConstant.h
${INCLUDE_DIR}/cleMaximumZProjection.h
${INCLUDE_DIR}/cleBuffer.h
${INCLUDE_DIR}/cleConfig.h
${INCLUDE_DIR}/tiffreader.h
${INCLUDE_DIR}/tiffwriter.h
${INCLUDE_DIR}/utils.h
)

## Define target and links ####################################

# Create target as executable
add_library(${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME} OpenCL::OpenCL TIFF::TIFF)

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR} PRIVATE ${SRC_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${COMPILED_LIBRARY_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 0)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "cleConfig.h")

## Install ####################################

# include(GNUInstallDirs)
# 
# install(TARGETS ${PROJECT_NAME} EXPORT cleConfig.h
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
# install(DIRECTORY ${INCLUDE_DIR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# 
# install(EXPORT MyLibConfig DESTINATION share/MyLib/cmake)
# 
# export(TARGETS ${PROJECT_NAME} FILE Clic.cmake)

## Tests ####################################
if(BUILD_TESTING)
  add_subdirectory(${TESTS_DIR})
endif()
