# managed third party libraries
add_subdirectory(thirdparty)

# manage configuration files
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/clic.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/clic.hpp"
    NO_SOURCE_PERMISSIONS @ONLY
)

# list project source files
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE TIERS "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp")
list(APPEND SOURCES ${TIERS})

if(CLE_METAL)
    file(GLOB_RECURSE OBJCXX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.mm")
    list(APPEND SOURCES ${OBJCXX_SOURCES})
endif()

message(STATUS "FETCHCONTENT_BASE_DIR: ${FETCHCONTENT_BASE_DIR}")

# target library definition
add_library(CLIc ${SOURCES})
target_link_libraries(CLIc PUBLIC cleKernels::cleKernels Eigen3::Eigen clic_vkfft)
# consumers of the installed package need C++17 too (public headers use it)
target_compile_features(CLIc PUBLIC cxx_std_17)
# make target depend on other targets
add_dependencies(CLIc cleKernels::cleKernels Eigen3::Eigen clic_vkfft)

add_library(CLIc::CLIc ALIAS CLIc)
# target_include_directories(CLIc PUBLIC ${FETCHCONTENT_BASE_DIR}/vkfft-src/vkFFT/)
target_include_directories(CLIc PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # CLIc headers
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include> # CLIc configuration headers
    $<BUILD_INTERFACE:${CLEKERNELS_INCLUDE_DIR}>           # cleKernels headers
    $<BUILD_INTERFACE:${EIGEN3_INCLUDE_DIR}>               # Eigen headers
    $<BUILD_INTERFACE:${FETCHCONTENT_BASE_DIR}/vkfft-src/vkFFT/> # VkFFT headers
    $<BUILD_INTERFACE:${FETCHCONTENT_BASE_DIR}/dlpack-src/include> # DLPack headers
    $<INSTALL_INTERFACE:include>
)


# library version, used to namespace the kernel disk cache
target_compile_definitions(CLIc PRIVATE CLIC_VERSION="${PROJECT_VERSION}")

set_target_properties(CLIc PROPERTIES LINKER_LANGUAGE CXX)        # force C++ linker
set_target_properties(CLIc PROPERTIES VERSION ${PROJECT_VERSION}) # set version
set_target_properties(CLIc PROPERTIES DEBUG_POSTFIX "_d")             # libCLIc_d for debug builds
# When statically linked into a consumer shared library (e.g. a Python extension), hide CLIc symbols
# by default so they don't leak into the consumer's dynamic symbol table and cause interposition.
if(NOT BUILD_SHARED_LIBS)
    set_target_properties(CLIc PROPERTIES CXX_VISIBILITY_PRESET hidden)
    set_target_properties(CLIc PROPERTIES VISIBILITY_INLINES_HIDDEN YES)
endif()
# link backend-specific dependencies
if(CLE_OPENCL)
    target_link_libraries(CLIc PUBLIC OpenCL::OpenCL)
elseif(CLE_CUDA)
    set_target_properties(CLIc PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    set_target_properties(CLIc PROPERTIES POSITION_INDEPENDENT_CODE ON)
    target_include_directories(CLIc PUBLIC "$<BUILD_INTERFACE:${CUDAToolkit_INCLUDE_DIRS}>")
    target_link_libraries(CLIc PUBLIC CUDA::cudart CUDA::cuda_driver CUDA::nvrtc)
elseif(CLE_METAL)
    enable_language(OBJCXX)
    file(GLOB_RECURSE OBJCXX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.mm")
    set_source_files_properties(${OBJCXX_SOURCES} PROPERTIES
        COMPILE_FLAGS "-x objective-c++"
    )
    target_include_directories(CLIc BEFORE PUBLIC
        $<BUILD_INTERFACE:${vkfft_SOURCE_DIR}/metal-cpp>
    )
    target_link_libraries(CLIc PUBLIC
        "-framework Metal"
        "-framework Foundation"
        "-framework QuartzCore"
    )
endif()

# library install instructions
include(${PROJECT_SOURCE_DIR}/cmake/Install.cmake)
