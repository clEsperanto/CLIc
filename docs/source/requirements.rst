Requirements
============

To compile the library, you'll need the following software:

- `Git <https://git-scm.com/downloads>`__
- `CMake <https://cmake.org/download/>`__ (version 3.20 or higher)
- `OpenCL <https://www.khronos.org/opencl/>`__ and corresponding device drivers.
- `Python <https://www.python.org/downloads/>`__
- C++ Compiler:
    - On Windows, you can install it with `MSVC <https://visualstudio.microsoft.com/>`__
    - On macOS, you can install the `Xcode command line tools <https://developer.apple.com/xcode/resources/>`__
    - On Linux, you can install GCC with the `build-essential <https://packages.ubuntu.com/jammy/build-essential>`__ package.

For building the documentation, the additional library is required:

- `Sphinx <https://www.sphinx-doc.org/en/master/>`__

For building the CUDA backend verions (WIP), the following software is required:

- `CUDA Toolkit <https://developer.nvidia.com/cuda-toolkit>`__

.. note::

   Ensure that the following are available in your system's ``$PATH`` variable, especially Git and CMake.
