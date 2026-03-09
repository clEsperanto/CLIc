# Copilot Instructions for CLIc

This file provides guidance to copilot on how to assist with code generation and suggestions for the CLIc repository, as part of the clEsperanto project.

## clEsperanto Overview
clEsperanto is a library for image processing with GPU-acceleration. It is composed of several components:
- **CLIc**: a C++ Library, the principal component of clEsperanto, which manage GPU resources and provides image processing algorithms. The library can be compiled for OpenCL (main target) or CUDA (experimental).
- **pyclEsperanto**: a Python wrapper around CLIc (using pybind11), allowing users to utilize clEsperanto's capabilities in Python, and aim to be compatible with the numpy API and environment.
- **clEsperantoJ**: a Java wrapper around CLIc (using javaCPP), allowing users to utilize clEsperanto's capabilities in Java, and aim to be compatible with ImagePlus and ImgLib2.

This repository focuses on the CLIc component, which is the core of the clEsperanto project. It provides the underlying functionality that both pyclEsperanto and clEsperantoJ rely on.

### Key principles:

- Portability - Support for multiple platforms and GPU backends (OpenCL, CUDA)
- Simplicity - Minimal dependencies, straightforward API
- Efficiency - Shared pointer management, optimized algorithms, minimal copying of data

### Architecture:

- CLIc is implemented with C++ library for the data structures (Array) and for the GPU resources (Device, Backends) while the rest are functions that operate on these data structures and resources.
- Each language wrapper (python, Java) has its own repository and build process which depends on the CLIc library. The wrappers are designed to be as thin as possible, delegating most of the work to the CLIc library while taking the responsibility for the language-specific interface (e.g., numpy API for pyclEsperanto).
- The algorithms function are organised as Tiers, with tier1 being the most basic and fully GPU code, tier2 relying on tier1 functions, tier3 relying on tier2 functions and so on.

### Repository Structure:

```
CLIc/                            # Meta-repository (THIS REPO)
├── README.md                    # Main project README with protocol spec
├── LICENSE.md                   # License file
├── CMakeLists.txt               # CMake configuration for the CLIc Project
├── clic/                        # Comprehensive Sphinx documentation
│   ├── include/                 # Include folder (header files for the library)
│   ├── src/                     # Source folder (source files for the library)
│   ├── thirdparty/              # Third-party dependencies (no code, only CMakeLists.txt fetching dependencies)
│   ├── CMakeLists.txt           # CMake configuration in charge of the library build
│   └── clic.hpp.in              # Configuration header template for the library managed by CMake
├── tests/                       # Unit tests folder(using goolgle test framework and ctest)
├── .clang-format                # Clang-format configuration
├── .gitignore                   # Git ignore file
├── .pre-commit-config.yaml      # Pre-commit configuration file
├── cmake/                       # CMake configuration folder (presets and other variables)
└── CMakePresets.json            # Main CMake presets file
```

### Related Repositories

The Python and Java wrappers are in separate repositories

- pyclesperanto: python wrapper implementation at ../pyclesperanto/
  - See ../pyclesperanto/.github/copilot-instructions.md for Python-specific
- clEsperantoJ: Java wrapper implementation at ../clEsperantoJ/
  - See ../clEsperantoJ/.github/copilot-instructions.md for Java-specific

The documentation repository (no implementation)

- clesperanto-docs: documentation repository at ../clesperanto-docs/
  - See ../clesperanto-docs/.github/copilot-instructions.md for documentation-specific

## Contribution Guidelines

- Follow the C++17 standard for code and features.
- Use the standard library.
- Extend the existing tests for any new functionality added to the core library.
- Focus on minimal and clean code, with clear but minimal documentation and comments.
- Do not break the API, and if you need to, ask for validation, justify the change, and keep legacy support.
