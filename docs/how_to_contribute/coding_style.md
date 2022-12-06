# Coding Style

## General

For clarity and consistency, we rely on clang-format to format the code. The [configuration file](../../.clang-format) is located in the root of the repository. It is strongly recommended to use the clang-format plugin for your favorite editor, or to use the command line tool.

The formatting rules are strongly inspired by the same rules used in the [ITK](https://github.com/InsightSoftwareConsortium/ITK) Library.

To format the code, run the following command at the root of your repository:

```bash
clang-format -i -style=file  $(find . -name "*.cpp" -or -name "*.hpp")
```

If you do not have clang-format, please refer to their [documentations](https://clang.llvm.org/docs/index.html) for installation, or to the following [issue](https://github.com/clEsperanto/CLIc_prototype/issues/116) on how to install it on your computer.

## Naming

### Variables

Variables should be named using `camelCase` convention. For example:

```cpp
int numberOfIterations = 10;
```

### Classes

Classes should be named using `PascalCase` convention. For example:

```cpp
class MyClass
{
};
```

All kernel classes should keep the `Kernel` suffix. For example:

```cpp 
class AddImagesKernel : public Operation
{
};
```

### Macros

Macros should be named using `UPPER_CASE` convention. For example:

```cpp
#define MY_MACRO 10
```

### Constants

Constants should be named using `UPPER_CASE` convention. For example:

```cpp
const int MY_CONSTANT = 10;
```

### Namespaces

Namespaces should be named using `lower_case` convention. For example:

```cpp
namespace my_namespace
{
}
```

Only two namespace are currently used in the library, `cle` and `backend`. Those are to differentiate the library code from the backend code (OpenCL).

### Files

Files should be named using the prefix `cle` and the suffix `hpp` or `cpp`. For example:

```bash
cleAddImagesKernel.hpp
cleAddImagesKernel.cpp
```
