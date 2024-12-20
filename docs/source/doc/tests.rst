Running tests
=============

Simply building the library does not necessarily guarantee its correct functionality. Tests provided with the library must be run to ensure that the library is working as expected.

It's essential to set the option ``-D BUILD_TESTS=ON`` during configuration (it should be set by default if you followed the instructions in the `Compilation <compilation>`__ section).
This step is highly recommended during development to verify the library's functionality after each modification.

To run all the tests, you can use the following command in the build directory:

.. code-block:: bash

   ctest --test-dir ./build -C Debug -V

The ``--test-dir`` flag specifies the directory where the build is located. The ``-C`` flag specifies the configuration to use.
The ``-V`` flag indicates that the tests should be run in verbose mode.

.. warning::

  If built traditionally, the root directory of the build should be ``./build/{config_type_build}/``, where ``{config_type_build}`` depends on the system and configuration used, e.g., ``./build/linux-ninja-multi/`` for Ubuntu systems.

.. note::

  It is also possible to run a particular test using the ``ctest`` command: ``ctest --test-dir ./build -C Debug -R {test_name}``.

If using VSCode or any other IDE, it is also possible to run the tests directly from it. Please refer to the respective software documentation for instructions on how to run the tests with CMake and the IDE.
