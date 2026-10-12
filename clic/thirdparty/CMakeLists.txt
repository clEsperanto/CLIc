include(FetchContent)

## Configured dependencies (their CMakeLists.txt runs):
# - cleKernels: generates the cle_<name>.h kernel headers
# - Eigen: provides Eigen3::Eigen and the install rules used by the CLIc package and clesperantoj (include/eigen3)
FetchContent_Declare(cleKernels
    GIT_REPOSITORY https://github.com/clEsperanto/clij-opencl-kernels.git
    GIT_TAG        ${CLIC_KERNELS_TAG}
    GIT_SHALLOW    ON
)
FetchContent_Declare(eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG        ${CLIC_EIGEN_TAG}
    GIT_SHALLOW    ON
)

## Header-only dependencies, downloaded but never configured nor built: SOURCE_SUBDIR points to a folder without
## CMakeLists.txt, so FetchContent_MakeAvailable does not call add_subdirectory (their own CMake would build extra code).
FetchContent_Declare(dlpack
    GIT_REPOSITORY https://github.com/dmlc/dlpack.git
    GIT_TAG        v1.3
    GIT_SHALLOW    ON
    SOURCE_SUBDIR  download-only
)
FetchContent_Declare(VkFFT
    GIT_REPOSITORY https://github.com/DTolm/VkFFT
    GIT_TAG        ${CLIC_VKFFT_TAG}
    GIT_SHALLOW    ON
    SOURCE_SUBDIR  download-only
)

# Eigen without its tests and documentation. Normal variables, local to this directory (not in the cache);
# Eigen 3.4 requires CMake 3.5, so CMP0077 must be NEW for its option() calls to accept them.
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(BUILD_TESTING OFF)
set(EIGEN_BUILD_DOC OFF)

FetchContent_MakeAvailable(cleKernels eigen dlpack VkFFT)

# Header locations of the download-only dependencies, for clic/CMakeLists.txt and the install rules
set(CLIC_DLPACK_INCLUDE_DIR "${dlpack_SOURCE_DIR}/include" PARENT_SCOPE)
set(CLIC_VKFFT_SOURCE_DIR "${vkfft_SOURCE_DIR}" PARENT_SCOPE)

## VkFFT target (header-only)
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

# CLIc-prefixed target name, to avoid clashing with a parent project's own VkFFT; exported as CLIc::VkFFT
add_library(clic_vkfft INTERFACE)
set_target_properties(clic_vkfft PROPERTIES EXPORT_NAME VkFFT)
target_compile_definitions(clic_vkfft INTERFACE -DVKFFT_BACKEND=${VKFFT_BACKEND} -DVKFFT_MAX_FFT_DIMENSIONS=${VKFFT_MAX_FFT_DIMENSIONS})
if (CLE_BACKEND STREQUAL "METAL")
    target_include_directories(clic_vkfft INTERFACE
        $<BUILD_INTERFACE:${vkfft_SOURCE_DIR}/metal-cpp>
    )
endif()
