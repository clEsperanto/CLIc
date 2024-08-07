
set(CURRENT_TIER "TIERS4")

# Get all test names in current tiers directory
file(GLOB TEST_NAMES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
string(REGEX REPLACE ".cpp" "" TEST_NAMES "${TEST_NAMES}")

# Add a test executable and link it to the CLIc library
function(add_test_executable test_name tier_name)
    add_executable(${test_name} ${test_name}.cpp)
    add_dependencies(${test_name} CLIc)
    target_link_libraries(${test_name} PRIVATE CLIc::CLIc gtest gtest_main)

    if(WIN32)
        add_custom_command(TARGET ${test_name} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            "$<TARGET_FILE:gtest>"
                            "$<TARGET_FILE_DIR:${test_name}>")
        add_custom_command(TARGET ${test_name} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            "$<TARGET_FILE:gtest_main>"
                            "$<TARGET_FILE_DIR:${test_name}>")
        add_custom_command(TARGET ${test_name} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            "$<TARGET_FILE:CLIc>"
                            "$<TARGET_FILE_DIR:${test_name}>")
    endif()

    set_target_properties(${test_name} PROPERTIES FOLDER "Tests")
    add_test(NAME ${test_name} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES LABELS ${tier_name} WILL_FAIL FALSE)
endfunction()

# Add all test executables
foreach(TEST_NAME IN LISTS TEST_NAMES)
    add_test_executable(${TEST_NAME} ${CURRENT_TIER})
endforeach()
