How to Contribute
=================

The library is organized into tiers. Each tier consists of functions that rely on one or more functions from the previous tier. 
``Tier1`` is the lowest, primarily composed of basic operations in pure ``OpenCL`` code. 
``Tier2`` relies on one or more calls to Tier1 functions. ``Tier3`` relies on one or more calls to Tier2 functions, and so forth. 
``Tier0`` is a special tier containing functions that span multiple tiers and are used by all others for managing specific operations such as output management.

Add a New Function
-------------------

Adding a new function to the library involves several steps. To add a new function, define it in a header file (``.hpp``) and instantiate it in a source file (``.cpp``), located respectively in the ``include`` and ``src`` directories of the ``clic`` folder.

Function Definition
~~~~~~~~~~~~~~~~~~~

The definition holds the signature of the function. In the correct tier's header file, add the function's signature.

.. code-block:: cpp

    // clic/include/tier2.hpp

    auto my_operation_func(const Device::Pointer &device, const Array::Pointer &src, Array::Pointer &dst, float param1, int param2) -> Array::Pointer;

The function must follow several rules. First, the function name should be suffixed with ``_func`` and use the `trailing return <https://en.wikipedia.org/wiki/Trailing_return_type>__` syntax. 
The first parameter should always be a ``const Device::Pointer& device``, followed by all the inputs and outputs required by the function. 
We require that the inputs are passed as ``const Array::Pointer&`` and named ``src``. The output must be an ``Array::Pointer&`` and named ``dst``. 
If multiple inputs or outputs are needed, an index is added to the name, e.g., ``src0``, ``src1``, ``dst0``, ``dst1``. 
The function should return a type corresponding to the ``dst`` parameter, usually ``Array::Pointer``. 
Finally, we can add other parameters required by the function as native types. 
We do not use default values for parameters as this code is not intended to be used directly by the user. 
Default values should be set in the documentation block of the function.

The function definition must also come with a Doxygen documentation block. 
This documentation block should be placed just before the function definition and should respect the following format to be used in the code autogeneration process later on.

.. code-block:: cpp

    // clic/include/tier2.hpp

    /**
     * @name my_operation
     * @brief This function does something.
     *     The brief can span several lines if needed.
     *     
     * @param device Device to perform the operation on. [const Device::Pointer &]
     * @param src The input array. [const Array::Pointer &]
     * @param dst The output array. [Array::Pointer ( = None )]
     * @param param1 The first parameter. [float ( = 0 )]
     * @param param2 The second parameter. [int ( = 1 )]
     * @return Array::Pointer
     *
     * @note 'category1', 'category2'   
     * @see https://reference_to_the_function_documentation_or_other_link_1
     * @see https://reference_to_the_function_documentation_or_other_link_2
     */
    auto my_operation_func(const Device::Pointer &device, const Array::Pointer &src, Array::Pointer &dst, float param1, int param2) -> Array::Pointer;

``@name`` is the name of the operation minus the ``_func`` suffix. 
``@brief`` is a short description of the function. 
We will then add a ``@param`` tag for each parameter of the function. 
The ``@return`` tag is used to specify the return type of the function. 
The ``@param`` tag should be specified as follows: ``@param {name_of_the_parameter} Description of the parameter [{type_of_the_parameter} ( = {default_value} )]``. 
The ``@note`` and ``@see`` tags are optional. 
The ``@note`` tag is used to pass additional information for later, mainly used to pass ``categories`` used to sort the functions in menus, inferences, or other similar operations. 
The ``@see`` tag is used to add links and references to the documentation. Multiple links can be added.

.. note:: 

    The order of the tags impacts the documentation appearance. We advise keeping the same order as in the example: ``@name``, ``@brief``, ``@param``, ``@return``, ``@note``, and finally ``@see``.

.. warning:: 

    If wrongly or undocumented, a function will either fail to be added to the library or will have undefined behavior when used in the library API.

.. warning:: 

    Auto-format tools will add line returns to the documentation block. This can be a problem for the autogeneration process. Until better integration, we advise keeping the documentation line length to a minimum. This is particularly important for `@param`, `@see`, and `@note` tags.


Function Instantiation
~~~~~~~~~~~~~~~~~~~~~~

Once defined and documented, we can proceed to the corresponding source file to instantiate the function.

.. code-block:: cpp

    // clic/src/tier2.cpp

    auto my_operation_func(const Device::Pointer &device, const Array::Pointer &src, Array::Pointer &dst, float param1, int param2) -> Array::Pointer
    {
        // Implementation of the function
    }

The first step in the function implementation is managing the return value. In `CLIc`, if not provided by the user, the functions are responsible for managing the output array creation and allocation. We can rely on a set of `tier0` functions which will create and allocate the output array `dst`. These functions test the existence of a `dst` array, and if not provided, will allocate one. The most common case is to use the `tier0::create_like()` function. This function utilizes the information from `src` (size, dimension, etc.) to create an array of the same size as `src`. Optionally, we can specify a `dType` parameter if the function is supposed to return an array of a specific type. The default behavior is to propagate the `src` data type to the `dst` array.

.. code-block:: cpp

    // clic/src/tier2.cpp

    auto my_operation_func(const Device::Pointer &device, const Array::Pointer &src, Array::Pointer &dst, float param1, int param2) -> Array::Pointer
    {
        tier0::create_like(dst, src, dType::FLOAT);
        // Implementation of the function
    } 

.. note:: 

    Several output creation functions already exist. See `tier0.hpp` for more information.

.. note:: 

    The current convention is labels Array are supposed to use the ``dType::LABEL`` and binary Array the ``dType::BINARY`` type.

The rest of the code should correspond to the algorithm of the function. It is highly advised to rely on pre-existing functions from previous tiers to avoid code duplication and ensure the consistency of the library. We recommend examining other functions to see how they are implemented and using them as a template for your own function, especially for similar operations.

Relying on Tier Functions
~~~~~~~~~~~~~~~~~~~~~~~~~

Once the shell of the function is implemented, with the return variable managed, we can proceed to implement the function itself. 
We simply use C++ code to implement the function and rely on already existing functions from previous tiers to perform the operations.

An easy example would be the ``difference_of_gaussian_func`` in ``tier2``, which relies on the ``gaussian_blur_func`` and ``add_weighted_images_func`` functions from ``tier1`` to perform the operation.

.. code-block:: cpp

    // clic/src/tier2.cpp

    auto difference_of_gaussian_func(const Device::Pointer& device,
                                     const Array::Pointer& src,
                                     Array::Pointer dst,
                                     float sigma1_x,
                                     float sigma1_y,
                                     float sigma1_z,
                                     float sigma2_x,
                                     float sigma2_y,
                                     float sigma2_z) -> Array::Pointer
    {
        tier0::create_like(src, dst, dType::FLOAT);
        auto gauss1 = tier1::gaussian_blur_func(device, src, nullptr, sigma1_x, sigma1_y, sigma1_z);
        auto gauss2 = tier1::gaussian_blur_func(device, src, nullptr, sigma2_x, sigma2_y, sigma2_z);
        return tier1::add_images_weighted_func(device, gauss1, gauss2, dst, 1, -1);
    }

The ``gaussian_blur_func`` computes two temporary Arrays ``gauss1`` and ``gauss2`` on the device. 
The ``add_images_weighted_func`` then applies the difference between the two Gaussians and stores the result in ``dst``, as well as returning it.

A more advanced function implementation could be the ``extend_labeling_via_voronoi_func`` also in ``tier2``, which also relies on pre-existing functions but implements them in a loop.

.. code-block:: cpp

    // clic/src/tier2.cpp

    auto extend_labeling_via_voronoi_func(const Device::Pointer& device,
                                          const Array::Pointer& src,
                                          Array::Pointer dst) -> Array::Pointer
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

This function is a good example of how to create temporary Arrays in a memory-efficient way. 
The ``flip`` and ``flop`` Arrays are created using the ``Array::create()`` function, which creates an Array of the same size and type as the ``dst`` Array. 
We then alternate the Arrays depending on the iteration count, hence the Arrays' names ``flip`` and ``flop``.

Call a Kernel file
------------------

In the previous examples, we haven't directly called a GPU kernel, yet we've managed to fully accelerate a ``difference of Gaussians`` operation on the GPU. This is mainly because we relied on blocks of the algorithm already implemented on the GPU, such as `gaussian_blur_func` and `add_images_weighted_func` from `tier1`. If we inspect their implementation, we can see that they don't contain algorithmic code but rather calls for GPU kernel execution.

.. code-block:: cpp

    // clic/src/tier1.cpp

    #include "cle_add_images_weighted.h

    auto add_images_weighted_func(const Device::Pointer& device,
                                  const Array::Pointer& src0,
                                  const Array::Pointer& src1,
                                  Array::Pointer dst,
                                  float factor0,
                                  float factor1) -> Array::Pointer
    {
        tier0::create_like(src0, dst, dType::FLOAT);
        const KernelInfo kernel = {"add_images_weighted", kernel::add_images_weighted};
        const ParameterList params = {{"src0", src0}, {"src1", src1}, {"dst", dst}, {"scalar0", factor0}, {"scalar1", factor1}};
        const RangeArray range = {dst->width(), dst->height(), dst->depth()};
        execute(device, kernel, params, range);
        return dst;
    }

We maintain the same structure as in the previous examples with the function signature, parameters, and return value management. 
The rest of the function code is dedicated to preparing the GPU code and running the ``execute`` function.
We rely on what is called JIT compilation, or ``Just In Time``. 
This means that the kernels are compiled and run at runtime. 
This is a very powerful feature as it allows writing GPU code in a flexible way, adapted to your data size and time requirements, but it requires a bit of preparation for execution. 
It will also add compilation time to the process, which can be a bit long for the first execution of a kernel but is drastically reduced for subsequent calls due to a caching system.

The first thing to ensure is that the kernel code we will call is available in the `CLIJ kernel repository <https://github.com/clEsperanto/clij-opencl-kernels/tree/clesperanto_kernels>__` and respects the CLIJ convention. If this is the case, we will be able to include the kernel as a header file in the ``clic`` library. This header will contain a stringified version of the kernel code and will be passed to the ``execute`` function as a ``KernelInfo`` structure with the name of the kernel and the code of the kernel. By default, the `KernelInfo` should match the pattern `{ "kernel_name", kernel::kernel_name }`.

.. code-block:: cpp
    :emphasize-lines: 3, 14

    // clic/src/tier1.cpp

    #include "cle_add_images_weighted.h"

    auto add_images_weighted_func(const Device::Pointer& device,
                                   const Array::Pointer& src0,
                                   const Array::Pointer& src1,
                                   Array::Pointer dst,
                                   float factor0,
                                   float factor1) -> Array::Pointer
    {
        tier0::create_like(src0, dst, dType::FLOAT);
        const KernelInfo kernel = {"add_images_weighted", kernel::add_images_weighted};
        const ParameterList params = {
            {"src0", src0}, {"src1", src1}, {"dst", dst}, {"scalar0", factor0}, {"scalar1", factor1}
        };
        const RangeArray range = {dst->width(), dst->height(), dst->depth()};
        execute(device, kernel, params, range);
        return dst;
    }

The next step is to prepare the parameters for the kernel. 
The parameters are passed as a ``ParameterList`` structure with the name of the parameter and the value of the parameter. 
The ``ParameterList`` is a list of parameters defined by a ``tag`` and a ``value``. 
Here, the ``tag`` is the parameter name defined in the kernel code, and the ``value`` is an ``Array::Pointer`` or a native type. 
The order of the parameters is important and should match the order of the parameters in the kernel code.

.. code-block:: cpp
    :emphasize-lines: 15, 16, 17

    // clic/src/tier1.cpp

    #include "cle_add_images_weighted.h"

    auto add_images_weighted_func(const Device::Pointer& device,
                                   const Array::Pointer& src0,
                                   const Array::Pointer& src1,
                                   Array::Pointer dst,
                                   float factor0,
                                   float factor1) -> Array::Pointer
    {
        tier0::create_like(src0, dst, dType::FLOAT);
        const KernelInfo kernel = {"add_images_weighted", kernel::add_images_weighted};
        const ParameterList params = {
            {"src0", src0}, {"src1", src1}, {"dst", dst}, {"scalar0", factor0}, {"scalar1", factor1}
        };
        const RangeArray range = {dst->width(), dst->height(), dst->depth()};
        execute(device, kernel, params, range);
        return dst;
    }

The last step is to prepare the range of the kernel execution. For that, we need to define a range of processing. Here, the range is the computational dimension of the kernel. 
By default, it is the dimension of the output memory, but it can be changed and must be optimized for the computation. 
Once the ``KernelInfo``, ``ParameterList``, and ``RangeArray`` are prepared, we can call the ``execute`` function. 
This function will take care of the kernel compilation and execution. 
The output of the computation should be stored as one of the parameters of the ``ParameterList``. 
In the majority of the cases, the output will be the ``dst`` Array.

.. code-block:: cpp
    :emphasize-lines: 18, 19, 20

    // clic/src/tier1.cpp

    #include "cle_add_images_weighted.h"

    auto add_images_weighted_func(const Device::Pointer& device,
                                  const Array::Pointer& src0,
                                  const Array::Pointer& src1,
                                  Array::Pointer dst,
                                  float factor0,
                                  float factor1) -> Array::Pointer
    {
        tier0::create_like(src0, dst, dType::FLOAT);
        const KernelInfo kernel = {"add_images_weighted", kernel::add_images_weighted};
        const ParameterList params = {
            {"src0", src0}, {"src1", src1}, {"dst", dst}, {"scalar0", factor0}, {"scalar1", factor1}
        };
        const RangeArray range = {dst->width(), dst->height(), dst->depth()};
        execute(device, kernel, params, range);
        return dst;
    }

.. note:: 

    The `RangeArray` has a strong impact on the performance of the kernel.


Add Function Tests
------------------

The final step is to add tests for the function. The tests are located in the ``tests`` directory at the root of the repository. They are organized in the same way as the library, in tiers. The tests for the function should be added in the correct tier folder.

Tests are written in ``cpp`` and utilize the Google Test framework. Their objective is to ensure that both the kernel and the functions work correctly in the library and that the output is as expected.

The test file should be located in the appropriate tier and named as ``test_{function_name}.cpp``. It should include the ``gtest/gtest.h`` header and the ``cle.hpp`` header. We recommend copying an existing test file and adapting it to the new function.

After adding a test, it may be necessary to reconfigure and rebuild the library for CMake to incorporate the new tests. Tests can be executed using the ``ctest`` command. Additionally, the CI/CD pipeline runs tests on each pull request.

.. note:: 

    To run a specific test, use the ``ctest -C Debug -R {test_name}`` command.
