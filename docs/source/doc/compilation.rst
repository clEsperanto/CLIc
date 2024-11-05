Requirements & Compilation
==========================

Requirements
------------

This section lists the software requirements for building the CLIc library.
If you never compiled a C++ project before, you may want to check out these tutorials before proceeding:

- `C++ Tutorial <https://www.tutorialspoint.com/cplusplus/index.htm>`__.
- `CMake Tutorial <https://cmake.org/cmake/help/latest/guide/tutorial/index.html>`__.

Otherwise, you'll need the following software to continue:

- `Git <https://git-scm.com/downloads>`__
- `CMake <https://cmake.org/download/>`__ (version 3.20 or higher)
- `OpenCL <https://www.khronos.org/opencl/>`__ (usually provided by the GPU vendor)
- `Python <https://www.python.org/downloads/>`__
- a C++ Compiler:
    - On Windows, you can install it with `MSVC <https://visualstudio.microsoft.com/>`__
    - On macOS, you can install the `Xcode command line tools <https://developer.apple.com/xcode/resources/>`__
    - On Linux, you can install GCC with the `build-essential <https://packages.ubuntu.com/jammy/build-essential>`__ package.
    - Or any other compiler that supports C++17 should work.

For building the documentation, the additional library is required:

- `Sphinx <https://www.sphinx-doc.org/en/master/>`__

.. note::

   Ensure that all the following software are available in your system's ``$PATH`` variable, especially Git and CMake.
   Please, refer to the respective software documentation for installation instructions and usage.

Compilation
-----------

Using terminal CLI
~~~~~~~~~~~~~~~~~~

First, clone the repository with Git:

.. code-block:: bash

   git clone https://github.com/clEsperanto/CLIc.git
   cd CLIc

We can then uses CMakes's command-line interface to configure

.. code-block:: bash

   cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON

The parameter ``-S`` specifies the source directory, and ``-B`` specifies the build directory. ``-G`` allows you to specify your project generator like Ninja, MSVC, XCode, or other compatible with CMake. You can also configure specific variable of the project as followed ``-D<NAME_OF_THE_VARIABLE>=<VALUE>``. In this case, we are setting the build type to ``Release`` and enabling the tests.

Once configure, we can build the project with the following command:

.. code-block:: bash

   cmake --build build --parallel 10 --target install

This command will build the project in parallel using 10 threads and install the project in the system.

.. note::

   By default the installation directory follows a GNU standard, which is usually ``/usr/local`` for Unix system. If you want to change the installation directory, you can set the variable ``CMAKE_INSTALL_PREFIX`` during the configuration step.
   This allows you to better control where the library and headers are installed.

Using CMake GUI
~~~~~~~~~~~~~~~

If you prefer a graphical interface, you can use CMake's GUI. The logic is the same as the command-line interface, but you can set the variables in the GUI.

1. Open CMake GUI
2. Set the source directory to the path where you cloned the repository.
3. Set the build directory to a path where you want to build the project.
4. Click on ``Configure`` and select your compiler/generator.
5. Set the variables you want to change.
6. Click on ``Generate``.

Once you have generated the project, you can open the project file with your IDE (this should match the compiler/generator you have selected in the GUI) and build the project.

Using VSCode
~~~~~~~~~~~~

If you are using Visual Studio Code, you can use the C/C++ Extension Pack provided by Microsoft. This extension pack includes the CMake Tools extension, which allows you to configure and build the project directly from the IDE.
We provided a set of presets configurations for the project that can be directly used with the CMake Tools extension.
Configure the project manually or using the presets from the extension menu or the bottom action bar. Mainly you will have to specify:
- The project generator (``Ninja``, ``MSVC``, ``XCode``, etc.)
- The project build type (``Debug``, ``Release``, etc.)
- The project test type (``Ctest Debug``, ``Ctest Release``, etc.)

.. note::

   Only a minimal set of presets are provided so far, for ``Ninja``, ``Makefile``, and ``MSVC``. Do not hesitate to create your own presets if needed or to provide us with new presets.
   They can be found in the ``./cmake/presets`` directory of the project.

.. warning::

   Tests cannot properly run in ``Release`` mode. If you want to run the tests, you should use the ``Debug`` mode.


Configuration options
~~~~~~~~~~~~~~~~~~~~~

The following options are available to configure the build:

- :BUILD_TESTS: Enable the build of the tests. Default is ``OFF``.
- :BUILD_CUDA_BACKEND: Enable the build of the CUDA backend. Default is ``OFF``.
- :BUILD_OPENCL_BACKEND: Enable the build of the OpenCL backend. Default is ``ON``.
- :BUILD_DOCUMENTATION: Enable the build of the documentation. Default is ``ON`` (``sphinx`` and ``doxygen`` required).
- :BUILD_SHARED_LIBS: Build the library as a shared library. Default is ``ON``.
- :BUILD_COVERAGE: Enable the build of the coverage report. Default is ``OFF``.
- :BUILD_BENCHMARKS: Enable the build of the benchmarks. Default is ``OFF`` (Work in Progress).
- :CMAKE_BUILD_TYPE: Specify the build type. Possible values are ``Debug``, ``Release``, ``RelWithDebInfo``, ``MinSizeRel``.
- :CMAKE_INSTALL_PREFIX: Specify the installation directory. Default is ``/usr/local``.
