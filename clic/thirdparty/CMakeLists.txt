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

# Fetch DLPack headers only (do not build its sources)
FetchContent_Declare(
    dlpack
    GIT_REPOSITORY https://github.com/dmlc/dlpack.git
    GIT_TAG        v1.3 # or pin a specific commit
)
FetchContent_MakeAvailable(cleKernels eigen)

# Populate DLPack (download only, do not add_subdirectory)
if(NOT dlpack_POPULATED)
    FetchContent_Populate(dlpack)
endif()

# To use DLPack headers in your target, add this in your CMakeLists.txt:
# target_include_directories(<your_target> PUBLIC $<BUILD_INTERFACE:${dlpack_SOURCE_DIR}/include>)

# Fetch vkFFT library sources
# 0 - Vulkan, 1 - CUDA, 2 - HIP, 3 - OpenCL, 4 - Level Zero, 5 - Metal.
if (CLE_BACKEND STREQUAL "OPENCL")
    set(VKFFT_BACKEND 3 CACHE STRING "VkFFT backend")
elseif (CLE_BACKEND STREQUAL "CUDA")
    set(VKFFT_BACKEND 1 CACHE STRING "VkFFT backend")
elseif (CLE_BACKEND STREQUAL "METAL")
    set(VKFFT_BACKEND 5 CACHE STRING "VkFFT backend")
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
add_library(VkFFT INTERFACE)
target_compile_definitions(VkFFT INTERFACE -DVKFFT_BACKEND=${VKFFT_BACKEND} -DVKFFT_MAX_FFT_DIMENSIONS=${VKFFT_MAX_FFT_DIMENSIONS})
if (CLE_BACKEND STREQUAL "METAL")
    target_include_directories(VkFFT INTERFACE
        $<BUILD_INTERFACE:${vkfft_SOURCE_DIR}/metal-cpp>
    )
endif()
