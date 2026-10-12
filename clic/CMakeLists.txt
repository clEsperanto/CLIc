# managed third party libraries
add_subdirectory(thirdparty)

# manage configuration files
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/clic.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/clic.hpp"
    NO_SOURCE_PERMISSIONS @ONLY
)

# list project source files (src/ and src/tierN/); CONFIGURE_DEPENDS re-runs the glob at build time,
# so a new source file is picked up without re-running the configure step by hand
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
if(CLE_METAL)
    # Metal backend sources are Objective-C++ (.mm)
    enable_language(OBJCXX)
    file(GLOB_RECURSE OBJCXX_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.mm")
    list(APPEND SOURCES ${OBJCXX_SOURCES})
endif()

# target library definition
add_library(CLIc ${SOURCES})
target_link_libraries(CLIc PUBLIC cleKernels::cleKernels Eigen3::Eigen clic_vkfft)
# consumers of the installed package need C++17 too (public headers use it)
target_compile_features(CLIc PUBLIC cxx_std_17)

add_library(CLIc::CLIc ALIAS CLIc)
# cleKernels and Eigen headers come with their targets; VkFFT and DLPack are download-only (clic/thirdparty)
target_include_directories(CLIc PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # CLIc headers
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include> # CLIc configuration headers
    $<BUILD_INTERFACE:${CLIC_VKFFT_SOURCE_DIR}/vkFFT>      # VkFFT headers
    $<BUILD_INTERFACE:${CLIC_DLPACK_INCLUDE_DIR}>          # DLPack headers
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
    set_target_properties(CLIc PROPERTIES POSITION_INDEPENDENT_CODE ON)
    target_include_directories(CLIc PUBLIC "$<BUILD_INTERFACE:${CUDAToolkit_INCLUDE_DIRS}>")
    target_link_libraries(CLIc PUBLIC CUDA::cudart CUDA::cuda_driver CUDA::nvrtc)
elseif(CLE_METAL)
    target_include_directories(CLIc BEFORE PUBLIC
        $<BUILD_INTERFACE:${CLIC_VKFFT_SOURCE_DIR}/metal-cpp>
    )
    target_link_libraries(CLIc PUBLIC
        "-framework Metal"
        "-framework Foundation"
        "-framework QuartzCore"
    )
endif()

# library install instructions
include(${PROJECT_SOURCE_DIR}/cmake/Install.cmake)
