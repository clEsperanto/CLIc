# managed third party libraries
add_subdirectory(thirdparty)

# manage configuration files
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME_LOWERCASE}.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/${LIBRARY_NAME_LOWERCASE}.hpp"
    NO_SOURCE_PERMISSIONS @ONLY
)

# list project source files
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE TIERS "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp")
list(APPEND SOURCES ${TIERS})

file(GLOB_RECURSE HEADERS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE HEADERS_BIN "${CMAKE_CURRENT_BINARY_DIR}/include/*.hpp")
set(HEADERS ${HEADERS_SRC})
list(APPEND HEADERS ${HEADERS_BIN})

# target library definition
add_library(${LIBRARY_NAME} ${SOURCES} ${HEADERS})
add_library(${PROJECT_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})
target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${KERNELS_HEADER_DIR}>
    $<BUILD_INTERFACE:${EIGEN_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)        # force C++ linker
set_target_properties(${LIBRARY_NAME} PROPERTIES VERSION ${PROJECT_VERSION}) # set version
add_dependencies(${LIBRARY_NAME} generate_kernels)

# test if BACKEND variable is OpenCL
if(${OpenCL_FOUND})
    target_include_directories(${LIBRARY_NAME} PUBLIC ${OpenCL_INCLUDE_DIRS})
    target_link_libraries(${LIBRARY_NAME} PUBLIC ${OpenCL_LIBRARIES})
endif()
if(${CUDAToolkit_FOUND})
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
