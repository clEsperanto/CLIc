include(FetchContent)

## Eigen: an installed Eigen >= 3.4 is used when found (point to one with Eigen3_DIR or Eigen3_ROOT), otherwise
## Eigen ${CLIC_EIGEN_TAG} is downloaded. The user package registry is skipped: it lists stale Eigen build trees.
# Each Eigen only accepts a request for its own major version (no range spans both): ask for 5, then for 3.4.
find_package(Eigen3 5 CONFIG QUIET NO_CMAKE_PACKAGE_REGISTRY)
if(NOT Eigen3_FOUND)
    find_package(Eigen3 3.4 CONFIG QUIET NO_CMAKE_PACKAGE_REGISTRY)
endif()
if(Eigen3_FOUND)
    message(STATUS "CLIc: using Eigen ${Eigen3_VERSION} from ${Eigen3_DIR}")
    set_target_properties(Eigen3::Eigen PROPERTIES IMPORTED_GLOBAL TRUE) # visible outside this directory
    set(CLIC_EIGEN3_DIR_HINT "${Eigen3_DIR}" PARENT_SCOPE)               # for CLIcConfig.cmake (not installed with CLIc)
endif()

## Configured dependencies (their CMakeLists.txt runs):
# - cleKernels: generates the cle_<name>.h kernel headers
# - Eigen (when downloaded): provides Eigen3::Eigen and the install rules used by the CLIc package and clesperantoj
#   (include/eigen3)
FetchContent_Declare(cleKernels
    GIT_REPOSITORY https://github.com/clEsperanto/clij-opencl-kernels.git
    GIT_TAG        ${CLIC_KERNELS_TAG}
    GIT_SHALLOW    ON
)
if(NOT Eigen3_FOUND)
    message(STATUS "CLIc: no Eigen >= 3.4 found, downloading Eigen ${CLIC_EIGEN_TAG}")
    FetchContent_Declare(eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG        ${CLIC_EIGEN_TAG}
        GIT_SHALLOW    ON
    )
    set(CLIC_EIGEN_DEPENDENCY eigen)
endif()

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

# Downloaded Eigen: install Eigen3Config.cmake (off by default in a subproject, CLIcConfig.cmake needs it) and keep
# its build tree out of the user package registry (~/.cmake/packages). Its tests and docs are off in a subproject.
set(EIGEN_BUILD_CMAKE_PACKAGE ON)
set(CMAKE_EXPORT_PACKAGE_REGISTRY OFF)

FetchContent_MakeAvailable(cleKernels ${CLIC_EIGEN_DEPENDENCY} dlpack VkFFT)

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
