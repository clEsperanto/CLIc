
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


