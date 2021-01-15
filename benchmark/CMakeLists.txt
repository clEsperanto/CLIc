
project(benchmark_kernel)

# uncomment to make this project external project
#find_package(CLIc 0.1.0 CONFIG REQUIRED)

# nifty hack, because CLIc does not import includes for subprojects
include_directories("${CMAKE_CURRENT_LIST_DIR}/../clic/include")

add_library(benchmark_kernel STATIC
    benchmark_kernel.cpp
    )
target_compile_features(benchmark_kernel PUBLIC cxx_std_11)
target_link_libraries(benchmark_kernel PRIVATE CLIc::CLIc)
target_include_directories(benchmark_kernel PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
    )

add_executable(benchmark_dummy benchmark_dummy.cpp)
target_compile_features(benchmark_dummy PUBLIC cxx_std_14)

target_link_libraries(benchmark_dummy benchmark_kernel)

add_executable(benchmark_mean benchmark_mean.cpp)
target_compile_features(benchmark_mean PUBLIC cxx_std_14)

target_link_libraries(benchmark_mean benchmark_kernel)
