
# stringify kernels and preamble
find_package(Python3 COMPONENTS Interpreter REQUIRED)
if(Python3_Interpreter_FOUND)
    message(CHECK_START "Generating kernerls headers")

    execute_process(
        COMMAND ${Python3_EXECUTABLE} ${PROJECT_UTILITIES_DIR}/stringify_clij.py ${CLIC_KERNELS_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/kernels/
        RESULT_VARIABLE CMD_ERROR 
        )
    execute_process(
        COMMAND ${Python3_EXECUTABLE} ${PROJECT_UTILITIES_DIR}/stringify_clij.py ${CLIC_PREAMBLE_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/core/includes/
        RESULT_VARIABLE CMD_ERROR 
        )
    
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/core/includes/cle_preamble.h)
        message(CHECK_PASS "done")
    else()
        message(CHECK_FAIL "Preamble header not found.")
        message(FATAL_ERROR "FATAL_ERROR: Kernel headers were not generated properly.")
    endif()
else()
    message(FATAL_ERROR "FATAL_ERROR: no Python interpreter found on the system.")
endif()


# # list kernel headers
# file(GLOB_RECURSE KERNELS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/kernels/*.h)

# list core files
file(GLOB_RECURSE HEADERS_core ${CMAKE_CURRENT_SOURCE_DIR}/core/includes/*.h)
file(GLOB_RECURSE SOURCES_core ${CMAKE_CURRENT_SOURCE_DIR}/core/src/*.cpp)

# list T1 files
file(GLOB_RECURSE HEADERS_T1 ${CMAKE_CURRENT_SOURCE_DIR}/tier1/includes/*.h)
file(GLOB_RECURSE SOURCES_T1 ${CMAKE_CURRENT_SOURCE_DIR}/tier1/src/*.cpp)

# # list T2 files
# file(GLOB_RECURSE HEADERS_T2 ${CMAKE_CURRENT_SOURCE_DIR}/tier2/includes/*.h)
# file(GLOB_RECURSE SOURCES_T2 ${CMAKE_CURRENT_SOURCE_DIR}/tier2/src/*.cpp)

# # list T3 files
# file(GLOB_RECURSE HEADERS_T3 ${CMAKE_CURRENT_SOURCE_DIR}/tier3/includes/*.h)
# file(GLOB_RECURSE SOURCES_T3 ${CMAKE_CURRENT_SOURCE_DIR}/tier3/src/*.cpp)

# # list T4 files
# file(GLOB_RECURSE HEADERS_T4 ${CMAKE_CURRENT_SOURCE_DIR}/tier4/includes/*.h)
# file(GLOB_RECURSE SOURCES_T4 ${CMAKE_CURRENT_SOURCE_DIR}/tier4/src/*.cpp)

# Configure Library
include(${PROJECT_SOURCE_DIR}/cmake/LibraryConfig.cmake)
