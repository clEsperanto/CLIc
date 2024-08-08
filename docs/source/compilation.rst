Compilation
-----------

in Terminal
~~~~~~~~~~~

We rely on CMake to build the library.
CMake is a cross-platform build system that can generate build files for different build systems like ``make``, ``ninja``, ``Visual Studio``, ``Xcode``, etc.
We encourage you to refer to your favorite IDE documentation to learn how to use CMake with it.
This documentation will cover how to build the library using the command line and ``ninja`` as the build system.
Please don't hesitate to contact us if you encounter any issues during the build process.

.. note::

  For Windows users, you will need to use the ``x64 Native Tools Command Prompt`` from MSVC to build the library.

First, clone the repository from GitHub:

.. code-block:: bash

   git clone https://github.com/clEsperanto/CLIc.git
   cd CLIc

Then, use ``cmake`` to configure the build:

.. code-block:: bash

   cmake --build ./build --source . -G "Ninja" -D BUILD_TESTS=ON -D CMAKE_BUILD_TYPE=Debug -D BUILD_CUDA_BACKEND=OFF -D BUILD_OPENCL_BACKEND=ON


The ``cmake`` command will generate the build files for the library. The ``--build`` and ``--source`` flags are used to specify the build and source directories.
The ``-G`` flag is used to specify the generator to use. The ``-D`` flags are used to pass options to the ``cmake`` command. In this case, we are specifying that we want to build the tests, that we want to build the library in debug mode, and that we want to build the OpenCL backend.

Once configured, we can build the library using the following command:

.. code-block:: bash

   cmake --build ./build --config Debug --parallel 8

The ``--config`` flag is used to specify the configuration to build if multiple are available.
The ``--parallel`` flag is used to specify the number of parallel builds to use.

Once the build is done, you can run the tests if they were built.

in VScode
~~~~~~~~~

To build the library using VScode, which we advise if you are planning to contribute to the library and you are not familiar with other IDEs or Cpp projects in general, you can follow the steps below:

- Install the requirements mentioned in the `Requirements <requirements>`__ section.
- Install VScode from `here <https://code.visualstudio.com/>`__ and the extension ``C/C++ Extension Pack`` by Microsoft
- Open the repository folder in VScode.
- Select the CMake presets configuration from the bottom action bar:
  
  - Project Generator: ``Ninja``, ``Makefiles``, ``Visual Studio``, etc.
  - Project Build Type: ``Debug``, ``Release``, etc.
  - Project Test type: ``Ctest Debug``, ``Ctest Release``, etc.

- You can now configure the project by calling the ``CMake: Configure`` command from the command palette (``Ctrl+Shift+P``).
- Once configured, you can build the project by calling the ``CMake: Build`` command from the command palette (``Ctrl+Shift+P``).

.. note::

  You can also refere to this issue for more information: `Issue #320 <https://github.com/clEsperanto/CLIc/issues/320#issuecomment-2225234326>`__
    

Configuration options
~~~~~~~~~~~~~~~~~~~~~

The following options can be passed to the ``cmake`` command to configure the build:

- :BUILD_TESTS: Enable the build of the tests. Default is ``OFF``.
- :BUILD_CUDA_BACKEND: Enable the build of the CUDA backend. Default is ``OFF``.
- :BUILD_OPENCL_BACKEND: Enable the build of the OpenCL backend. Default is ``ON``.
- :BUILD_DOCUMENTATION: Enable the build of the documentation. Default is ``ON`` (``sphinx`` and ``doxygen`` required).
- :CMAKE_BUILD_TYPE: Specify the build type. Possible values are ``Debug``, ``Release``, ``RelWithDebInfo``, ``MinSizeRel``.
- :BUILD_SHARED_LIBS: Build the library as a shared library. Default is ``ON``.
- :BUILD_COVERAGE: Enable the build of the coverage report. Default is ``OFF``.
- :BUILD_BENCHMARKS: Enable the build of the benchmarks. Default is ``OFF`` (Work in Progress).
