How to contribute
*****************

The library is organised in tiers. Each tier is a set of functions that rely on one or more function from the previous tier. The `tier1` is the lowest, mainly composed of basic operations in pure `OpenCL` code.
The `tier2` will rely on one or more call of a tier1 function. The `tier3` will rely on one or more call of a tier2 function, and so on. The `tier0` is a special tier that contains functions that are cross-tiers and
are used by all the other tiers for managing specific operations such as output management.


Add a new function
==================

Adding a new function to the library is a process that involves several steps. In order to add a new function to the library, you need to define it in a header file (.hpp) and instanciate it in a source file (.cpp) 
respectively located in the `include` and `src` directories of the `clic` folder.

function definition
-------------------

The definition holds the signature of the function. In the correct tiers header file, you can add the signature of the function.

.. code-block:: cpp

    // clic/include/tier2.hpp

    auto
    my_operation_func(const Device::Pointer &device, const Array::Pointer &src, Array::Pointer &dst, float param1, int param2) -> Array::Pointer;

The function must follow several set of rules. First, the function name should be sufixed with `_func`, and use the [trailling return](https://en.wikipedia.org/wiki/Trailing_return_type) syntax. The first parameter should always 
be a `const Device::Pointer& device` and followed by all the inputs and the outputs required by the function. We require that the inputs are passed as `const Array::Pointer&` and named as `src`. The output must be a `Array::Pointer&` and named `dst`. 
If multiple inputs or outputs are needed, an index is added to the name, e.g. `src0`, `src1`, `dst0`, `dst1`. The function should return a type which should correspond to the `dst` parameter, usually `Array::Pointer`. Finally, we can add the other 
parameters required by the function as native type. We do not uses default values for parameters as this code is not intended to be used directly by the user. The default values should be set in the documentation block of the function.

The function definition must also comes with a doxygen documentation block. This documentation blocks should be placed just before the function definition and should respect the following format inorder to be used in the code autogeneration process 
later on.

.. code-block:: cpp

    // clic/include/tier2.hpp

    /**
     * @name my_operation
     * @brief This function does something
     *     And the brief can be on several lines
     *     if needed
     *     
     * @param device Device to perform the operation on. [const Device::Pointer &]
     * @param src The input array [const Array::Pointer &]
     * @param dst The output array [Array::Pointer ( = None )]
     * @param param1 The first parameter [float ( = 0 )]
     * @param param2 The second parameter [int ( = 1 )]
     * @return Array::Pointer
     *
     * @note category 'cat1', 'cat2'   
     * @see https://reference_to_the_function_documentation_or_other_link_1
     * @see https://reference_to_the_function_documentation_or_other_link_2
     */
    auto
    my_operation_func(const Device::Pointer &device, const Array::Pointer &src, Array::Pointer &dst, float param1, int param2) -> Array::Pointer;

`name` is the name of the operation minus the `_func` suffix. `brief` is a short description of the function. We will then add a `param` tag for each parameter of the function. The `return` tag is used to specify the return type of the function.
The `param` tag should be specified as follows: `@param {name_of_the_parameter} Description oftheparameter [{type_of_the_parameter} ( = {default_value} )]`. The tags `note` and `see` are optional. 
The `note` tag is used to pass additionnal information for later. Mainly used to pass `categories` used to sort the functions in menus, inferences, or other similar operations. The `see` tag is used to add links and reference to the documentation. Multiple links can be added.  

.. warning:: 

    If wrongly or not documented, a function will either fail to be added to the library or will have undefined behaviour when used in the library API.

function instanciation
----------------------

Once defined and documented, we can go to the correcponding source file to instanciate the function.

.. code-block:: cpp

    // clic/src/tier2.cpp
    auto
    my_operation_func(const Device::Pointer &device, const Array::Pointer &src, Array::Pointer &dst, float param1, int param2) -> Array::Pointer
    {
        // Implementation of the function
    }    

 First thing to do in the function implementation is the management of the return value. In `CLIc`, if not provided by user, the functions are in charge of managing the output array creation and allocation.
 We can rely on a set of `tier0` function which will create and allocate the output array `dst`. These function test the existance of a `dst` array, and if not provided, will allocate one. 
 The most classic case is to use the `tier0::create_like()` function. The function uses the information from `src` (size, dimension, etc) to create an array of the same size as `src`. 
 We can also optionally specify a `dtype` as parameter if the function is suposed to return an array of a specific type.

.. code-block:: cpp

    // clic/src/tier2.cpp
    auto
    my_operation_func(const Device::Pointer &device, const Array::Pointer &src, Array::Pointer &dst, float param1, int param2) -> Array::Pointer
    {
        tier0::create_like(dst, src, dtype::FLOAT);
        // Implementation of the function
    } 

.. note:: 

    Several output creation functions are already existing. See `tier0.hpp` for more.

.. note:: 

    The default behaviour is to propagate the `src` data type in the `dst` array. Otherwise, Labels array are supposed to use the `dtype::LABEL` and binary arrays the `dtype::BINARY` type.

The reste of the code should correspond to the algorithm of the function. It is of course highly advised to rely on pre-existing functions from previous tiers to avoid code duplication and to ensure the consistency of the library.
We advise to go have a look at other functions to see how they are implemented and to use them as a template for your own function, especially for similar operations.


Call a tier function
====================

ToDo


Call a kernel
=============

Only needed if the function rely on an OpenCL CLIJ kernel.


function tests
--------------

The last step is to add tests for the function. The tests are located in the `tests` directory at the root of the repository. The tests are organised in the same way as the library, in tiers, and the tests for the function should be added in the correct tier folder.
The tests are written in `cpp` and use the google test framework. 

Once a test is added, it may be necessary to reconfigure and rebuild the library for cmake to take into account the new tests. The tests can be run using the `ctest` command.