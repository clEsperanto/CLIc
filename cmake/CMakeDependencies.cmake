set(CLE_OPENCL false)
set(CLE_CUDA false)

# find GPU Framework (OpenCL, CUDA)
find_package(OpenCL)
if (OpenCL_FOUND)
    set(CLE_OPENCL true)
    message(STATUS "OpenCL found : ${CLE_OPENCL}")
endif()

find_package(CUDAToolkit)
if (CUDAToolkit_FOUND)
    set(CLE_CUDA true)
    message(STATUS "CUDA found : ${CLE_CUDA}")
endif()

if (NOT OpenCL_FOUND AND NOT CUDAToolkit_FOUND)
    # we need at least one of the two
    message(FATAL_ERROR "No GPU framework found (OpenCL, CUDA). Please install one of them in order to compile the librairy.")
endif()

# add the variables to the compile definitions for the source code
add_compile_definitions(
    $<$<BOOL:${CLE_CUDA}>:CLE_CUDA>
    $<$<BOOL:${CLE_OPENCL}>:CLE_OPENCL>
)
