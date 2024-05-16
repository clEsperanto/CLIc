cmake_minimum_required(VERSION 3.20)

project(clic-docs)

# Find Doxygen and Sphinx executables to build the documentation
find_package(Doxygen)
find_program(SPHINX
  NAMES sphinx-build
  DOC "Path to sphinx-build executable"
)

# If Doxygen or Sphinx are not found, the documentation target will not be available
if (NOT DOXYGEN_FOUND)
    message(STATUS "Doxygen not found, documentation target will not be available.")
    return()
endif()
if (NOT SPHINX)
    message(STATUS "Sphinx not found, documentation target will not be available.")
    return()
endif()

# Configure the Doxyfile with the correct input folder
set(SOURCE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/../clic/)
configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
set(DOXYGEN_INPUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")  # Path to the generated Doxyfile

# Add a custom target to run Doxygen when building the documentation
add_custom_target(
    doxygen
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating Doxygen documentation"
    VERBATIM
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/docs/xml
)

# Add a custom target to run Sphinx when building the documentation
add_custom_target(
    documentation
    COMMAND ${CMAKE_COMMAND} -E env CLIC_DOCS=${CMAKE_CURRENT_BINARY_DIR}/xml ${SPHINX} -b html ${CMAKE_CURRENT_SOURCE_DIR}/source ${CMAKE_CURRENT_SOURCE_DIR}/build
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating Sphinx documentation"
    VERBATIM
    DEPENDS doxygen
    BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/build
)
