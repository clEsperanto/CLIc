
add_executable(gpu_test gpu_test.cpp)
add_dependencies(gpu_test CLIc)
target_link_libraries(gpu_test PRIVATE CLIc::CLIc)
set_target_properties(gpu_test PROPERTIES FOLDER "Tests")

add_test(NAME core_gpu_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gpu_test)
set_tests_properties(
    core_gpu_test PROPERTIES LABELS "PROCESSOR" WILL_FAIL FALSE
)

add_executable(scalar_test scalar_test.cpp)
add_dependencies(scalar_test CLIc)
target_link_libraries(scalar_test PRIVATE CLIc::CLIc)
set_target_properties(scalar_test PROPERTIES FOLDER "Tests")

add_test(NAME core_scalar_test_int COMMAND ${CMAKE_CURRENT_BINARY_DIR}/scalar_test int 1)
add_test(NAME core_scalar_test_float COMMAND ${CMAKE_CURRENT_BINARY_DIR}/scalar_test float 1.5)
set_tests_properties(
    core_scalar_test_int
    core_scalar_test_float PROPERTIES LABELS "SCALAR" WILL_FAIL FALSE
)

add_executable(buffer_test buffer_test.cpp)
add_dependencies(buffer_test CLIc)
target_link_libraries(buffer_test PRIVATE CLIc::CLIc)
set_target_properties(buffer_test PROPERTIES FOLDER "Tests")

add_test(NAME core_buffer_test_float_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test float 10 1 1)
add_test(NAME core_buffer_test_int_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test int 10 1 1)
add_test(NAME core_buffer_test_short_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test short 10 1 1)
add_test(NAME core_buffer_test_char_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test char 10 1 1)
add_test(NAME core_buffer_test_uint_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test uint 10 1 1)
add_test(NAME core_buffer_test_ushort_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test ushort 10 1 1)
add_test(NAME core_buffer_test_uchar_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test uchar 10 1 1)
add_test(NAME core_buffer_test_float_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test float 10 5 1)
add_test(NAME core_buffer_test_int_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test int 10 5 1)
add_test(NAME core_buffer_test_short_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test short 10 5 1)
add_test(NAME core_buffer_test_char_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test char 10 5 1)
add_test(NAME core_buffer_test_uint_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test uint 10 5 1)
add_test(NAME core_buffer_test_ushort_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test ushort 10 5 1)
add_test(NAME core_buffer_test_uchar_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test uchar 10 5 1)
add_test(NAME core_buffer_test_float_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test float 10 5 3)
add_test(NAME core_buffer_test_int_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test int 10 5 3)
add_test(NAME core_buffer_test_short_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test short 10 5 3)
add_test(NAME core_buffer_test_char_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test char 10 5 3)
add_test(NAME core_buffer_test_uint_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test uint 10 5 3)
add_test(NAME core_buffer_test_ushort_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test ushort 10 5 3)
add_test(NAME core_buffer_test_uchar_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/buffer_test uchar 10 5 3)
set_tests_properties(
    core_buffer_test_float_1d
    core_buffer_test_int_1d
    core_buffer_test_short_1d
    core_buffer_test_char_1d
    core_buffer_test_uint_1d
    core_buffer_test_ushort_1d
    core_buffer_test_uchar_1d
    core_buffer_test_float_2d
    core_buffer_test_int_2d
    core_buffer_test_short_2d
    core_buffer_test_char_2d
    core_buffer_test_uint_2d
    core_buffer_test_ushort_2d
    core_buffer_test_uchar_2d
    core_buffer_test_float_3d
    core_buffer_test_int_3d
    core_buffer_test_short_3d
    core_buffer_test_char_3d
    core_buffer_test_uint_3d
    core_buffer_test_ushort_3d
    core_buffer_test_uchar_3d PROPERTIES LABELS "BUFFER" WILL_FAIL FALSE
)

add_executable(image_test image_test.cpp)
add_dependencies(image_test CLIc)
target_link_libraries(image_test PRIVATE CLIc::CLIc)
set_target_properties(image_test PROPERTIES FOLDER "Tests")

add_test(NAME core_image_test_float_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test float 10 1 1)
add_test(NAME core_image_test_int_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test int 10 1 1)
add_test(NAME core_image_test_short_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test short 10 1 1)
add_test(NAME core_image_test_char_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test char 10 1 1)
add_test(NAME core_image_test_uint_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test uint 10 1 1)
add_test(NAME core_image_test_ushort_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test ushort 10 1 1)
add_test(NAME core_image_test_uchar_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test uchar 10 1 1)
add_test(NAME core_image_test_float_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test float 10 5 1)
add_test(NAME core_image_test_int_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test int 10 5 1)
add_test(NAME core_image_test_short_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test short 10 5 1)
add_test(NAME core_image_test_char_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test char 10 5 1)
add_test(NAME core_image_test_uint_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test uint 10 5 1)
add_test(NAME core_image_test_ushort_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test ushort 10 5 1)
add_test(NAME core_image_test_uchar_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test uchar 10 5 1)
add_test(NAME core_image_test_float_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test float 10 5 3)
add_test(NAME core_image_test_int_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test int 10 5 3)
add_test(NAME core_image_test_short_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test short 10 5 3)
add_test(NAME core_image_test_char_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test char 10 5 3)
add_test(NAME core_image_test_uint_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test uint 10 5 3)
add_test(NAME core_image_test_ushort_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test ushort 10 5 3)
add_test(NAME core_image_test_uchar_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/image_test uchar 10 5 3)
set_tests_properties(
    core_image_test_float_1d
    core_image_test_int_1d
    core_image_test_short_1d
    core_image_test_char_1d
    core_image_test_uint_1d
    core_image_test_ushort_1d
    core_image_test_uchar_1d
    core_image_test_float_2d
    core_image_test_int_2d
    core_image_test_short_2d
    core_image_test_char_2d
    core_image_test_uint_2d
    core_image_test_ushort_2d
    core_image_test_uchar_2d
    core_image_test_float_3d
    core_image_test_int_3d
    core_image_test_short_3d
    core_image_test_char_3d
    core_image_test_uint_3d
    core_image_test_ushort_3d
    core_image_test_uchar_3d PROPERTIES LABELS "IMAGE" WILL_FAIL FALSE
)

add_executable(kernel_test kernel_test.cpp)
add_dependencies(kernel_test CLIc)
target_link_libraries(kernel_test PRIVATE CLIc::CLIc)
set_target_properties(kernel_test PROPERTIES FOLDER "Tests")

add_test(NAME core_kernel_test_float_buffer_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test float buffer 10 1 1)
add_test(NAME core_kernel_test_int_buffer_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test int buffer 10 1 1)
add_test(NAME core_kernel_test_short_buffer_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test short buffer 10 1 1)
add_test(NAME core_kernel_test_char_buffer_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test char buffer 10 1 1)
add_test(NAME core_kernel_test_uint_buffer_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test uint buffer 10 1 1)
add_test(NAME core_kernel_test_ushort_buffer_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test ushort buffer 10 1 1)
add_test(NAME core_kernel_test_uchar_buffer_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test char buffer 10 1 1)
add_test(NAME core_kernel_test_float_buffer_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test float buffer 10 7 1)
add_test(NAME core_kernel_test_int_buffer_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test int buffer 10 7 1)
add_test(NAME core_kernel_test_short_buffer_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test short buffer 10 7 1)
add_test(NAME core_kernel_test_char_buffer_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test char buffer 10 7 1)
add_test(NAME core_kernel_test_uint_buffer_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test uint buffer 10 7 1)
add_test(NAME core_kernel_test_ushort_buffer_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test ushort buffer 10 7 1)
add_test(NAME core_kernel_test_uchar_buffer_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test char buffer 10 7 1)
add_test(NAME core_kernel_test_float_buffer_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test float buffer 10 7 4)
add_test(NAME core_kernel_test_int_buffer_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test int buffer 10 7 4)
add_test(NAME core_kernel_test_short_buffer_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test short buffer 10 7 4)
add_test(NAME core_kernel_test_char_buffer_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test char buffer 10 7 4)
add_test(NAME core_kernel_test_uint_buffer_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test uint buffer 10 7 4)
add_test(NAME core_kernel_test_ushort_buffer_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test ushort buffer 10 7 4)
add_test(NAME core_kernel_test_uchar_buffer_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test char buffer 10 7 4)
add_test(NAME core_kernel_test_float_image_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test float image 10 1 1)
add_test(NAME core_kernel_test_int_image_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test int image 10 1 1)
add_test(NAME core_kernel_test_short_image_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test short image 10 1 1)
add_test(NAME core_kernel_test_char_image_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test char image 10 1 1)
add_test(NAME core_kernel_test_uint_image_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test uint image 10 1 1)
add_test(NAME core_kernel_test_ushort_image_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test ushort image 10 1 1)
add_test(NAME core_kernel_test_uchar_image_1d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test uchar image 10 1 1)
add_test(NAME core_kernel_test_float_image_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test float image 10 7 1)
add_test(NAME core_kernel_test_int_image_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test int image 10 7 1)
add_test(NAME core_kernel_test_short_image_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test short image 10 7 1)
add_test(NAME core_kernel_test_char_image_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test char image 10 7 1)
add_test(NAME core_kernel_test_uint_image_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test uint image 10 7 1)
add_test(NAME core_kernel_test_ushort_image_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test ushort image 10 7 1)
add_test(NAME core_kernel_test_uchar_image_2d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test uchar image 10 7 1)
add_test(NAME core_kernel_test_float_image_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test float image 10 7 4)
add_test(NAME core_kernel_test_int_image_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test int image 10 7 4)
add_test(NAME core_kernel_test_char_image_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test char image 10 7 4)
add_test(NAME core_kernel_test_short_image_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test short image 10 7 4)
add_test(NAME core_kernel_test_uint_image_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test uint image 10 7 4)
add_test(NAME core_kernel_test_ushort_image_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test ushort image 10 7 4)
add_test(NAME core_kernel_test_uchar_image_3d COMMAND ${CMAKE_CURRENT_BINARY_DIR}/kernel_test uchar image 10 7 4)
set_tests_properties(
    core_kernel_test_float_buffer_1d
    core_kernel_test_int_buffer_1d
    core_kernel_test_short_buffer_1d
    core_kernel_test_char_buffer_1d
    core_kernel_test_uint_buffer_1d
    core_kernel_test_ushort_buffer_1d
    core_kernel_test_uchar_buffer_1d
    core_kernel_test_float_buffer_2d
    core_kernel_test_int_buffer_2d
    core_kernel_test_short_buffer_2d
    core_kernel_test_char_buffer_2d
    core_kernel_test_uint_buffer_2d
    core_kernel_test_ushort_buffer_2d
    core_kernel_test_uchar_buffer_2d
    core_kernel_test_float_buffer_3d
    core_kernel_test_int_buffer_3d
    core_kernel_test_short_buffer_3d
    core_kernel_test_char_buffer_3d
    core_kernel_test_uint_buffer_3d
    core_kernel_test_ushort_buffer_3d
    core_kernel_test_uchar_buffer_3d
    core_kernel_test_float_image_1d
    core_kernel_test_int_image_1d
    core_kernel_test_short_image_1d
    core_kernel_test_char_image_1d
    core_kernel_test_uint_image_1d
    core_kernel_test_ushort_image_1d
    core_kernel_test_uchar_image_1d
    core_kernel_test_float_image_2d
    core_kernel_test_int_image_2d
    core_kernel_test_short_image_2d
    core_kernel_test_char_image_2d
    core_kernel_test_uint_image_2d
    core_kernel_test_ushort_image_2d
    core_kernel_test_uchar_image_2d
    core_kernel_test_float_image_3d
    core_kernel_test_int_image_3d
    core_kernel_test_char_image_3d
    core_kernel_test_short_image_3d
    core_kernel_test_uint_image_3d
    core_kernel_test_ushort_image_3d
    core_kernel_test_uchar_image_3d PROPERTIES LABELS "KERNEL" WILL_FAIL FALSE
)
