


# stringify kernels and preamble
find_package(PythonInterp REQUIRED)
set(STRINGIFY_KERNELS ${PYTHON_EXECUTABLE} ${PROJECT_UTILITIES_DIR}/stringify_clij.py ${CLIC_KERNELS_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/kernels)
execute_process(
    COMMAND ${STRINGIFY_KERNELS}
    RESULT_VARIABLE CMD_ERROR 
    )
set(STRINGIFY_PREAMBLE ${PYTHON_EXECUTABLE} ${PROJECT_UTILITIES_DIR}/stringify_clij.py ${CLIC_PREAMBLE_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/core/includes)
execute_process(
    COMMAND ${STRINGIFY_PREAMBLE}
    RESULT_VARIABLE CMD_ERROR 
    )

# list kernel headers
file(GLOB_RECURSE KERNELS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/kernels/*.h)

# list core files
file(GLOB_RECURSE HEADERS_core ${CMAKE_CURRENT_SOURCE_DIR}/core/includes/*.h)
file(GLOB_RECURSE SOURCES_core ${CMAKE_CURRENT_SOURCE_DIR}/core/src/*.cpp)

# list T1 files
file(GLOB_RECURSE HEADERS_T1 ${CMAKE_CURRENT_SOURCE_DIR}/tier1/includes/*.h)
file(GLOB_RECURSE SOURCES_T1 ${CMAKE_CURRENT_SOURCE_DIR}/tier1/src/*.cpp)

# Configure Library
include(${PROJECT_SOURCE_DIR}/cmake/LibraryConfig.cmake)
