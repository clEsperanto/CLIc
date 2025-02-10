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

# target library definition
add_library(${LIBRARY_NAME} ${SOURCES})
target_link_libraries(${LIBRARY_NAME} PUBLIC cleKernels::cleKernels Eigen3::Eigen ${CLFFT_LIBRARIES})

# make target depend on other targets
add_dependencies(${LIBRARY_NAME} cleKernels::cleKernels Eigen3::Eigen clFFT)

add_library(${PROJECT_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})
target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${CLEKERNELS_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${EIGEN3_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${clFFT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${clFFT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)        # force C++ linker
set_target_properties(${LIBRARY_NAME} PROPERTIES VERSION ${PROJECT_VERSION}) # set version

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

# library install instructions
include(${PROJECT_SOURCE_DIR}/cmake/Install.cmake)
