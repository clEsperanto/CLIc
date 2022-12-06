# Add a valid test case

In order to ensure that the kernel is working as expected, write a test case and register it in `tests/CMakeLists.txt`.

## <a id="test_source">Write a test case</a>

In this example, add `tests/add_image_and_scalar_test.cpp`. In order to be able to generate random test data, we include `<random>`. Obviously, we also need to include `clesperanto.hpp`.

```cpp
#include <random>

#include "clesperanto.hpp"

template <class type>
auto
```
The test routine itself is defined in `run_test`, which accepts two parameters: `shape` and `mem_type`. In this case, the test routine simply tests whether the kernel manages to add a scalar (10) to a constant vector filled with the value 10.

```cpp
run_test(const std::array<size_t, 3> & shape, const cle::MemoryType & mem_type) -> bool
{
  const type        value = 10;
  const type        scalar = 10;
  std::vector<type> input(shape[0] * shape[1] * shape[2]);
  std::vector<type> valid(shape[0] * shape[1] * shape[2]);
  std::fill(input.begin(), input.end(), static_cast<type>(value));
  std::fill(valid.begin(), valid.end(), static_cast<type>(value + scalar));

  cle::Clesperanto cle;
  cle.GetDevice()->WaitForKernelToFinish();
  auto gpu_input = cle.Push<type>(input, shape, mem_type);
  auto gpu_output = cle.Create<type>(shape, mem_type);
  cle.AddImageAndScalar(gpu_input, gpu_output, scalar);
  auto output = cle.Pull<type>(gpu_output);

  return std::equal(output.begin(), output.end(), valid.begin());
}
```
The main function executes `run_test` with vectors of various shapes:
```cpp
auto
main(int argc, char ** argv) -> int
{
  if (!run_test<float>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }

  if (!run_test<signed int>({ 10, 1, 1 }, cle::BUFFER))
  {
    return EXIT_FAILURE;
  }
\\ many more different shapes are tested
}
```

## <a id="test_cmakelist">Register the test</a>

In `tests/CMakeLists.txt`, we need to register the text in three places:

In the beginning, we register the executable, its dependencies and define targets:
```makefile
add_executable(add_image_and_scalar_test add_image_and_scalar_test.cpp)
add_dependencies(add_image_and_scalar_test CLIc)
target_link_libraries(add_image_and_scalar_test PRIVATE CLIc::CLIc)
set_target_properties(add_image_and_scalar_test PROPERTIES FOLDER "Tests")
target_compile_features(add_image_and_scalar_test PRIVATE cxx_std_17)
```
below that, we add the test:
```makefile
add_test(NAME add_image_and_scalar_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND add_image_and_scalar_test)
```
finally, we have to pass the test to the `set_tests_properties` function:
```makefile
set_tests_properties(
    [...]
    add_image_and_scalar_test
    [...]
)
```

## Verify that the test is working

Now you can [build the binaries as described in the documentation](https://github.com/clEsperanto/CLIc_prototype/blob/master/docs/guidelines.md#source-compilation).

If the compilation succeeds, you can find your compiled test case in `build/tests/add_image_and_scalar_test`

Execute it and make sure it runs without errors.