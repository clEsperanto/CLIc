include(FetchContent)

# Prevent BUILD_SHARED_LIBS from leaking into GTest — always build it static.
# This eliminates the need to copy gtest.dll / gtest_main.dll entirely.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # Use shared CRT to match your project
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
    add_dependencies(${SUITE_NAME} CLIc)
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
