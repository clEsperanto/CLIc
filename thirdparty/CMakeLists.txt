
include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})

## Manage OpenCL Kernel Sources
FetchContent_Declare(OpenCL_Kernels
    GIT_REPOSITORY https://github.com/clEsperanto/clij-opencl-kernels.git
    GIT_TAG        clesperanto_kernels
    BUILD_ALWAYS OFF
    CONFIGURE_COMMAND ""
)

if(NOT OpenCL_Kernels_POPULATED)
    FetchContent_Populate(OpenCL_Kernels)
endif()

set(OCL_KERNELS_DIR ${CMAKE_CURRENT_BINARY_DIR}/opencl_kernels-src CACHE PATH "Path to opencl kernel files (.cl)" FORCE)
mark_as_advanced(OCL_KERNELS_DIR)


## Manage OpenCL headers source code
FetchContent_Declare(OpenCL_HEADERS
    GIT_REPOSITORY https://github.com/KhronosGroup/OpenCL-Headers.git
    GIT_TAG v2023.02.06
    CONFIGURE_COMMAND ""
    BUILD_ALWAYS OFF
    GIT_SUBMODULES "tests"
    ) 

FetchContent_Declare(OpenCL_CLHPP
    GIT_REPOSITORY https://github.com/KhronosGroup/OpenCL-CLHPP.git
    GIT_TAG v2023.02.06
    CONFIGURE_COMMAND ""
    BUILD_ALWAYS OFF
    GIT_SUBMODULES "docs"
    )

if(NOT OpenCL_HEADERS_POPULATED)
    FetchContent_Populate(OpenCL_HEADERS)
endif()

if(NOT OpenCL_CLHPP_POPULATED)
    FetchContent_Populate(OpenCL_CLHPP)
endif()

set(OpenCL_HEADERS_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/opencl_headers-src CACHE PATH "Path to opencl headers" FORCE)
set(OpenCL_CLHPP_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/opencl_clhpp-src CACHE PATH "Path to opencl clhpp" FORCE)
set(OpenCL_INCLUDE_DIR ${OpenCL_HEADERS_SRC_DIR} ${OpenCL_CLHPP_SRC_DIR}/include/ CACHE PATH "Path to opencl (header and clhpp)" FORCE)

## Set OpenCL version to 1.2
# set(OpenCL_VERSION "${OpenCL_VERSION_MAJOR}${OpenCL_VERSION_MINOR}0" CACHE STRING "OpenCL version" FORCE)
set(OpenCL_VERSION "120" CACHE STRING "OpenCL version" FORCE)
