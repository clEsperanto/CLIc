message(STATUS "Backend selected: ${CLE_BACKEND_TYPE}")

set(CLE_OPENCL false)
set(CLE_CUDA false)

if(CLE_BACKEND_TYPE STREQUAL "OPENCL")
    # User can bypass find_package by providing:
    #   -DOpenCL_LIBRARIES=/path/to/libOpenCL.so
    #   -DOpenCL_INCLUDE_DIRS=/path/to/opencl/headers
    if(OpenCL_LIBRARIES AND OpenCL_INCLUDE_DIRS)
        message(STATUS "Using user-provided OpenCL paths")
    else()
        find_package(OpenCL REQUIRED)
    endif()
    set(CLE_OPENCL true)
    message(STATUS "OpenCL library  : ${OpenCL_LIBRARIES}")
    message(STATUS "OpenCL includes : ${OpenCL_INCLUDE_DIRS}")
elseif(CLE_BACKEND_TYPE STREQUAL "CUDA")
    # User can bypass find_package by providing:
    #   -DCUDAToolkit_LIBRARY_DIR=/path/to/cuda/lib
    #   -DCUDAToolkit_INCLUDE_DIRS=/path/to/cuda/include
    if(CUDAToolkit_LIBRARY_DIR AND CUDAToolkit_INCLUDE_DIRS)
        message(STATUS "Using user-provided CUDAToolkit paths")
    else()
        find_package(CUDAToolkit REQUIRED)
    endif()
    set(CLE_CUDA true)
    message(STATUS "CUDAToolkit library  : ${CUDAToolkit_LIBRARY_DIR}")
    message(STATUS "CUDAToolkit includes : ${CUDAToolkit_INCLUDE_DIRS}")
endif()

# add the compile definition for the selected backend
add_compile_definitions(
    $<$<BOOL:${CLE_CUDA}>:CLE_CUDA>
    $<$<BOOL:${CLE_OPENCL}>:CLE_OPENCL>
)
