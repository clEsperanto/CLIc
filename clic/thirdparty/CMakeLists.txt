include(FetchContent)



## Fetch OpenCL kernel sources
FetchContent_Declare(cleKernels
    GIT_REPOSITORY https://github.com/clEsperanto/clij-opencl-kernels.git
    GIT_TAG        ${kernel_version_tag}
    GIT_SHALLOW ON
)

## Fetch Eigen library sources
FetchContent_Declare(eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG        ${eigen_lib_version_tag}
    GIT_SHALLOW ON
)

# # Fetch clFFT library sources
# FetchContent_Declare(clFFT
#     GIT_REPOSITORY https://github.com/clEsperanto/clFFT # https://github.com/clMathLibraries/clFFT.git
#     GIT_TAG        clic-build #master
#     GIT_SHALLOW ON
#     SOURCE_SUBDIR  src  # clFFT CMakeLists.txt is in src/ instead of root
# )

FetchContent_MakeAvailable(cleKernels eigen ) # clFFT

# test to avoid building eigen tests for fortran 
set(EIGEN_TEST_FORTRAN OFF CACHE BOOL "" FORCE)
set(EIGEN_LEAVE_TEST_IN_ALL_TARGET OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

# Fetch vkFFT library sources
set(VKFFT_BACKEND 3 CACHE STRING "VkFFT backend to use (1, 2, 3)")
set(VKFFT_MAX_FFT_DIMENSIONS 4 CACHE STRING "VkFFT maximum dimension supported")
FetchContent_Declare(VkFFT
    GIT_REPOSITORY https://github.com/DTolm/VkFFT
    GIT_TAG        v1.3.4 #master
    GIT_SHALLOW ON
)
# FetchContent_MakeAvailable(VkFFT)

if(NOT VkFFT_POPULATED)
    FetchContent_Populate(VkFFT)
endif()
add_library(VkFFT INTERFACE)
target_compile_definitions(VkFFT INTERFACE -DVKFFT_BACKEND=${VKFFT_BACKEND} -DVKFFT_MAX_FFT_DIMENSIONS=${VKFFT_MAX_FFT_DIMENSIONS})
