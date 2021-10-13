
# Create and configure 'clic.h'
#   - define OpenCL version
#   - include OpenCL headers
#   - define CLIC version
configure_file(
  "${PROJECT_SOURCE_DIR}/${LIBRARY_DIR}/${PROJECT_NAME_LOWERCASE}.hpp.in"
  "${PROJECT_SOURCE_DIR}/${LIBRARY_DIR}/includes/core/${PROJECT_NAME_LOWERCASE}.hpp"
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
    if(NOT EXISTS ${OpenCL_CLHPP_HEADERS})
      if(APPLE)
          set(OpenCL_CLHPP_HEADERS ${OpenCL_INCLUDE_DIR}/opencl/opencl.hpp)
      else()
          set(OpenCL_CLHPP_HEADERS ${OpenCL_INCLUDE_DIR}/CL/opencl.hpp)
      endif()
    endif()
    if(NOT EXISTS ${OpenCL_CLHPP_HEADERS})
        message(STATUS "Using project version of OpenCL-CLHPP.")
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
    COMMENT "Generating cle_preamble header."
)

# generate kernel headers
add_custom_target(
    generate_kernels ALL
    COMMAND ${Python3_EXECUTABLE} ${UTILITIES_DIR}/stringify_clij.py ${CLIC_KERNELS_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/kernels/
    BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/kernels 
    COMMENT "Generating kernels header from clij opencl files."
)

# generate kernel headers
add_custom_target(
    generate_kernels_list ALL
    COMMAND ${Python3_EXECUTABLE} ${UTILITIES_DIR}/autogeneration_script.py ${CMAKE_CURRENT_SOURCE_DIR}/includes
    BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/includes/core/cleKernelList.h 
    COMMENT "Generating cleKernelList header."
)

# Target dependencies
add_dependencies(${LIBRARY_NAME} generate_preamble generate_kernels generate_kernels_list)

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
set(PUBLIC_HEADERS "")
subdirlist(INCLUDES_DIR_LIST ${CMAKE_CURRENT_SOURCE_DIR}/includes)
foreach(inc_dir ${INCLUDES_DIR_LIST})
    file(GLOB files ${inc_dir}/*.hpp)
    list(APPEND PUBLIC_HEADERS ${files})
endforeach()
list(APPEND INCLUDES_DIR_LIST ${CMAKE_CURRENT_SOURCE_DIR}/kernels)
file(GLOB KERNELS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/kernels/*.h)

# Global includes. Used by all targets
# Note:
#   - header can be included by C++ code `#include <foo.h>`
#   - header location in project: ${CMAKE_CURRENT_BINARY_DIR}/generated_headers
target_include_directories( ${LIBRARY_NAME} PUBLIC 
    "$<BUILD_INTERFACE:${INCLUDES_DIR_LIST}>"
    "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
)

# Use thirdparty header library opencl-clhpp required
if(USE_PROJECT_CLHPP)
    target_include_directories( ${LIBRARY_NAME} PUBLIC 
        "$<BUILD_INTERFACE:${THIRDPARTY_DIR}/OpenCL-CLHPP/include>"
    ) 
    list(APPEND PUBLIC_HEADERS "${THIRDPARTY_DIR}/OpenCL-CLHPP/include/CL/opencl.hpp")
endif()

# Headers:
#   - foo/*.h -> <prefix>/include/*.h
install(
    FILES       ${PUBLIC_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

# Kernels:
#   - foo/*.cl -> <prefix>/include/kernel/*.cl
install(
    FILES        ${KERNELS_FILES}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/kernels"
)

install(
    TARGETS              ${LIBRARY_NAME}
    EXPORT               ${TARGETS_EXPORT_NAME}
    ARCHIVE DESTINATION  ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    LIBRARY DESTINATION  ${CMAKE_INSTALL_LIBDIR} COMPONENT RuntimeLibraries
    RUNTIME DESTINATION  ${CMAKE_INSTALL_BINDIR} COMPONENT RuntimeLibraries
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

#------------------------------------------------------------------------------
# CMake Export Configuration
# This layout works for all platforms:
#   - <prefix>/lib*/cmake/<PROJECT-NAME>
#   - <prefix>/lib*/
#   - <prefix>/include/

# Include module with functions:
#   - write_basic_package_version_file(...)
#   - configure_package_config_file(...)
include(CMakePackageConfigHelpers)

set(PROJECT_CONFIG_FILE "${PROJECT_NAME}Config.cmake")
set(VERSION_CONFIG_FILE "${PROJECT_NAME}ConfigVersion.cmake")

# Export for the build tree

write_basic_package_version_file( 
    "${PROJECT_BINARY_DIR}/${VERSION_CONFIG_FILE}"
    VERSION "${${PROJECT_NAME}_VERSION}"
    COMPATIBILITY SameMajorVersion
)

export(
    EXPORT ${TARGETS_EXPORT_NAME}
    NAMESPACE ${PROJECT_NAME}::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGETS_EXPORT_NAME}.cmake"
)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_CONFIG_FILE}"
    INSTALL_DESTINATION "${PROJECT_BINARY_DIR}"
)

# Uninstall targets (build tree only)
configure_file("${PROJECT_SOURCE_DIR}/cmake/Uninstall.cmake.in" "${PROJECT_BINARY_DIR}/Uninstall.cmake"
  IMMEDIATE @ONLY
)
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${PROJECT_BINARY_DIR}/Uninstall.cmake)

# Export for the install tree

set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

install(
    EXPORT      ${TARGETS_EXPORT_NAME}
    NAMESPACE   ${PROJECT_NAME}::
    FILE        ${TARGETS_EXPORT_NAME}.cmake
    DESTINATION ${CONFIG_INSTALL_DIR}
)

install(
    FILES       "${PROJECT_BINARY_DIR}/${PROJECT_CONFIG_FILE}"
                "${PROJECT_BINARY_DIR}/${VERSION_CONFIG_FILE}"
    DESTINATION "${CONFIG_INSTALL_DIR}"
)

