# Set minimum version of CMake.
cmake_minimum_required(VERSION 3.20)

# Set project name and version
project(CLIc VERSION 0.5.2 LANGUAGES CXX)

# Set environment variables
include(${PROJECT_SOURCE_DIR}/cmake/SetEnv.cmake)

# find required package
find_package(OpenCL )
find_package(Python COMPONENTS Interpreter Development REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

set(PYCLIC_CMAKE_PACKAGE_INSTALL_SUBDIR "share/pyclic/cmake")


# Library sources
add_subdirectory(${THIRDPARTY_DIR})
add_subdirectory(${LIBRARY_DIR})

# Build and run tests
if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(${LIBRARY_DIR}/tests)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif(BUILD_TESTING)

# Build and run benchmark
if(BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif(BUILD_BENCHMARK)

if(BUILD_DOCUMENTATION)
    # find_package(Doxygen)
    # if(DOXYGEN_FOUND)
    #     # set input and output files
    #     set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    #     set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    #     # request to configure the file
    #     configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    #     message("Doxygen build started")
    #     # note the option ALL which allows to build the docs together with the application
    #     add_custom_target( doc_doxygen ALL
    #         COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    #         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    #         COMMENT "Generating API documentation with Doxygen"
    #         VERBATIM )
    # else(DOXYGEN_FOUND)
    #     message("Doxygen need to be installed to generate the documentation")
    # endif(DOXYGEN_FOUND)
endif(BUILD_DOCUMENTATION)




# split c++ stand-alone build from python wrapper build with scikit-build
if(DEFINED SKBUILD)

    set(ignoreMe "${SKBUILD}")
    set(PY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/python/)

    file(GLOB_RECURSE PY_SOURCES_FILES ${PROJECT_SOURCE_DIR}/python/pyclic/*.cpp) 

    # call pybind11-config to obtain the root of the cmake package
    execute_process(COMMAND ${PYTHON_EXECUTABLE} -m pybind11 --cmakedir
                    OUTPUT_VARIABLE pybind11_ROOT_RAW)
    string(STRIP ${pybind11_ROOT_RAW} pybind11_ROOT)
    find_package(pybind11)

    pybind11_add_module(_pyclic MODULE ${PY_SOURCES_FILES})
    target_link_libraries(_pyclic PRIVATE CLIc)
    add_dependencies(_pyclic CLIc)
    target_include_directories(_pyclic PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/python/pyclic/>")

    # Installing the extension module to the root of the package
    install(TARGETS _pyclic DESTINATION .)

    configure_file("${PROJECT_SOURCE_DIR}/python/pyclic/__main__.py.in"
                   "${PROJECT_BINARY_DIR}/python/pyclic/__main__.py")

    install(FILES "${PROJECT_BINARY_DIR}/python/pyclic/__main__.py" DESTINATION .)

    # if(APPLE)
    #     set_target_properties(
    #         _gpu PROPERTIES INSTALL_RPATH "@loader_path/${CMAKE_INSTALL_LIBDIR}")
    # else()
set_target_properties(_pyclic PROPERTIES INSTALL_RPATH
    "$ORIGIN/${CMAKE_INSTALL_LIBDIR}")
    # endif()

endif()


