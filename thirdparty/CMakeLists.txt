# define path dir to kernel and preamble from clesperanto kernel git repo.
set(OCL_KERNELS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/clesperanto-kernels CACHE PATH "Path to opencl kernel files (.cl)" FORCE)
mark_as_advanced(OCL_KERNELS_DIR)

# set(OpenCL_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencl/ocl-headers/" CACHE PATH "Path to opencl headers" FORCE)
# set(OpenCL_CLHPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencl/ocl-clhpp/include/" CACHE PATH "Path to opencl clhpp" FORCE)
# set(OpenCL_INCLUDE_DIR ${OpenCL_HEADERS_DIR} ${OpenCL_CLHPP_DIR} CACHE PATH "Path to opencl (header and clhpp)" FORCE)

include(FetchContent)
# include(ExternalProject)

FetchContent_Declare(OpenCL_HEADERS
    GIT_REPOSITORY https://github.com/KhronosGroup/OpenCL-Headers.git
    GIT_TAG v2022.09.30
    CONFIGURE_COMMAND ""
    BUILD_ALWAYS OFF
    )
    
FetchContent_Declare(OpenCL_CLHPP
    GIT_REPOSITORY https://github.com/KhronosGroup/OpenCL-CLHPP.git
    GIT_TAG v2022.09.30
    CONFIGURE_COMMAND ""
    BUILD_ALWAYS OFF
    )

# if(NOT OpenCL_HEADERS_POPULATED)
#     FetchContent_MakeAvailable(OpenCL_HEADERS)
# endif()
# if(NOT OpenCL_CLHPP_POPULATED)
#     FetchContent_MakeAvailable(OpenCL_CLHPP)
# endif()

# FetchContent_GetProperties(OpenCL_HEADERS)
# FetchContent_GetProperties(OpenCL_CLHPP)

if(NOT OpenCL_HEADERS_POPULATED)
    FetchContent_Populate(OpenCL_HEADERS)
endif()
if(NOT OpenCL_CLHPP_POPULATED)
    FetchContent_Populate(OpenCL_CLHPP)
endif()

set(OpenCL_HEADERS_SRC_DIR ${CMAKE_BINARY_DIR}/_deps/opencl_headers-src CACHE PATH "Path to opencl headers" FORCE)
set(OpenCL_CLHPP_SRC_DIR ${CMAKE_BINARY_DIR}/_deps/opencl_clhpp-src CACHE PATH "Path to opencl clhpp" FORCE)

message(STATUS "git opencl header src dir: ${OpenCL_HEADERS_SRC_DIR}")
message(STATUS "git opencl clhpp src dir: ${OpenCL_CLHPP_SRC_DIR}")


# ExternalProject_Add(OpenCL_HEADERS
#     SOURCE_DIR ${OpenCL_HEADERS_SRC_DIR}
#     BUILD_ALWAYS OFF
#     INSTALL_COMMAND ""
#     CONFIGURE_COMMAND ""
# )

# ExternalProject_Add(OpenCL_CLHPP
#     SOURCE_DIR ${OpenCL_CLHPP_SRC_DIR}
#     BUILD_ALWAYS OFF
#     INSTALL_COMMAND ""
#     CONFIGURE_COMMAND ""
# )

# FetchContent_GetProperties(OpenCL_HEADERS SOURCE_DIR)
# message(STATUS "git opencl header dir: ${SOURCE_DIR}")

# FetchContent_GetProperties(OpenCL_CLHPP SOURCE_DIR)
# message(STATUS "git opencl header dir: ${SOURCE_DIR}")

# ExternalProject_Get_Property(OpenCL_HEADERS SOURCE_DIR)
# message(STATUS "git opencl header dir: ${SOURCE_DIR}")
# set(OpenCL_HEADERS_DIR "${CMAKE_CURRENT_BINARY_DIR}/OpenCL_headers/" CACHE PATH "Path to opencl headers" FORCE)

# ExternalProject_Get_Property(OpenCL_CLHPP SOURCE_DIR)
# message(STATUS "git opencl header dir: ${SOURCE_DIR}")
# set(OpenCL_CLHPP_DIR "${CMAKE_CURRENT_BINARY_DIR}/OpenCL_clhpp/include/" CACHE PATH "Path to opencl clhpp" FORCE)

set(OpenCL_INCLUDE_DIR ${OpenCL_HEADERS_SRC_DIR} ${OpenCL_CLHPP_SRC_DIR}/include/ CACHE PATH "Path to opencl (header and clhpp)" FORCE)
message(STATUS "git opencl includes: ${OpenCL_INCLUDE_DIR}")

# set(OpenCL_VERSION "${OpenCL_VERSION_MAJOR}${OpenCL_VERSION_MINOR}0" CACHE STRING "OpenCL version" FORCE)
set(OpenCL_VERSION "120" CACHE STRING "OpenCL version" FORCE)