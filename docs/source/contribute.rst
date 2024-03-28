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
     * @note 'category1', 'category2'   
     * @see https://reference_to_the_function_documentation_or_other_link_1
     * @see https://reference_to_the_function_documentation_or_other_link_2
     */
    auto
    my_operation_func(const Device::Pointer &device, const Array::Pointer &src, Array::Pointer &dst, float param1, int param2) -> Array::Pointer;

`name` is the name of the operation minus the `_func` suffix. `brief` is a short description of the function. We will then add a `param` tag for each parameter of the function. The `return` tag is used to specify the return type of the function.
The `param` tag should be specified as follows: `@param {name_of_the_parameter} Description oftheparameter [{type_of_the_parameter} ( = {default_value} )]`. The tags `note` and `see` are optional. 
The `note` tag is used to pass additionnal information for later. Mainly used to pass `categories` used to sort the functions in menus, inferences, or other similar operations. The `see` tag is used to add links and reference to the documentation. Multiple links can be added.  

.. note:: 

    The order of the tags as an impact on the documentation looks. We advise to keep the same order as in the example:
    `@name`, `@brief`, `@param`, `@return`, `@snote`, and finally `@see`

.. warning:: 

    If wrongly or undocumented, a function will either fail to be added to the library or will have undefined behaviour when used in the library API.

.. warning:: 

    Auto-format tools will add line-return to the documentation block. This can be a problem for the autogeneration process. Until better integration, we advise to keep the documentation line length to a minimum.
    This is particularly important for `@param`, `@see`, and `@note` tags.

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
We can also optionally specify a `dType` as parameter if the function is suposed to return an array of a specific type. The default behaviour is to propagate the `src` data type to the `dst` array.

.. code-block:: cpp

    // clic/src/tier2.cpp

    auto
    my_operation_func(const Device::Pointer &device, const Array::Pointer &src, Array::Pointer &dst, float param1, int param2) -> Array::Pointer
    {
        tier0::create_like(dst, src, dType::FLOAT);
        // Implementation of the function
    } 


.. note:: 

    Several output creation functions are already existing. See `tier0.hpp` for more.

.. note:: 

     Current convention is labels Array are supposed to use the `dType::LABEL` and binary Array the `dType::BINARY` type.

The reste of the code should correspond to the algorithm of the function. It is of course highly advised to rely on pre-existing functions from previous tiers to avoid code duplication and to ensure the consistency of the library.
We advise to go have a look at other functions to see how they are implemented and to use them as a template for your own function, especially for similar operations.


Use tiers function
==================

Once the shell of the function is implemented, with the return variable managed, we can start to implement the function itself. We simply use C++ code to implement the function and rely on already existing functions from previous tiers to perform the operations.

An easy example would be the tier2 function `difference_of_gaussian_func` which relies on the tier1 function `gaussian_blur_func` and `add_weighted_images_func` to perform the operation. 

.. code-block:: cpp

    // clic/src/tier2.cpp

    auto
    difference_of_gaussian_func(const Device::Pointer & device,
                                const Array::Pointer &  src,
                                Array::Pointer          dst,
                                float                   sigma1_x,
                                float                   sigma1_y,
                                float                   sigma1_z,
                                float                   sigma2_x,
                                float                   sigma2_y,
                                float                   sigma2_z) -> Array::Pointer
    {
    tier0::create_like(src, dst, dType::FLOAT);
    auto gauss1 = tier1::gaussian_blur_func(device, src, nullptr, sigma1_x, sigma1_y, sigma1_z);
    auto gauss2 = tier1::gaussian_blur_func(device, src, nullptr, sigma2_x, sigma2_y, sigma2_z);
    return tier1::add_images_weighted_func(device, gauss1, gauss2, dst, 1, -1);
    }

The `gaussian_blur_func` will compute two temporary Arrays `gauss1` and `gauss2` on the device. The `add_images_weighted_func` will then apply the difference between the two gaussians and store the result in `dst`, as well as return it.

A more advance function implementation could be the `extend_labeling_via_voronoi_func` also in `tier2` which relies also on pre-exisitng function but implement them into a loop.

.. code-block:: cpp

    // clic/src/tier2.cpp

    auto
    extend_labeling_via_voronoi_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
    -> Array::Pointer
    {
    tier0::create_like(src, dst, dType::UINT32);
    auto flip = Array::create(dst);
    auto flop = Array::create(dst);
    tier1::copy_func(device, src, flip);

    auto flag = Array::create(1, 1, 1, 1, dType::INT32, mType::BUFFER, device);
    flag->fill(0);
    int flag_value = 1;
    int iteration_count = 0;
    while (flag_value > 0)
    {
        if (iteration_count % 2 == 0)
        {
        tier1::onlyzero_overwrite_maximum_box_func(device, flip, flag, flop);
        }
        else
        {
        tier1::onlyzero_overwrite_maximum_box_func(device, flop, flag, flip);
        }
        flag->read(&flag_value);
        flag->fill(0);
        iteration_count++;
    }
    if (iteration_count % 2 == 0)
    {
        flip->copy(dst);
    }
    else
    {
        flop->copy(dst);
    }
    return dst;
    }

This function is a good example on how to create temporary Arrays in a memory efficient way. The `flip` and `flop` Arrays are created using the `Array::create()` function which will create an Array of the same size and type as the `dst` Array.
We will then alternate the Arrays depending on the iteration count. Hence the Arrays name `flip` and `flop`.


Execute a kernel
================

We can see in the previous examples that we have not yet directly called a GPU kernel and yet we have managed to implement a `difference of gaussians` fully accelerated in GPU. This is mainly because we relied on blocks of the algorithm already implemented in GPU.
In this case, the `gaussian_blur_func` and `add_images_weighted_func` from the `tier1`. If we go to their implementation, we can see that their implementation is not algorithmic but rather a call for a GPU kernel execution.

.. code-block:: cpp

    // clic/src/tier1.cpp

    #include "cle_add_images_weighted.h

    auto
    add_images_weighted_func(const Device::Pointer & device,
                            const Array::Pointer &  src0,
                            const Array::Pointer &  src1,
                            Array::Pointer          dst,
                            float                   factor0,
                            float                   factor1) -> Array::Pointer
    {
    tier0::create_like(src0, dst, dType::FLOAT);
    const KernelInfo    kernel = { "add_images_weighted", kernel::add_images_weighted };
    const ParameterList params = {
        { "src0", src0 }, { "src1", src1 }, { "dst", dst }, { "scalar0", factor0 }, { "scalar1", factor1 }
    };
    const RangeArray range = { dst->width(), dst->height(), dst->depth() };
    execute(device, kernel, params, range);
    return dst;
    }

We keep the same structure as the previous examples with the function signature, parameters, and return value management. The reste of the function code is dedicated in preparing GPU code and run the `execute` function.
We are relying on what is called a JIT compilation, for `Just In Time`. This means that the kernels are compiled and run at runtime. This is a very powerful feature as it allows to write GPU code in a very flexible way, adapted to your data size and Time
but requires a bit of preparation for execution. It will also add up a compilation time to the process which can be a bit long for the first execution of a kernel but dratisclly reduced for the next calls due to a cache system.

The first thing to ensure is that the kernel code we will call is available in the [CLIJ kernel repository]() and respect the CLIJ convention. If this is the case, we will be able to include the kernel as a header file 
in the `clic` library. This header will contained a stringified version of the kernel code and will be pass to the execute function as a `KernelInfo` structure with the name of the kernel and the code of the kernel. By default, the `KernelInfo` should math the pattern `{ "kernel_name", kernel::kernel_name }`.

.. code-block:: cpp
    :emphasize-lines: 3, 14

    // clic/src/tier1.cpp

    #include "cle_add_images_weighted.h

    auto
    add_images_weighted_func(const Device::Pointer & device,
                            const Array::Pointer &  src0,
                            const Array::Pointer &  src1,
                            Array::Pointer          dst,
                            float                   factor0,
                            float                   factor1) -> Array::Pointer
    {
    tier0::create_like(src0, dst, dType::FLOAT);
    const KernelInfo    kernel = { "add_images_weighted", kernel::add_images_weighted };
    const ParameterList params = {
        { "src0", src0 }, { "src1", src1 }, { "dst", dst }, { "scalar0", factor0 }, { "scalar1", factor1 }
    };
    const RangeArray range = { dst->width(), dst->height(), dst->depth() };
    execute(device, kernel, params, range);
    return dst;
    }


The next step is to prepare the parameters for the kernel. The parameters are passed as a `ParameterList` structure with the name of the parameter and the value of the parameter. The `ParameterList` is a list of parameters defined by a `tag` and a `value`.
Here the `tag` is the parameter name defined in the kernel code and the `value` is an `Array::Pointer` or a native type. The order of the parameters is important and should match the order of the parameters in the kernel code.

.. code-block:: cpp
    :emphasize-lines: 15, 16, 17

    // clic/src/tier1.cpp

    #include "cle_add_images_weighted.h

    auto
    add_images_weighted_func(const Device::Pointer & device,
                            const Array::Pointer &  src0,
                            const Array::Pointer &  src1,
                            Array::Pointer          dst,
                            float                   factor0,
                            float                   factor1) -> Array::Pointer
    {
    tier0::create_like(src0, dst, dType::FLOAT);
    const KernelInfo    kernel = { "add_images_weighted", kernel::add_images_weighted };
    const ParameterList params = {
        { "src0", src0 }, { "src1", src1 }, { "dst", dst }, { "scalar0", factor0 }, { "scalar1", factor1 }
    };
    const RangeArray range = { dst->width(), dst->height(), dst->depth() };
    execute(device, kernel, params, range);
    return dst;
    }


The last step is to prepare the range of the kernel execution. For that we need to define a range of processing. Here the range is the computational dimension of the kernel. By default it is the dimension of the output memory but it can be changed and must be optimised for the computation.
Once the `KernelInfo`, `ParameterList`, and `RangeArray` are prepared, we can call the `execute` function. This function will take care of the kernel compilation and execution. The output of the computation should be stored as one of the parameter of the `ParameterList`.
In the majority of the cases, the output will be the `dst` Array.

.. code-block:: cpp
    :emphasize-lines: 18, 19, 20

    // clic/src/tier1.cpp

    #include "cle_add_images_weighted.h

    auto
    add_images_weighted_func(const Device::Pointer & device,
                            const Array::Pointer &  src0,
                            const Array::Pointer &  src1,
                            Array::Pointer          dst,
                            float                   factor0,
                            float                   factor1) -> Array::Pointer
    {
    tier0::create_like(src0, dst, dType::FLOAT);
    const KernelInfo    kernel = { "add_images_weighted", kernel::add_images_weighted };
    const ParameterList params = {
        { "src0", src0 }, { "src1", src1 }, { "dst", dst }, { "scalar0", factor0 }, { "scalar1", factor1 }
    };
    const RangeArray range = { dst->width(), dst->height(), dst->depth() };
    execute(device, kernel, params, range);
    return dst;
    }


.. note:: 

    The `RangeArray` as a strong impact on the performance of the kernel.

Add function tests
==================

The last step is to add tests for the function. The tests are located in the `tests` directory at the root of the repository. The tests are organised in the same way as the library, in tiers, and the tests for the function should be added in the correct tier folder.
The tests are written in `cpp` and use the google test framework. The test has the objective to make sure that both the kernel and the functions correctly work in the library and that the output is as expected.

The tests file should be located in the correct tiers, and named as `test_{function_name}.cpp`. The test file should include the `gtest/gtest.h` header and the `cle.hpp` header. We advise to copy an already existing test file and to adapt it to the new function.

Once a test is added, it may be necessary to reconfigure and rebuild the library for cmake to take into account the new tests. The tests can be run using the `ctest` command. The tests are also run on the CI/CD pipeline and will be run on each pull request.

.. note:: 

    It is possible to run a specific test using the `ctest -C Debug -R {test_name}` command.  