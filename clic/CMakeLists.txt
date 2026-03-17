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

message(STATUS "FETCHCONTENT_BASE_DIR: ${FETCHCONTENT_BASE_DIR}")
message(STATUS "VKFFT_INCLUDE_DIR: ${VKFFT_INCLUDE_DIR}")

# target library definition
add_library(${LIBRARY_NAME} ${SOURCES})
target_link_libraries(${LIBRARY_NAME} PUBLIC cleKernels::cleKernels Eigen3::Eigen  VkFFT) # clFFT

# make target depend on other targets
add_dependencies(${LIBRARY_NAME} cleKernels::cleKernels Eigen3::Eigen  VkFFT) # clFFT

add_library(${PROJECT_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})
# target_include_directories(${LIBRARY_NAME} PUBLIC ${FETCHCONTENT_BASE_DIR}/vkfft-src/vkFFT/)
target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # CLIc headers
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include> # CLIc configuration headers
    $<BUILD_INTERFACE:${CLEKERNELS_INCLUDE_DIR}>           # cleKernels headers
    $<BUILD_INTERFACE:${EIGEN3_INCLUDE_DIR}>               # Eigen headers
    $<BUILD_INTERFACE:${FETCHCONTENT_BASE_DIR}/vkfft-src/vkFFT/> # VkFFT headers
    $<INSTALL_INTERFACE:include>
)

set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)        # force C++ linker
set_target_properties(${LIBRARY_NAME} PROPERTIES VERSION ${PROJECT_VERSION}) # set version
# When statically linked into a consumer shared library (e.g. a Python extension), hide CLIc symbols
# by default so they don't leak into the consumer's dynamic symbol table and cause interposition.
if(NOT BUILD_SHARED_LIBS)
    set_target_properties(${LIBRARY_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
    set_target_properties(${LIBRARY_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN YES)
endif()
# link backend-specific dependencies
if(CLE_OPENCL)
    target_include_directories(${LIBRARY_NAME} PUBLIC ${OpenCL_INCLUDE_DIRS})
    target_link_libraries(${LIBRARY_NAME} PUBLIC ${OpenCL_LIBRARIES})
elseif(CLE_CUDA)
    set_target_properties(${LIBRARY_NAME} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    set_target_properties(${LIBRARY_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
    target_include_directories(${LIBRARY_NAME} PUBLIC ${CUDAToolkit_INCLUDE_DIRS})
    target_link_libraries(${LIBRARY_NAME} PUBLIC CUDA::cudart CUDA::cuda_driver CUDA::nvrtc)
endif()

# library install instructions
include(${PROJECT_SOURCE_DIR}/cmake/Install.cmake)
