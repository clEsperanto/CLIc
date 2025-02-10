include(FetchContent)

## Fetch OpenCL kernel sources
FetchContent_Declare(cleKernels
    GIT_REPOSITORY https://github.com/clEsperanto/clij-opencl-kernels.git
    GIT_TAG        ${kernel_version_tag}
)
## Fetch Eigen library sources
FetchContent_Declare(eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG        ${eigen_lib_version_tag}
)
# Fetch clFFT library sources
FetchContent_Declare(clFFT
    GIT_REPOSITORY https://github.com/clMathLibraries/clFFT.git
    GIT_TAG        master
    SOURCE_SUBDIR  src  # clFFT CMakeLists.txt is in src/ instead of root
)
# Make the fetched content available
FetchContent_MakeAvailable(cleKernels eigen clFFT)


if(NOT CLFFT_INCLUDE_DIRS)
    set(CLFFT_INCLUDE_DIRS ${clFFT_SOURCE_DIR}/include ${clFFT_BINARY_DIR}/include)
endif()
if(NOT CLFFT_LIBRARIES)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CLFFT_LIBRARIES ${clFFT_BINARY_DIR}/library/${CMAKE_BUILD_TYPE}/libclFFT.so CACHE FILEPATH "Path to clFFT library")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CLFFT_LIBRARIES ${clFFT_BINARY_DIR}/library/${CMAKE_BUILD_TYPE}/libclFFT_d.so CACHE FILEPATH "Path to clFFT library")
    else()
        message(FATAL_ERROR "clFFT library not found for build type ${CMAKE_BUILD_TYPE}")
    endif()  # Close the else block here
endif()
