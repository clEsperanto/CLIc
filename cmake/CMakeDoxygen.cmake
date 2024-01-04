find_package(Doxygen)

set(SOURCE_FOLDER ${CMAKE_SOURCE_DIR}/clic/)
configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

if(DOXYGEN_FOUND)
    set(DOXYGEN_INPUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")  # Path to the generated Doxyfile
    # Add a custom target to run Doxygen when building the documentation
    add_custom_target(
        documentation
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating Doxygen documentation"
        VERBATIM
    )
    # Add the 'documentation' target as a dependency for 'all' (build all by default)
    add_dependencies(${LIBRARY_NAME} documentation)
else()
    message(STATUS "Doxygen not found, documentation target will not be available.")
endif()
