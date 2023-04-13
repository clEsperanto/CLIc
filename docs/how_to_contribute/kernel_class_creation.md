# Kernel class creation

All kernel operations in CLIc must inherite from the `Operation` class which define all the major functions needed for running an OpenCL kernel, and must have a valid kernel file `.cl` associated to it. The `Operation` class is defined in [`clic/include/core/cleOperation.hpp`](https://github.com/clEsperanto/CLIc_prototype/blob/master/clic/include/core/cleOperation.hpp) and the source code is in [`clic/src/core/cleOperation.cpp`](https://github.com/clEsperanto/CLIc_prototype/blob/master/clic/src/core/cleOperation.cpp). The kernel files are located in [clij-opencl-kernels](https://github.com/clEsperanto/clij-opencl-kernels/tree/clesperanto_kernels) repository.

## Define a new kernel class

The first step is to define a new class inheriting from `Operation` class, by creating a header file (`.hpp`) and a source file (`.cpp`). The class name must correspond to the kernel name. For example, the `AddImageAndScalarKernel` kernel is defined in [`clic/include/tier1/cleAddImageAndScalarKernel.hpp`]() and the source code is in [`clic/src/tier1/cleAddImageAndScalarKernel.cpp`]().

The operations are grouped in different tiers, defining their complexity. The `tier1` operations are the most basic operations, the `tier2` operations are more complex operations which rely on some `tier1` operations. The `tier3` operations rely a minima on a `tier2` operation. The `tier4` on `tier3` and so on.

### <a id="header">__Header file__</a>

First we declare the `AddImageAndScalarKernel` class in a header (`.hpp`) file as follow:

```cpp
//--> cleAddImageAndScalarKernel.hpp

#ifndef __TIER1_CLEADDIMAGEANDSCALARKERNEL_HPP        // <-- include guard
#define __TIER1_CLEADDIMAGEANDSCALARKERNEL_HPP        //

#include "cleOperation.hpp"                           // <-- include the Operation class

namespace cle                                         // <-- namespace cle
{

class AddImageAndScalarKernel : public Operation      // <-- kernel class which inherit 
{                                                     //     from Operation class
    public:
    /* my class methods */
}

} // namespace cle                                    // <-- end namespace cle

#endif // __TIER1_CLEADDIMAGEANDSCALARKERNEL_HPP      // <-- close include guard
```
It is inside this class that we will declare the methods needed for running the kernel.

By default a kernel class must declare at least the following functions: 
- `AddImageAndScalarKernel`: the class constructor to instanciate the kernel
- `SetInput` and `SetOutput`: to define the input and output images.

In the case of the `AddImageAndScalarKernel` kernel, we also need to declare the `SetScalar` method to define the scalar value to add to the image.

The constructor is declare as follow:

```cpp
explicit AddImageAndScalarKernel(const ProcessorPointer & device);
```

The `SetInput` and `SetOutput` functions must be declare as follow:
```cpp
auto SetInput(const Image & object) -> void;
auto SetOutput(const Image & object) -> void;
```

__Warning__: If a kernel requires multiple input or output, the `SetInput` and `SetOutput` functions must be differenciated by adding an index to the function name. For example, the `AddImages` kernel requires two input images and one output image, so the `SetInput` and `SetOutput` functions are declare as `SetInput1`, `SetInput2` and `SetOutput`.

And to finish the `SetScalar` function is declare as follow:
```cpp
auto SetScalar(const float & scalar) -> void;
```

As you can see the header file only hold declaration of class and methods, no actual code is written here. This is the role of the source file.

### <a id="source">__Source file__</a>

For each header file we need to create a corresponding source file (`.cpp`) with the same name. The source file is where the class methods code are defined. 
```cpp
//--> cleAddImageAndScalarKernel.cpp

#include "cleAddImageAndScalarKernel.hpp"    // <-- include the header file
#include "cle_add_image_and_scalar.h"        // <-- include the opencl kernel file

namespace cle                                // <-- namespace cle
{
    /* define class method here */

} // namespace cle                           // <-- end namespace cle

```

From the header file previously created we can see that the `AddImageAndScalarKernel` class requires the following methods to be declared:
- `AddImageAndScalarKernel`: the class constructor to instanciate the kernel
- `SetInput` and `SetOutput`: to define the input and output images.
- `SetScalar`: to define the scalar value to add to the image.

The constructor is defined as follow:
```cpp
AddImageAndScalarKernel::AddImageAndScalarKernel(const ProcessorPointer & device) : 
    Operation(device, 3)
{
  this->SetSource("add_image_and_scalar", oclKernel::add_image_and_scalar);
}
```
This is the most complex methods we will declare for now. The constructor is called when we instanciate the kernel class. To do so we first rely on the mother class `Operation` constructor whic takes two arguments: the `device` on which the kernel will run and the number of arguments of the kernel. In this case we have three arguments: the `input` image, the `output` image and the `scalar` value. Then, to finish the constructor, we need to provide the kernel source code which will run on the GPU. This is done by calling the `SetSource` method which takes two arguments: the kernel name as it is name in the OpenCL code and the kernel source file. The kernel source file is stored in a string variable and is called from the included header of the opencl kernel. Those headers are generated from the list of OpenCL files from the [`clij-opencl-kernel`](https://github.com/clEsperanto/clij-opencl-kernels/tree/clesperanto_kernels) repository.

Once the constructor is done, we can declare the `SetInput` and `SetOutput` functions as follow:
```cpp
auto AddImageAndScalarKernel::SetInput(const Image & object) -> void 
{
    this->AddParameter("src", object);
}

auto AddImageAndScalarKernel::SetOutput(const Image & object) -> void 
{
    this->AddParameter("dst", object);
}
```
Here, the `AddParameter` method is called from the mother class to add the input and output images to the kernel arguments. The `AddParameter` method takes two arguments: the argument name identical as it is in the OpenCL code and the image object.

Finally, the `SetScalar` function is defined as follow:
```cpp
auto AddImageAndScalarKernel::SetScalar(const float & scalar) -> void 
{
    this->AddParameter("scalar", scalar);
}
```
Same as for the `SetInput` and `SetOutput` functions, the `AddParameter` method is called to add the scalar value to the kernel arguments. The `AddParameter` method can be used as well to add also integers.

### <a id="call-and-gateway-access">__Call function and Gateway methaod__</a>

## __The Kernel Call function__

The last step to finalise the class if to declare the `Call` function. This function is the one that will be called to run the kernel. It is named identically as the class with the prefix `_Call` and is defined in the header file as an `inline` function. The `inline` keyword is used to tell the compiler to copy the function code at the place where the function is called. This is done to avoid the overhead of calling a function. The `inline` keyword is not mandatory but it is recommended to use it for the `Call` function.

The `Call` function is defined as follow:
```cpp
inline auto AddImageAndScalarKernel_Call(const ProcessorPointer & device, const Image & src, const Image & dst, const float & scalar) -> void
{
    AddImageAndScalarKernel kernel(device);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetScalar(scalar);
    kernel.Execute();
}
```
It allows a quick and simple way to call and run the kernel without having to instanciate the kernel class. The `Call` function takes the same arguments as the kernel class constructor and the `SetInput`, `SetOutput` and `SetScalar` functions. The `Call` function is then responsible to instanciate the kernel class, to set the input and output images and the scalar value and to run the kernel.  
In addition to simplying kernel call, it enable simple python wrapper for the `pyclesperanto` package. The drowback is that each call of the `Call` function will instanciate a new kernel class. This is not a problem for small kernels but for large kernels it is better to instanciate the kernel class only once and to call the `Execute` method multiple times.

## Add a new method to the `Clesperanto` class gateway

In order to make the new kernel accessible to the user, we need to add a new method to the `Clesperanto` class gateway.
The `Clesperanto` class is defined in [`clic/include/core/clesperanto.hpp`](https://github.com/clEsperanto/CLIc_prototype/blob/master/clic/include/core/clesperanto.hpp) and the source code is in [`clic/src/core/clesperanto.cpp`](https://github.com/clEsperanto/CLIc_prototype/blob/master/clic/src/core/clesperanto.cpp).

### Modify the header file

Add the new kernel as a method to the header (`clic/include/core/clesperanto.hpp`):
```cpp
  auto
  AddImageAndScalar(const Image & source, const Image & destination, const float & scalar = 0) -> void;
```

### Modify the source file

And call `AddImageAndScalarKernel_Call` from within this method in (`clic/src/core/clesperanto.cpp`):
```cpp
auto
Clesperanto::AddImageAndScalar(const Image & source, const Image & destination, const float & scalar) -> void
{
  AddImageAndScalarKernel_Call(this->GetDevice(), source, destination, scalar);
}
```

### __Summary__

Now that the kernel is accessible to the user, it is very important to [**provide a valid test case**](https://github.com/clEsperanto/CLIc_prototype/blob/extend_doc_add_new_kernel/docs/add_new_kernel/add_test_case.md) to insure that the kernel is working as expected.