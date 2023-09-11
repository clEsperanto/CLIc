cmake_minimum_required(VERSION 3.20)

project(CLIc VERSION 0.7.2)

set(BUILD_CUDA_BACKEND ON CACHE BOOL "Build CUDA backend")
set(BUILD_OPENCL_BACKEND ON CACHE BOOL "Build OpenCL backend")

# set environment variables
include(${PROJECT_SOURCE_DIR}/cmake/CMakeSetEnv.cmake)
# find library dependencies
include(${PROJECT_SOURCE_DIR}/cmake/CMakeDependencies.cmake)
# set documentation
include(${PROJECT_SOURCE_DIR}/cmake/CMakeDoxygen.cmake)
# managed third party libraries
add_subdirectory(thirdparty)

# manage configuration files
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME_LOWERCASE}/${LIBRARY_NAME_LOWERCASE}.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME_LOWERCASE}/include/${LIBRARY_NAME_LOWERCASE}.hpp"
    NO_SOURCE_PERMISSIONS @ONLY
)

# list project source files
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME_LOWERCASE}/src/*.cpp")
file(GLOB_RECURSE HEADERS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME_LOWERCASE}/include/*.hpp")
file(GLOB_RECURSE HEADERS_BIN "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME_LOWERCASE}/include/*.hpp")
set(HEADERS ${HEADERS_SRC})
list(APPEND HEADERS ${HEADERS_BIN})

# target library definition
add_library(${LIBRARY_NAME} ${SOURCES} ${HEADERS})
add_library(${PROJECT_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})
target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME_LOWERCASE}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME_LOWERCASE}/include>
    $<BUILD_INTERFACE:${KERNELS_HEADER_DIR}>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)        # force C++ linker
set_target_properties(${LIBRARY_NAME} PROPERTIES VERSION ${PROJECT_VERSION}) # set version
add_dependencies(${LIBRARY_NAME} generate_kernels)
add_dependencies(${LIBRARY_NAME} documentation)

# link GPU framework to the target
if (OpenCL_FOUND)
    message (STATUS "OpenCL found")
    target_include_directories(${LIBRARY_NAME} PUBLIC ${OpenCL_INCLUDE_DIRS})
    target_link_libraries(${LIBRARY_NAME} PUBLIC ${OpenCL_LIBRARIES})
endif()
if (CUDAToolkit_FOUND)
    message (STATUS "CUDA found")
    set_target_properties(${LIBRARY_NAME} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    set_target_properties(${LIBRARY_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
    target_include_directories(${LIBRARY_NAME} PUBLIC ${CUDAToolkit_INCLUDE_DIRS})
    target_link_libraries(${LIBRARY_NAME} PUBLIC CUDA::cudart CUDA::cuda_driver CUDA::nvrtc CUDA::OpenCL) 
endif()

# # Set RPATH configuration
# if(APPLE)
#     set(CMAKE_BUILD_RPATH "@loader_path")
#     set(CMAKE_INSTALL_RPATH "@loader_path/../lib")
# else()
#     set(CMAKE_BUILD_RPATH "$ORIGIN")
#     set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
# endif()

# library install instructions
include(${PROJECT_SOURCE_DIR}/cmake/Install.cmake)

# Build and run tests
if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif(BUILD_TESTING)



