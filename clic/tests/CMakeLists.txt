

add_executable(gpu_test gpu_test.cpp)
add_dependencies(gpu_test CLIc) 
target_link_libraries(gpu_test PRIVATE CLIc::CLIc)
add_test(NAME gpu_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gpu_test)
set_target_properties(gpu_test PROPERTIES FOLDER "Tests")

add_executable(scalar_test scalar_test.cpp)
add_dependencies(scalar_test CLIc) 
target_link_libraries(scalar_test PRIVATE CLIc::CLIc)
add_test(NAME scalar_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/scalar_test)
set_target_properties(scalar_test PROPERTIES FOLDER "Tests")

add_executable(buffer_test buffer_test.cpp)
add_dependencies(buffer_test CLIc) 
target_link_libraries(buffer_test PRIVATE CLIc::CLIc)
add_test(NAME buffer_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test)
set_target_properties(buffer_test PROPERTIES FOLDER "Tests")

add_executable(image_test image_test.cpp)
add_dependencies(image_test CLIc) 
target_link_libraries(image_test PRIVATE CLIc::CLIc)
add_test(NAME image_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test)
set_target_properties(image_test PROPERTIES FOLDER "Tests")

add_executable(kernel_test kernel_test.cpp)
add_dependencies(kernel_test CLIc) 
target_link_libraries(kernel_test PRIVATE CLIc::CLIc)
add_test(NAME kernel_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test)
set_target_properties(kernel_test PROPERTIES FOLDER "Tests")
