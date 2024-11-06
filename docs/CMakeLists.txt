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

# Add a custom target to run Doxygen when building the documentation
add_custom_target(
    doxygen
    COMMAND doxygen ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating Doxygen documentation"
    VERBATIM
)

# Add a custom target to run Sphinx when building the documentation
add_custom_target(
    documentation
    COMMAND make html
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating Sphinx documentation"
    VERBATIM
    DEPENDS doxygen
)
