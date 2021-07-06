#
# cmake library project by Pablo Speciale:
# https://github.com/pablospe/cmake-example-library
#

if(OpenCL_INCLUDE_DIR)
  find_package(OpenCL REQUIRED)
  if(OpenCL_FOUND)
    include_directories(${OpenCL_INCLUDE_DIR}) 
  endif()
endif()

# compile definition path to kernels and preamble .cl
#   - (alt. option) replace by a generated header file ?
# add_compile_definitions(KERNELS_DIR="${CLIC_KERNELS_DIR}")
# add_compile_definitions(PREAMBLE_OCL="${CLIC_PREAMBLE_FILE}")

set(PUBLIC_HEADERS ${HEADERS_core} ${HEADERS_T1})

add_library(${LIBRARY_NAME}
  ${SOURCES_core} ${HEADERS_core} 
  ${SOURCES_T1} ${HEADERS_T1}
  # ${SOURCES_T2} ${HEADERS_T2}
  # ${SOURCES_T3} ${HEADERS_T3}
  ${KERNELS_HEADERS}
)

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
#   - others : -DFOO_DEBUG=0
target_compile_definitions(${LIBRARY_NAME} PUBLIC
  "${PROJECT_NAME_UPPERCASE}_DEBUG=$<CONFIG:Debug>")

# Global includes. Used by all targets
# Note:
#   - header can be included by C++ code `#include <foo.h>`
#   - header location in project: ${CMAKE_CURRENT_BINARY_DIR}/generated_headers
target_include_directories(
  ${LIBRARY_NAME} PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${LIBRARY_FOLDER}/kernels>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${LIBRARY_FOLDER}/core/includes>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${LIBRARY_FOLDER}/tier1/includes>"
    # "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${LIBRARY_FOLDER}/tier2/includes>"
    # "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${LIBRARY_FOLDER}/tier3/includes>"
    "$<INSTALL_INTERFACE:.>"
)

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
    FILES        ${KERNELS_HEADERS}
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
