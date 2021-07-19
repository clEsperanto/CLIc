
add_executable(gpu_test gpu_test.cpp)
add_dependencies(gpu_test CLIc) 
target_link_libraries(gpu_test PRIVATE CLIc::CLIc)
add_test(NAME gpu_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gpu_test)

add_executable(object_test object_test.cpp)
add_dependencies(object_test CLIc) 
target_link_libraries(object_test PRIVATE CLIc::CLIc)
add_test(NAME object_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/object_test)

add_executable(operations_test operations_test.cpp)
add_dependencies(operations_test CLIc) 
target_link_libraries(operations_test PRIVATE CLIc::CLIc)
add_test(NAME operations_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/operations_test)
