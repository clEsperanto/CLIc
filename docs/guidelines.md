# Guidelines

## Requierements

1. A C++ compiler (e.g. gcc, clang, msvc)
2. [CMake](https://cmake.org/download/), version 3.20 or higher.
3. [OpenCL](https://www.khronos.org/opencl/)

## OpenCL installation

CLIc rely on the OpenCL library to operate. The OpenCL library is provided by the vendor of your GPU. Please follow the [instruction to install OpenCL](./how_to_build/opencl_installation.md) before considering using CLIc.

## Source compilation

|[Windows](./how_to_build/windows_build/windows_build.md)|[Linux](./how_to_build/linux_build/linux_build.md)|[MacOS](./how_to_build/macos_build/macos_build.md)|
|-|-|-|

## Developpeur guidelines

All contribution should be done through forks and pull requests. Please follow the [contribution guidelines](./contribution_guidelines.md) to contribute to CLIc.

Add a new kernels to CLIc:
- Provide an nD opencl kernel to the [clesperanto-kernels](https://github.com/clEsperanto/clij-opencl-kernels/tree/clesperanto_kernels) repository
- Add a new kernel class to CLIc (see [here](./how_to_contribute/kernel_class_creation.md))
    - Create the [header file](./how_to_contribute/kernel_class_creation.md#header) for the new kernel class
    - Create the [source file](./how_to_contribute/kernel_class_creation.md#source) for the new kernel class
    - Provide a `Call` function and a `Gateway method` to operate the new kernel class (see [here](./how_to_contribute/kernel_class_creation.md#call-and-gateway-access))
- Add a test case for the new kernel class (see [here](./how_to_contribute/kernel_test_case.md))
    - Create the [source file](./how_to_contribute/kernel_test_case.md#test_source) of the test case
    - Add the test case to the [test suite](./how_to_contribute/kernel_test_case.md#test_cmakelist)
- All code should respect the coding style enforced through the use of [clang-format](https://clang.llvm.org/docs/ClangFormat.html). See [here](./how_to_contribute/coding_style.md) for more information.

If you do not know where to start, you can check the [list of kernels](https://github.com/clEsperanto/CLIc_prototype/issues/120) still missing, and do not hesitate to take contact with use through an issue.


## FAQ

**WIP**

## More help?

Do not hesitate to create [an issue](https://github.com/clEsperanto/CLIc_prototype/issues) if you are facing un-documented errors or difficulties. Or via the [clEsperanto](https://forum.image.sc/tags/clesperanto) section of the `image.sc` forum.