# Create and configure 'clic.h'
# - define OpenCL version and includes
# - define CLIC version
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME_LOWERCASE}.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME_LOWERCASE}.hpp"
    NO_SOURCE_PERMISSIONS @ONLY
)

# fetch sources (.cpp) and public headers (.hpp)
file(GLOB_RECURSE SOURCES_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/include/core/*.hpp)
file(GLOB_RECURSE OPENCL_FILE_H ${OpenCL_HEADERS_DIR}/*.h)
file(GLOB_RECURSE OPENCL_FILE_HPP ${OpenCL_CLHPP_DIR}/*.hpp)
list(APPEND OPENCL_FILE ${OPENCL_FILE_H} ${OPENCL_FILE_HPP})

# define library target
add_library(${LIBRARY_NAME} ${SOURCES_FILES})

set_target_properties(${LIBRARY_NAME} PROPERTIES FOLDER ${LIBRARY_DIR})

# Target links
target_link_libraries(${LIBRARY_NAME} PRIVATE ${OpenCL_LIBRARY})

# Alias: -> Foo::foo alias of foo
add_library(${PROJECT_NAME}::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

# Add definitions for targets
# Values:  - Debug  : -DFOO_DEBUG=1
# - Release: -DFOO_DEBUG=0
target_compile_definitions(${LIBRARY_NAME} PUBLIC "${PROJECT_NAME_UPPERCASE}_DEBUG=$<CONFIG:Debug>")

# Use C++17 on target (avoid global define)
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_17)

# Target dependencies
add_dependencies(${LIBRARY_NAME} generate_kernels_list generate_kernels)

add_custom_target(
    generate_kernels 
    COMMAND ${Python3_EXECUTABLE} ${UTILITIES_DIR}/convert_ocl_to_header.py ${OCL_KERNELS_DIR} ${CMAKE_CURRENT_BINARY_DIR}/kernels/
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/kernels 
    COMMENT "Generating kernels header from clij opencl files."
)

add_custom_target(
    generate_kernels_list 
    COMMAND ${Python3_EXECUTABLE} ${UTILITIES_DIR}/generate_kernellist_header.py ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/include
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/include/cleKernelList.hpp
    COMMENT "Generating cleKernelList header."
)

# fetch all include folders
file(GLOB child_dir RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/include/*)
set(INCLUDE_DIR_LIST "")

foreach(child ${child_dir})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${child})
        list(APPEND INCLUDE_DIR_LIST ${CMAKE_CURRENT_SOURCE_DIR}/include/${child})
    endif()
endforeach()

list(APPEND INCLUDE_DIR_LIST ${CMAKE_CURRENT_BINARY_DIR}/kernels)
list(APPEND INCLUDE_DIR_LIST ${CMAKE_CURRENT_BINARY_DIR}/include)

# Global includes. Used by all targets
# Note:
# - header can be included by C++ code `#include <foo.h>`
# - header location in project: ${CMAKE_CURRENT_BINARY_DIR}/generated_headers
target_include_directories(${LIBRARY_NAME} PUBLIC
    "$<BUILD_INTERFACE:${INCLUDE_DIR_LIST}>"
    "$<BUILD_INTERFACE:${OpenCL_INCLUDE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}>"
)

# Headers:
# - foo/*.h -> <prefix>/include/*.h
install(
    FILES ${HEADERS_FILE}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

# Headers:
# - foo/*.h -> <prefix>/include/*.h
install(
    FILES ${OPENCL_FILE}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/CL
)
install(
    TARGETS ${LIBRARY_NAME}
    EXPORT ${TARGETS_EXPORT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT RuntimeLibraries
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT RuntimeLibraries
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

# ------------------------------------------------------------------------------
# CMake Export Configuration
# This layout works for all platforms:
# - <install_prefix>/lib*/cmake/<PROJECT-NAME>
# - <install_prefix>/lib*/
# - <install_prefix>/include/
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
    EXPORT ${TARGETS_EXPORT_NAME}
    NAMESPACE ${PROJECT_NAME}::
    FILE ${TARGETS_EXPORT_NAME}.cmake
    DESTINATION ${CONFIG_INSTALL_DIR}
)
install(
    FILES "${PROJECT_BINARY_DIR}/${PROJECT_CONFIG_FILE}"
    "${PROJECT_BINARY_DIR}/${VERSION_CONFIG_FILE}"
    DESTINATION "${CONFIG_INSTALL_DIR}"
)
