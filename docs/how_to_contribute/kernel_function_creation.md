# Kernel function creation

All function operation in CLIc are either associated to an OpenCL kernel function (`.cl`) or are composed of a mini-pipeline using CLIc operations. These operations are organised in tiers, defining their complexity. The `tier1` operations are the most basic operations and directly rely on a kernel function from the [clesperanto kernel repository](https://github.com/clEsperanto/clij-opencl-kernels/tree/clesperanto_kernels). The `tier2` operations are other operation which will rely on `tier1` operations, etc. New operations are to be placed in the highest operation tier they rely on plus one (e.g. a new operation relying on `tier2` operations will be a `tier3` operation).

In the following document we will see how to contribute to either:
- a new kernel function based on an OpenCL kernel file
- a new operation based on a mini-pipeline of CLIc operations


## <a id="base">__OpenCL kernel based function__</a>

We will cover here the creation of a new CLIc operation which rely only on an OpenCL kernel function. As it does not rely on any other CLIc operation, it will be a `tier1` operation. 

Here we assume that the OpenCL file is already located in the [clesperanto kernel repository](https://github.com/clEsperanto/clij-opencl-kernels/tree/clesperanto_kernels). And we will here use the `add_image_and_scalar` operation as an example. This operation is defined in the [`add_image_and_scalar.cl`](https://github.com/clEsperanto/clij-opencl-kernels/tree/clesperanto_kernels/kernel/add_image_and_scalar.cl) file.

We first add the function declaration in the [`tier1`]() header file as followed:

```cpp
//--> clic/include/tier1.hpp
auto
add_image_and_scalar_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer;
```
The function name is composed of the kernel name with the suffix `_func` and respect the following convention:
- `device`: the first argument should always be a `const Device::Pointer &` on which the kernel will run on
- `src`: the input source `const Array::Pointer &`
- `dst`: the output destination `Array::Pointer`
- `scalar`: additional arguments required by the algorithm, usually a `float` or an `int`
- `-> Array::Pointer`: the function return type, here an `Array::Pointer` but can also be a `float`, an `int`, or `void` if no return is expected

Notice that the `dst` argument is not a `const Array::Pointer &` as it will be modified by the kernel, which also allows the user to pass a `nullptr` to the function and leave the output memory allocation to the function.

Then we need to add the function definition in the [`tier1`]() source file as followed:
```cpp
//--> clic/src/tier1.cpp
#include "cle_add_image_and_scalar.h"    // <-- include the opencl kernel file

auto
add_image_and_scalar_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
    /* function code */
}
```
the first thing we did is `#include` the header file corresponding to our OpenCL kernel file. 
```cpp
#include "cle_add_image_and_scalar.h"
```
These headers are automatically generated from the list of OpenCL files from the [clesperanto kernel repository](https://github.com/clEsperanto/clij-opencl-kernels/tree/clesperanto_kernels) and make the kernel source available.

Then we define the function as declared in the header file and write the function core. We will usually respect the following steps:
1. create the output array if not provided by the user
2. define the kernel name and source file
3. define the kernel arguments list
4. define the kernel range
5. execute the kernel
6. return the output array

### 1. Create the output array if not provided by the user

Here we can rely on the `tier0` operations which already implement several output managment logics. The most classic being the `create_like` which, if `dst` is `nullptr` will create `dst` as a copy of `src`
```cpp
tier0::create_like(src, dst);
```
Other logic already exist, and more can be added if needed. Alternatively, the output array can be created manually using the `Array::create` function if `dst` is `nullptr`.

### 2. Define the kernel name and source file

We need to provide the kernel function `name` and `source code` as strings in a `KernelInfo` structure. The `name` should match the OpenCL kernel name, here `add_image_and_scalar`, and the source code is provided by the `#include`, which was done at the beginning of the function, as a variable `kernel::add_image_and_scalar`.
```cpp
const KernelInfo kernel = { "add_image_and_scalar", kernel::add_image_and_scalar };
```

### 3. Define the kernel arguments list

The kernel arguments list is defined as a `ParameterList` structure. The `ParameterList` is a `std::vector` that associate a `tag` and a value to each kernel argument. The `tag` should match the argument name in the OpenCL kernel function. The `value` can be either an `Array::Pointer` or a `float` or an `int`. Here we have three arguments: `src`, `dst` and `scalar`.
```cpp
const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
```

### 4. Define the kernel range

The kernel range is the computational space which the device will use to run the various processing threads. The range is defined as a `RangeArray` structure. The `RangeArray` is a `std::array` of `size_t` with three elements: `width`, `height` and `depth`. In most cases, we can use the `dst` dimensions as the kernel range.
```cpp
const RangeArray range = { dst->width(), dst->height(), dst->depth() };
```

__Warning__: The kernel range is not the same as the image dimensions. The kernel range is the computational space which the device will use to run the various processing threads. It as a strong impact on the kernel performance and should be directly define based on the operation.

### 5. Execute the kernel

Once all kernel information are structured, we are ready to pass them to the `execute` function which will take care of building and running the kernel on the device.
```cpp
execute(device, kernel, params, range);
```

### 6. Return the output array

Finally, we return the output array to the user.
```cpp
return dst;
```

making our final function as follow:
```cpp
//--> clic/src/tier1.cpp
#include "cle_add_image_and_scalar.h"    // <-- include the opencl kernel file

auto
add_image_and_scalar_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, float scalar)
  -> Array::Pointer
{
  tier0::create_like(src, dst);
  const KernelInfo    kernel = { "add_image_and_scalar", kernel::add_image_and_scalar };
  const ParameterList params = { { "src", src }, { "dst", dst }, { "scalar", scalar } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}
```

### Function usage

Now that the function is defined, it can be called as follow:
```cpp
auto src = cle::Array::create(/*...*/);
auto dst = cle::tier1::add_image_and_scalar_func(device, src, nullptr, 5);
```
Here we use the `nullptr` to let the function create the output array. Alternatively, the output array can be created manually and passed to the function:
```cpp
auto src = cle::Array::create(/*...*/);
auto dst = cle::Array::create(dst);
cle::tier1::add_image_and_scalar_func(device, src, dst, 5);
```

## <a id="pipeline">__CLIc mini-pipeline function__</a>


As more and more CLIc operation are developped, it is possible to reuse them in order to build but more complex and advance operation. This is done by creating a new function which will call the different CLIc operations in a specific order. This function will be a `tier2` operation as it relies on at least a `tier1` operations, `tier3` if it relies on at least a `tier2` operations, etc.

For this documentation we will use the `difference_of_gaussian` operation as an example. This operation rely on two operations from the tier1:
- [`gaussian_blur`]()
- [`difference`]()

We will first need to add the function declaration in the [`tier2`]() header file as followed:
```cpp
//--> clic/include/tier2.hpp
auto
difference_of_gaussian_func(const Device::Pointer & device,
                            const Array::Pointer &  src,
                            Array::Pointer          dst,
                            float                   sigma1_x,
                            float                   sigma1_y,
                            float                   sigma1_z,
                            float                   sigma2_x,
                            float                   sigma2_y,
                            float                   sigma2_z) -> Array::Pointer;
```
and the definition in the [`tier2`]() source file as followed:
```cpp
//--> clic/src/tier2.cpp
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
    /* function code */
}
```
Definition and declaration follows the same rulls as for the `tier1` operation.

We now simply need to implement the function core by following the `difference_of_gaussian` algorithm:
1. Create the output array if not provided by the user
2. Call the `gaussian_blur` operation
3. Call the `difference` operation
4. Return the output array

### 1. Create the output array if not provided by the user

Here we can rely on the `tier0` operations which already implement several output managment logics. The most classic being the `create_like` which, if `dst` is `nullptr` will create `dst` as a copy of `src`.
```cpp
tier0::create_like(src, dst, dType::FLOAT);
```
Gaussian operation output should be of `float` type, so we request that if `dst` is `nullptr` it is created as a `float` array.

### 2. Call the `gaussian_blur` operation

The operation requires to run two `gaussian_blur` operations with different sigma values. The first one with `sigma1` and the second one with `sigma2`. The `gaussian_blur` operation is a `tier1` operation and can be called as follow:
```cpp
auto blurred1 = tier1::gaussian_blur_func(device, src, nullptr, sigma1_x, sigma1_y, sigma1_z);
```
Here we can use the `nullptr` to let the function create the output array. We can do the same for the second `gaussian_blur` operation:
```cpp
auto blurred2 = tier1::gaussian_blur_func(device, src, nullptr, sigma2_x, sigma2_y, sigma2_z);
```
We have now two different blurred images stored in `blurred1` and `blurred2` on our device.

### 3. Call the `difference` operation

The last step of the algorithm is to compute the difference between the two blurred images. The `difference` operation is a `tier1` operation and can be called as follow:
```cpp
tier1::add_images_weighted_func(device, blurred1, blurred2, dst, 1, -1);
```
Here we pass the `dst` array as the output array as it was created in the first step.

### 4. Return the output array

Finally, we return the output array to the user.
```cpp
return dst;
```

making our final function as follow:
```cpp
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
  tier1::add_images_weighted_func(device, gauss1, gauss2, dst, 1, -1);
  return dst;
}
```

### Function usage

Now that the function is defined, it can be called as follow:
```cpp
auto src = cle::Array::create(/*...*/);
auto gpu_output = cle::tier2::difference_of_gaussian_func(device, src, nullptr, 1, 1, 1, 3, 3, 3);
```
or alternatively:
```cpp
auto src = cle::Array::create(/*...*/);
auto dst = cle::Array::create(dst);
cle::tier2::difference_of_gaussian_func(device, src, dst, 1, 1, 1, 3, 3, 3);
```

## More example and issues

Several operations are already implemented, most will correspond to these guidelines and can be used as more example. You can check the various tiers source for more inputs.

