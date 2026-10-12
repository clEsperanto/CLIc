include(FetchContent)



## Fetch OpenCL kernel sources
FetchContent_Declare(cleKernels
    GIT_REPOSITORY https://github.com/clEsperanto/clij-opencl-kernels.git
    GIT_TAG        ${CLIC_KERNELS_TAG}
    GIT_SHALLOW ON
)

## Fetch Eigen library sources
FetchContent_Declare(eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG        ${CLIC_EIGEN_TAG}
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

# Fetch vkFFT library sources (header-only)
# VkFFT backend follows CLE_BACKEND: 0 - Vulkan, 1 - CUDA, 2 - HIP, 3 - OpenCL, 4 - Level Zero, 5 - Metal.
# Normal variables (not cache): they must follow CLE_BACKEND when it changes, and stay out of a parent's cache.
if (CLE_BACKEND STREQUAL "OPENCL")
    set(VKFFT_BACKEND 3)
elseif (CLE_BACKEND STREQUAL "CUDA")
    set(VKFFT_BACKEND 1)
elseif (CLE_BACKEND STREQUAL "METAL")
    set(VKFFT_BACKEND 5)
else()
    message(FATAL_ERROR "Unsupported backend type: ${CLE_BACKEND}")
endif()
set(VKFFT_MAX_FFT_DIMENSIONS 4)
FetchContent_Declare(VkFFT
    GIT_REPOSITORY https://github.com/DTolm/VkFFT
    GIT_TAG         ${CLIC_VKFFT_TAG}
    GIT_SHALLOW ON
)

if(NOT VkFFT_POPULATED)
    FetchContent_Populate(VkFFT)
endif()
# CLIc-prefixed target name, to avoid clashing with a parent project's own VkFFT; exported as CLIc::VkFFT
add_library(clic_vkfft INTERFACE)
set_target_properties(clic_vkfft PROPERTIES EXPORT_NAME VkFFT)
target_compile_definitions(clic_vkfft INTERFACE -DVKFFT_BACKEND=${VKFFT_BACKEND} -DVKFFT_MAX_FFT_DIMENSIONS=${VKFFT_MAX_FFT_DIMENSIONS})
if (CLE_BACKEND STREQUAL "METAL")
    target_include_directories(clic_vkfft INTERFACE
        $<BUILD_INTERFACE:${vkfft_SOURCE_DIR}/metal-cpp>
    )
endif()
