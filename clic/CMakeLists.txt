
# Create and configure 'clic.h'
#   - define OpenCL version
#   - include OpenCL headers
#   - define CLIC version
configure_file(
  "${PROJECT_SOURCE_DIR}/${LIBRARY_DIR}/${PROJECT_NAME_LOWERCASE}.h.in"
  "${PROJECT_SOURCE_DIR}/${LIBRARY_DIR}/includes/core/${PROJECT_NAME_LOWERCASE}.h"
  NO_SOURCE_PERMISSIONS @ONLY
)

# fetch all src and includes
set(SOURCES_FILES "")
subdirlist(SRC_DIR_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src)
foreach(src_dir ${SRC_DIR_LIST})
    file(GLOB_RECURSE files ${src_dir}/*.cpp)
    list(APPEND SOURCES_FILES ${files})
endforeach()

# find opencl
if(OpenCL_FOUND)
    include_directories(${OpenCL_INCLUDE_DIR})
    if(APPLE)
        set(OpenCL_CLHPP_HEADERS ${OpenCL_INCLUDE_DIR}/opencl/opencl.hpp)
    else()
        set(OpenCL_CLHPP_HEADERS ${OpenCL_INCLUDE_DIR}/CL/opencl.hpp)
    endif()
    if(NOT EXISTS ${OpenCL_CLHPP_HEADERS})
        message(WARNING "WARNING: OpenCL-CLHPP is required for a stable install of CLIc Library.")
        set(USE_PROJECT_CLHPP TRUE)
    endif()
endif()

if(NOT Python3_Interpreter_FOUND)
    message(FATAL_ERROR "FATAL_ERROR: no Python interpreter found on the system.")
endif()


# define library target
add_library(${LIBRARY_NAME} ${SOURCES_FILES})

# generate preamble headers
add_custom_target(
    generate_preamble ALL
    COMMAND ${Python3_EXECUTABLE} ${UTILITIES_DIR}/stringify_clij.py ${CLIC_PREAMBLE_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/includes/core/
    BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/includes/core/cle_preamble.h
    COMMENT "Generating cle preamble header."
)

# generate kernel headers
add_custom_target(
    generate_kernels ALL
    COMMAND ${Python3_EXECUTABLE} ${UTILITIES_DIR}/stringify_clij.py ${CLIC_KERNELS_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/kernels/
    BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/kernels #/timestamp.txt
    COMMENT "Generating cle kernels header."
)

# Target dependencies
add_dependencies(${LIBRARY_NAME} generate_preamble generate_kernels)

# Target links
target_link_libraries(${LIBRARY_NAME} OpenCL::OpenCL)

# Alias:
#   - Foo::foo alias of foo
add_library(${PROJECT_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

# C++11
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_11)

# Add definitions for targets
# Values:
#   - Debug  : -DFOO_DEBUG=1
#   - Release: -DFOO_DEBUG=0
target_compile_definitions(${LIBRARY_NAME} PUBLIC "${PROJECT_NAME_UPPERCASE}_DEBUG=$<CONFIG:Debug>")

# fetch all src and includes
set(HEADERS_FILES "")
subdirlist(INCLUDES_DIR_LIST ${CMAKE_CURRENT_SOURCE_DIR}/includes)
foreach(inc_dir ${INCLUDES_DIR_LIST})
    file(GLOB files ${inc_dir}/*.h)
    list(APPEND HEADERS_FILES ${files})
endforeach()
list(APPEND INCLUDES_DIR_LIST ${CMAKE_CURRENT_SOURCE_DIR}/kernels)
file(GLOB KERNELS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/kernels/*.h)

# Global includes. Used by all targets
# Note:
#   - header can be included by C++ code `#include <foo.h>`
#   - header location in project: ${CMAKE_CURRENT_BINARY_DIR}/generated_headers
target_include_directories( ${LIBRARY_NAME} PUBLIC 
    "$<BUILD_INTERFACE:${INCLUDES_DIR_LIST}>"
    "$<INSTALL_INTERFACE:.>"
)

# Use thirdparty header library opencl-clhpp required
if(USE_PROJECT_CLHPP)
    target_include_directories( ${LIBRARY_NAME} PUBLIC 
        "$<BUILD_INTERFACE:${THIRDPARTY_DIR}/OpenCL-CLHPP/include>"
    ) 
    list(APPEND ${PUBLIC_HEADERS} ${THIRDPARTY_DIR}/OpenCL-CLHPP/include/CL/opencl.hpp)
endif()




# Targets:
#   - <prefix>/lib/libfoo.a
#   - header location after install: <prefix>/foo/foo.h
#   - headers can be included by C++ code `#include <foo/foo.h>`
install(
    TARGETS              "${LIBRARY_NAME}"
    EXPORT               "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION  "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION  "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION  "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Headers:
#   - foo/*.h -> <prefix>/include/*.h
install(
    FILES       ${PUBLIC_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Kernels:
#   - foo/*.cl -> <prefix>/include/kernel/*.cl
install(
    FILES        ${KERNELS_FILES}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/kernels"
)

# Config
#   - <prefix>/lib/cmake/Foo/FooConfig.cmake
#   - <prefix>/lib/cmake/Foo/FooConfigVersion.cmake
install(
    FILES       "${PROJECT_CONFIG_FILE}"
                "${VERSION_CONFIG_FILE}"
    DESTINATION "${CONFIG_INSTALL_DIR}"
)

# Config
#   - <prefix>/lib/cmake/Foo/FooTargets.cmake
install(
  EXPORT      "${TARGETS_EXPORT_NAME}"
  FILE        "${PROJECT_NAME}Targets.cmake"
  DESTINATION "${CONFIG_INSTALL_DIR}"
  NAMESPACE   "${PROJECT_NAME}::"
)
