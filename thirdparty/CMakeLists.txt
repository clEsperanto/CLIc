


# find opencl
set(OpenCL_CLHPP_FOUND FALSE)
find_package(OpenCL REQUIRED)
if(OpenCL_FOUND)
    include_directories(${OpenCL_INCLUDE_DIR})
    if(APPLE)
    message(STATUS ${OpenCL_INCLUDE_DIR}/opencl.hpp)
    if(EXISTS ${OpenCL_INCLUDE_DIR}/opencl.hpp)
        set(OpenCL_CLHPP_FOUND TRUE)    
    endif()
    else()
    message(STATUS ${OpenCL_INCLUDE_DIR}/CL/opencl.hpp)
    if(EXISTS ${OpenCL_INCLUDE_DIR}/CL/opencl.hpp)
        set(OpenCL_CLHPP_FOUND TRUE)    
    endif()
    endif()
endif()
if(NOT OpenCL_CLHPP_FOUND)
    message(FATAL_ERROR "FATAL ERROR: OpenCL-CLHPP is not install on the system.")
endif()

# Set Kernel directory
message(CHECK_START "Loading CLIJ Kernels")
if(IS_DIRECTORY ${CLIC_KERNELS_DIR}) 
    include_directories(${CLIC_KERNELS_DIR})
    file(GLOB KERNELS_LIST ${CLIC_KERNELS_DIR}/*.cl)
    message(CHECK_PASS "done")
else()
    message(CHECK_FAIL "failled")
    message(FATAL_ERROR "FATAL ERROR: Could not find CLIJ kernel folder.")
endif()


