project(benchmark_kernel)

# retrieve CLIc includes from subprojects
get_target_property(CLIc_INCLUDES CLIc INCLUDE_DIRECTORIES)

add_library(benchmark_kernel STATIC
    benchmark_base.cpp
)
set_target_properties(benchmark_kernel PROPERTIES FOLDER "Benchmark")

add_dependencies(benchmark_kernel CLIc)

target_link_libraries(benchmark_kernel PRIVATE CLIc::CLIc)

target_include_directories(benchmark_kernel PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
    "$<BUILD_INTERFACE:${CLIc_INCLUDES}>"
)

add_executable(benchmark_dummy benchmark_dummy.cpp)
target_compile_features(benchmark_dummy PUBLIC cxx_std_17)
target_link_libraries(benchmark_dummy benchmark_kernel)
set_target_properties(benchmark_dummy PROPERTIES FOLDER "Benchmark")

add_executable(benchmark_mean benchmark_mean.cpp)
target_compile_features(benchmark_mean PUBLIC cxx_std_17)
target_link_libraries(benchmark_mean benchmark_kernel)
set_target_properties(benchmark_mean PROPERTIES FOLDER "Benchmark")

add_executable(benchmark_means_comparison benchmark_means_comparison.cpp)
target_compile_features(benchmark_means_comparison PUBLIC cxx_std_17)
target_link_libraries(benchmark_means_comparison benchmark_kernel CLIc::CLIc)
set_target_properties(benchmark_means_comparison PROPERTIES FOLDER "Benchmark")

add_executable(benchmark_maximum_projections benchmark_maximum_projections.cpp)
target_compile_features(benchmark_maximum_projections PUBLIC cxx_std_17)
target_link_libraries(benchmark_maximum_projections benchmark_kernel)
set_target_properties(benchmark_maximum_projections PROPERTIES FOLDER "Benchmark")
