Compilation
===========

This library is a backend for the different language API libraries available for users:
  * [pyclesperanto]()
  * [clEsperantoJ]()
  * [ClEsperanto]()

The library can be installed on your system but it is more adapted to use the language specific API libraries for actual use.
We will conver here the compilation of the library from the source code in order to contribute to the development of the library.

Prerequisites
-------------

The following software is required to compile the library:
  * [Git](https://git-scm.com/downloads).
  * [CMake](https://cmake.org/download/) (version 3.20 or higher).
  * [OpenCL]() and device drivers. See installation [instructions]().
  * [Python](https://www.python.org/downloads/).
  * C++ Compiler:
      * On Windows, you can install it with [MSVC](https://visualstudio.microsoft.com/).
      * On MacOS, you can install the [Xcode commande line tools](https://developer.apple.com/xcode/resources/).
      * On Linux, your can install GCC with [build-essential](https://packages.ubuntu.com/jammy/build-essential) package.

.. note::
  
  Make sure that the following are available in your system $PATH variable. Especially Git and CMake.


Build
-----

The library can be build using terminal command line. It is also possible to use an IDE like Visual Code but we will not cover this here as there is too many different IDEs to cover.

.. note:: 

  For windows user, you will need to use the `x64 Native Tools Command Prompt` from MSVC to build the library.

First, clone the repository from GitHub:

.. code-block:: bash

  git clone https:://github.com/clEsperanto/CLIc.git
  cd CLIc

Then, use `cmake` to configure the build.

.. code-block:: bash

  cmake --build ./build --source . -G "Ninja" -D BUILD_TESTS=ON -D CMAKE_BUILD_TYPE=Debug -D BUILD_CUDA_BACKEND=OFF -D BUILD_OPENCL_BACKEND=ON
  

The `cmake` command will generate the build files for the library. The `--build` and `--source` flags are used to specify the build and source directories. The `-G` flag is used to specify the generator to use. 
The `-D` flags are used to pass options to the `cmake` command. In this case, we are specifying that we want to build the tests, that we want to build the library in debug mode and that we want to build the OpenCL backend.

.. note:: 

  It is also possible to use cmake with a GUI to configure the build. This is especially useful for Windows users.

.. note:: 

  Some IDE integrate cmake and can be used to configure the build directly. e.g. Visual Code.

Once configure, we can build the library using the following command:

.. code-block:: bash

  cmake --build ./build --config Debug --parallel 8

The `--config` flag is used to specify the configuration to build if multiple are available. The `--parallel` flag is used to specify the number of parallel build to use.


Tests
-----

Building the library does not necessaryly means that the library is working correctly. The best way to ensure that the library is working correctly is to run the tests if the option `-D BUILD_TESTS=ON` was used at configure time.
This step will also be useful to ensure that any contribution to the library is not breaking it.

To run all the tests, one can use the following command:

.. code-block:: bash

  ctest --test-dir ./build -C Debug -V

The `--test-dir` flag is used to specify the directory where the build is. The `-C` flag is used to specify the configuration to use. The `-V` flag is used to specify that the tests should be run in verbose mode.

.. warning:: 

  If build classically, the root directory of the build should be `./build/{config_type_build}/` where `{config_type_build}` is depending on the system and configuration used. e.g. `./build/linux-ninja-multi/` for Ubunut system.

.. note::

  It is also possible to run a particular test using the `ctest` command `ctest --test-dir ./build -C Debug -R {test_name}`.
