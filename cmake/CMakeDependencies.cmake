message(STATUS "Search librairy dependencies")

set(CLE_OPENCL false)
set(CLE_CUDA false)

# find GPU Framework (OpenCL, CUDA)
if (DEFINED OpenCL_LIBRARIES AND DEFINED OpenCL_INCLUDE_DIRS)
    set(OpenCL_FOUND true)
else()
    find_package(OpenCL)
endif()
if (OpenCL_FOUND)
    set(CLE_OPENCL true)
    message(STATUS "OpenCL librairy : ${OpenCL_LIBRARIES}")
    message(STATUS "OpenCL includes : ${OpenCL_INCLUDE_DIRS}")
endif()

find_package(CUDAToolkit)
if (CUDAToolkit_FOUND)
    set(CLE_CUDA true)
    message(STATUS "CUDAToolkit librairy : ${CUDAToolkit_LIBRARY_DIR}")
    message(STATUS "CUDAToolkit includes : ${CUDAToolkit_INCLUDE_DIRS}")
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
