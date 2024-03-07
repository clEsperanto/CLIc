
project(sphinxdoc)

find_package(Doxygen)
# find_package(Sphinx)
find_program(SPHINX
  NAMES sphinx-build
  DOC "Path to sphinx-build executable"
)

if (NOT DOXYGEN_FOUND)
    message(STATUS "Doxygen not found, documentation target will not be available.")
    return()
endif()

if (NOT SPHINX)
    message(STATUS "Sphinx not found, documentation target will not be available.")
    return()
endif()

set(SOURCE_FOLDER ${CMAKE_SOURCE_DIR}/clic/)

configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
set(DOXYGEN_INPUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")  # Path to the generated Doxyfile

# Add a custom target to run Doxygen when building the documentation
add_custom_target(
    documentation
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating Doxygen documentation"
    VERBATIM
)

# Add a custom target to run Sphinx when building the documentation
add_custom_target(
    sphinx-doc
    COMMAND ${CMAKE_COMMAND} -E env CLIC_DOCS=${CMAKE_CURRENT_BINARY_DIR}/xml ${SPHINX} -b html ${CMAKE_CURRENT_SOURCE_DIR}/source ${CMAKE_CURRENT_SOURCE_DIR}/build
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating Sphinx documentation"
    VERBATIM
    DEPENDS documentation
)

# Add the 'documentation' target as a dependency for 'all' (build all by default)
add_dependencies(${LIBRARY_NAME} sphinx-doc)
    
    
