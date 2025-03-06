message(STATUS "Searching for backend dependencies")

set(CLE_OPENCL false)
set(CLE_CUDA false)

if (OpenCL_LIBRARIES AND OpenCL_INCLUDE_DIRS)
    set(OpenCL_FOUND true)
else()
    find_package(OpenCL)
endif()
if (OpenCL_FOUND)
    message(STATUS "OpenCL library : ${OpenCL_LIBRARIES}")
    message(STATUS "OpenCL includes : ${OpenCL_INCLUDE_DIRS}")
    set(CLE_OPENCL true)
endif()

# if(CUDAToolkit_LIBRARY_DIR AND CUDAToolkit_INCLUDE_DIRS)
#     set(CUDAToolkit_FOUND true)
# else()
#     find_package(CUDAToolkit)
# endif()
# if (CUDAToolkit_FOUND)
#     set(CLE_CUDA true)
#     message(STATUS "CUDAToolkit library : ${CUDAToolkit_LIBRARY_DIR}")
#     message(STATUS "CUDAToolkit includes : ${CUDAToolkit_INCLUDE_DIRS}")
# endif()

if (NOT OpenCL_FOUND AND NOT CUDAToolkit_FOUND)
    message(FATAL_ERROR "No GPU framework found (OpenCL, CUDA). Please provide one of these GPU frameworks in order to compile the library.")
endif()

# add the variables to the compile definitions for the source code
add_compile_definitions(
    $<$<BOOL:${CLE_CUDA}>:CLE_CUDA>
    $<$<BOOL:${CLE_OPENCL}>:CLE_OPENCL>
)
