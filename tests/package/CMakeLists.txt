# Minimal external project consuming CLIc, used to check that CLIc can be integrated in other C++ code.
#   - installed package:  cmake -S tests/package -B <dir> -DCMAKE_PREFIX_PATH=<CLIc install prefix>
#   - subproject:         cmake -S tests/package -B <dir> -DCLIC_SOURCE_DIR=<CLIc source tree>
# This directory is not part of the CLIc build: it is driven by the 'package_*' tests and the package CI workflow.
cmake_minimum_required(VERSION 3.20)

project(clic_package_test LANGUAGES CXX)

set(CLIC_SOURCE_DIR "" CACHE PATH "CLIc source tree to add as a subproject (default: use find_package)")

if(CLIC_SOURCE_DIR)
    set(BUILD_TESTING OFF)
    add_subdirectory(${CLIC_SOURCE_DIR} clic)
else()
    find_package(CLIc REQUIRED)
    message(STATUS "Found CLIc ${CLIc_VERSION} (backend: ${CLIc_BACKEND})")
endif()

add_executable(clic_package_test main.cpp)
target_link_libraries(clic_package_test PRIVATE CLIc::CLIc)

# Windows has no RPATH: put the CLIc DLL next to the executable
if(WIN32 AND CLIC_SOURCE_DIR)
    add_custom_command(TARGET clic_package_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:clic_package_test> $<TARGET_FILE_DIR:clic_package_test>
        COMMAND_EXPAND_LISTS
    )
endif()
