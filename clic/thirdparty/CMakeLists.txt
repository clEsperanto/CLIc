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

FetchContent_Declare(
    dlpack
    GIT_REPOSITORY https://github.com/dmlc/dlpack.git
    GIT_TAG        v1.3 # or pin a specific commit
)

FetchContent_MakeAvailable(cleKernels eigen dlpack) # clFFT

# test to avoid building eigen tests for fortran

# Fetch vkFFT library sources
# 0 - Vulkan, 1 - CUDA, 2 - HIP, 3 - OpenCL, 4 - Level Zero, 5 - Metal.
if (CLE_BACKEND STREQUAL "OPENCL")
    set(VKFFT_BACKEND 3 CACHE STRING "VkFFT backend")
elseif (CLE_BACKEND STREQUAL "CUDA")
    set(VKFFT_BACKEND 1 CACHE STRING "VkFFT backend")
else()
    message(FATAL_ERROR "Unsupported backend type: ${CLE_BACKEND}")
endif()
set(VKFFT_MAX_FFT_DIMENSIONS 4 CACHE STRING "VkFFT maximum dimension supported")
FetchContent_Declare(VkFFT
    GIT_REPOSITORY https://github.com/DTolm/VkFFT
    GIT_TAG         ${vkFFT_version_tag}
    GIT_SHALLOW ON
)
# FetchContent_MakeAvailable(VkFFT)

if(NOT VkFFT_POPULATED)
    FetchContent_Populate(VkFFT)
endif()
# add_library(VkFFT INTERFACE)
# target_compile_definitions(VkFFT INTERFACE -DVKFFT_BACKEND=${VKFFT_BACKEND} -DVKFFT_MAX_FFT_DIMENSIONS=${VKFFT_MAX_FFT_DIMENSIONS})
# T_POPULATED)
#     FetchContent_Populate(VkFFT)
# endif()
add_library(VkFFT INTERFACE)
target_compile_definitions(VkFFT INTERFACE -DVKFFT_BACKEND=${VKFFT_BACKEND} -DVKFFT_MAX_FFT_DIMENSIONS=${VKFFT_MAX_FFT_DIMENSIONS})
