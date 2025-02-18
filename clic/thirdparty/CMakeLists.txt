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
    GIT_REPOSITORY https://github.com/clEsperanto/clFFT # https://github.com/clMathLibraries/clFFT.git
    GIT_TAG        clic-build #master
    SOURCE_SUBDIR  src  # clFFT CMakeLists.txt is in src/ instead of root
)
# Fetched the content of the three libraries
FetchContent_MakeAvailable(cleKernels eigen clFFT)

# clFFT configuration variables for clic
if(NOT CLFFT_INCLUDE_DIRS)
    set(CLFFT_INCLUDE_DIRS ${clFFT_SOURCE_DIR}/include ${clFFT_BINARY_DIR}/include CACHE PATH "Path to clFFT includes")
endif()
if(NOT CLFFT_LIBRARIES)
    set(CLFFT_LIBRARIES_SUFFIX "$<IF:$<CONFIG:Debug>,_d,>${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(CLFFT_LIBRARIES "${clFFT_BINARY_DIR}/library/$<CONFIG>/libclFFT${CLFFT_LIBRARIES_SUFFIX}" CACHE FILEPATH "Path to clFFT library")

    message(STATUS "clFFT Libraries Path: ${CLFFT_LIBRARIES}")
endif()
