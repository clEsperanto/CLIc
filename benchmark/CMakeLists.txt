
project(benchmark_kernel)

# uncomment to make this project external project
# find_package(CLIc 0.1.0 CONFIG REQUIRED)

# add binary
add_executable(benchmark_kernel benchmark_kernel.cpp)

# add library
target_link_libraries(benchmark_kernel PRIVATE CLIc::CLIc)