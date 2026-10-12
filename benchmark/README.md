# Benchmarking CLIc
This is the result of a small student project.

To create a new benchmark have a look at `benchmark_mean.cpp` or `benchmark_dummy.cpp`.
More complicated examples are `benchmark_means_comparison.cpp` and `benchmark_maximum_projections.cpp`.

These benchmarks are not built: they use the old CLIc API (`clesperanto.hpp`) and must be ported to `cle.hpp`
before being added back to the build.

Keep in mind that your new benchmark has to be added manually to this directories `CMakeLists.txt`.
