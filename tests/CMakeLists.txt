include(FetchContent)

# Prevent BUILD_SHARED_LIBS from leaking into GTest — always build it static.
# This eliminates the need to copy gtest.dll / gtest_main.dll entirely.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # Use shared CRT to match your project
set(INSTALL_GTEST OFF)  # 'cmake --install' installs CLIc only
set(BUILD_GMOCK OFF)    # tests use gtest only
set(BUILD_SHARED_LIBS_BACKUP ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)
set(BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS_BACKUP})

include(GoogleTest)

function(add_clic_test_suite SUITE_NAME LABEL)
    file(GLOB SUITE_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

    if(NOT SUITE_SOURCES)
        return()
    endif()

    add_executable(${SUITE_NAME} ${SUITE_SOURCES})
    target_link_libraries(${SUITE_NAME} PRIVATE CLIc::CLIc gtest gtest_main)
    target_include_directories(${SUITE_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/tests)
    set_target_properties(${SUITE_NAME} PROPERTIES FOLDER "Tests")

    # On Windows, only need to copy CLIc.dll (gtest is now static)
    if(WIN32 AND TARGET CLIc AND BUILD_SHARED_LIBS)
        add_custom_command(TARGET ${SUITE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:CLIc>"
                "$<TARGET_FILE_DIR:${SUITE_NAME}>"
            COMMENT "Copying CLIc.dll for ${SUITE_NAME}"
        )
    endif()

    # Build properties list for test discovery
    set(TEST_PROPERTIES LABELS "${LABEL}")

    # For CUDA backend on Linux, add library path to environment
    if(CLE_BACKEND STREQUAL "CUDA" AND UNIX AND NOT APPLE AND CUDAToolkit_LIBRARY_DIR)
        list(APPEND TEST_PROPERTIES
            ENVIRONMENT "LD_LIBRARY_PATH=${CUDAToolkit_LIBRARY_DIR}:$ENV{LD_LIBRARY_PATH}")
    endif()

    # PRE_TEST: discovery happens at ctest time, not build time.
    # This allows building on CI runners without GPU drivers/runtime.
    gtest_discover_tests(${SUITE_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        PROPERTIES ${TEST_PROPERTIES}
        DISCOVERY_MODE PRE_TEST
    )
endfunction()

add_subdirectory(core)
add_subdirectory(tier1)
add_subdirectory(tier2)
add_subdirectory(tier3)
add_subdirectory(tier4)
add_subdirectory(tier5)
add_subdirectory(tier6)
add_subdirectory(tier7)
add_subdirectory(tier8)

# Package test: install CLIc into the build tree, then build and run an external project (tests/package)
# that consumes it with find_package(CLIc). Skipped for coverage builds (instrumented library).
if(NOT BUILD_COVERAGE)
    set(PACKAGE_TEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/package")
    set(PACKAGE_BUILD_OPTIONS "-DCMAKE_PREFIX_PATH=${PACKAGE_TEST_DIR}/install" "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
    if(CMAKE_GENERATOR_PLATFORM)
        set(PACKAGE_PLATFORM_OPTION --build-generator-platform "${CMAKE_GENERATOR_PLATFORM}")
    endif()

    add_test(NAME package_install
        COMMAND ${CMAKE_COMMAND} --install "${PROJECT_BINARY_DIR}" --prefix "${PACKAGE_TEST_DIR}/install" --config $<CONFIG>
    )
    add_test(NAME package_find_package
        COMMAND ${CMAKE_CTEST_COMMAND}
            --build-and-test "${CMAKE_CURRENT_SOURCE_DIR}/package" "${PACKAGE_TEST_DIR}/build"
            --build-generator "${CMAKE_GENERATOR}" ${PACKAGE_PLATFORM_OPTION}
            --build-config $<CONFIG>
            --build-options ${PACKAGE_BUILD_OPTIONS}
            --test-command clic_package_test
    )
    set_tests_properties(package_install PROPERTIES FIXTURES_SETUP clic_package LABELS package)
    set_tests_properties(package_find_package PROPERTIES FIXTURES_REQUIRED clic_package LABELS package)
    if(WIN32)
        set_tests_properties(package_find_package PROPERTIES
            ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${PACKAGE_TEST_DIR}/install/${CMAKE_INSTALL_BINDIR}")
    endif()
endif()
