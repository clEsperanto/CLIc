
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

# Shared function: builds one test executable per tier from all .cpp files in the directory,
# then uses gtest_discover_tests to register individual test cases with CTest.
function(add_clic_test_suite SUITE_NAME LABEL)
    file(GLOB SUITE_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

    if(NOT SUITE_SOURCES)
        return()
    endif()

    add_executable(${SUITE_NAME} ${SUITE_SOURCES})
    add_dependencies(${SUITE_NAME} CLIc gtest gtest_main)
    target_link_libraries(${SUITE_NAME} PRIVATE CLIc::CLIc gtest gtest_main)
    target_include_directories(${SUITE_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/tests)
    set_target_properties(${SUITE_NAME} PROPERTIES FOLDER "Tests")

    # On Windows, copy DLLs to test executable directory before test discovery
    if(WIN32)
        add_custom_command(TARGET ${SUITE_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_if_different
                               "$<TARGET_FILE:gtest>"
                               "$<TARGET_FILE_DIR:${SUITE_NAME}>")
        add_custom_command(TARGET ${SUITE_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_if_different
                               "$<TARGET_FILE:gtest_main>"
                               "$<TARGET_FILE_DIR:${SUITE_NAME}>")
        add_custom_command(TARGET ${SUITE_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_if_different
                               "$<TARGET_FILE:CLIc>"
                               "$<TARGET_FILE_DIR:${SUITE_NAME}>")
    endif()

    # For Windows, defer test discovery to runtime to ensure DLLs are available
    if(WIN32)
        gtest_discover_tests(${SUITE_NAME}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            PROPERTIES LABELS "${LABEL}"
            DISCOVERY_MODE POST_BUILD
        )
    else()
        gtest_discover_tests(${SUITE_NAME}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            PROPERTIES LABELS "${LABEL}"
        )
    endif()

    # For CUDA backend, set library path for test execution on Linux
    if(CLE_BACKEND_TYPE STREQUAL "CUDA" AND UNIX AND NOT APPLE AND CUDAToolkit_LIBRARY_DIR)
        set_tests_properties(${SUITE_NAME} PROPERTIES
            ENVIRONMENT "LD_LIBRARY_PATH=${CUDAToolkit_LIBRARY_DIR}:$ENV{LD_LIBRARY_PATH}"
        )
    endif()
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
