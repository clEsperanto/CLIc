cmake_minimum_required(VERSION 3.50)

project(clic-doxygen)

# Find Doxygen and Sphinx executables to build the documentation
find_package(Doxygen)
if (NOT DOXYGEN_FOUND)
    message(STATUS "Doxygen not found, documentation target will not be available.")
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
