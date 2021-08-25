
add_executable(platform_test platform_test.cpp)
add_dependencies(platform_test CLIc) 
target_link_libraries(platform_test PRIVATE CLIc::CLIc)
add_test(NAME platform_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/platform_test)

add_executable(device_test device_test.cpp)
add_dependencies(device_test CLIc) 
target_link_libraries(device_test PRIVATE CLIc::CLIc)
add_test(NAME device_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/device_test)

add_executable(context_test context_test.cpp)
add_dependencies(context_test CLIc) 
target_link_libraries(context_test PRIVATE CLIc::CLIc)
add_test(NAME context_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/context_test)

add_executable(commandqueue_test commandqueue_test.cpp)
add_dependencies(commandqueue_test CLIc) 
target_link_libraries(commandqueue_test PRIVATE CLIc::CLIc)
add_test(NAME commandqueue_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/commandqueue_test)

add_executable(gpu_test gpu_test.cpp)
add_dependencies(gpu_test CLIc) 
target_link_libraries(gpu_test PRIVATE CLIc::CLIc)
add_test(NAME gpu_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gpu_test)

add_executable(buffer_test buffer_test.cpp)
add_dependencies(buffer_test CLIc) 
target_link_libraries(buffer_test PRIVATE CLIc::CLIc)
add_test(NAME buffer_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test)

add_executable(object_test object_test.cpp)
add_dependencies(object_test CLIc) 
target_link_libraries(object_test PRIVATE CLIc::CLIc)
add_test(NAME object_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/object_test)

add_executable(operations_test operations_test.cpp)
add_dependencies(operations_test CLIc) 
target_link_libraries(operations_test PRIVATE CLIc::CLIc)
add_test(NAME operations_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/operations_test)

add_executable(types_test types_test.cpp)
add_dependencies(types_test CLIc) 
target_link_libraries(types_test PRIVATE CLIc::CLIc)
add_test(NAME types_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/types_test)
