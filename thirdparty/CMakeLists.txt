# define path dir to kernel and preamble from clesperanto kernel git repo.
set(OCL_KERNELS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/clesperanto-kernels CACHE PATH "Path to opencl kernel files (.cl)" FORCE)
mark_as_advanced(OCL_KERNELS_DIR)

# set(OpenCL_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencl/ocl-headers/" CACHE PATH "Path to opencl headers" FORCE)
# set(OpenCL_CLHPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencl/ocl-clhpp/include/" CACHE PATH "Path to opencl clhpp" FORCE)
# set(OpenCL_INCLUDE_DIR ${OpenCL_HEADERS_DIR} ${OpenCL_CLHPP_DIR} CACHE PATH "Path to opencl (header and clhpp)" FORCE)

include(ExternalProject)

ExternalProject_Add(OpenCL_HEADERS
    GIT_REPOSITORY https://github.com/KhronosGroup/OpenCL-Headers.git
    GIT_TAG v2022.09.30
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/OpenCL_headers
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    CONFIGURE_COMMAND ""
)

ExternalProject_Add(OpenCL_CLHPP
    GIT_REPOSITORY https://github.com/KhronosGroup/OpenCL-CLHPP.git
    GIT_TAG v2022.09.30
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/OpenCL_clhpp
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    CONFIGURE_COMMAND ""
)

# ExternalProject_Get_Property(OpenCL_HEADERS SOURCE_DIR)
# message(STATUS "git opencl header dir: ${SOURCE_DIR}")
# set(OpenCL_HEADERS_DIR "${CMAKE_CURRENT_BINARY_DIR}/OpenCL_headers/" CACHE PATH "Path to opencl headers" FORCE)

# ExternalProject_Get_Property(OpenCL_CLHPP SOURCE_DIR)
# message(STATUS "git opencl header dir: ${SOURCE_DIR}")
# set(OpenCL_CLHPP_DIR "${CMAKE_CURRENT_BINARY_DIR}/OpenCL_clhpp/include/" CACHE PATH "Path to opencl clhpp" FORCE)

# set(OpenCL_INCLUDE_DIR ${OpenCL_HEADERS_DIR} ${OpenCL_CLHPP_DIR} CACHE PATH "Path to opencl (header and clhpp)" FORCE)

# message(STATUS "git opencl includes: ${OpenCL_INCLUDE_DIR}")

# set(OpenCL_VERSION "${OpenCL_VERSION_MAJOR}${OpenCL_VERSION_MINOR}0" CACHE STRING "OpenCL version" FORCE)
set(OpenCL_VERSION "120" CACHE STRING "OpenCL version" FORCE)