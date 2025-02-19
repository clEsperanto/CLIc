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
    GIT_TAG        test-cmake-export #master
    SOURCE_SUBDIR  src  # clFFT CMakeLists.txt is in src/ instead of root
)
# Fetched the content of the three libraries
FetchContent_MakeAvailable(cleKernels eigen clFFT)

