include(FetchContent)

## Fetch OpenCL kernel sources
FetchContent_Declare(cleKernels
    GIT_REPOSITORY https://github.com/clEsperanto/clij-opencl-kernels.git
    GIT_TAG        ${kernel_version_tag}
)
FetchContent_MakeAvailable(cleKernels)

## Fetch Eigen library sources
FetchContent_Declare(eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG        ${eigen_lib_version_tag}
)
FetchContent_MakeAvailable(eigen)

# Future work - integration of clFFT library for deconvolution operations
## Fetch clFFT library sources
# FetchContent_Declare(clfft
#     GIT_REPOSITORY https://github.com/clMathLibraries/clFFT.git
#     GIT_TAG        master
#     SOURCE_SUBDIR  src  # clFFT CMakeLists.txt is in src/ instead of root
# )
# FetchContent_MakeAvailable(clfft)
# clFFT linking usinge the variable  ${CLFFT_LIBRARIES}