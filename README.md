![CI-Build](https://github.com/clEsperanto/CLIc_prototype/workflows/CI-Build/badge.svg)
# CLIc

CLIc is a **prototype** for [CLesperanto](https://github.com/clEsperanto) - a multi-language framework for GPU-accelerated image processing. It uses [OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels/tree/development/src/main/java/net/haesleinhuepf/clij/kernels) from [CLIJ](https://clij.github.io/)

Right now, this is very preliminary, and mainly focussed on running a few kernel using the OpenCL C++ API.

# Usage example

```c++
#include "CLE.h"  // CLIc include

int main( int argc, char** argv)
{
    cle::GPU gpu();       // Initialise GPU 
    cle::CLE cle(gpu);    // Initialise cle

    // store data to process in vector
    unsigned int dimensions[3] = {width, height, depth};
    std::vector<float> input [width * height * depth]; 

    /*
     * ... fill input with data to process  
     */

    // push data into GPU, and create the output into GPU
    cle::Buffer src = cle.Push<float>(input, dimensions);

    // create output GPU object and apply filter
    cle::Buffer dst = cle.Create<float>(dimensions);
    cle.AddImageAndScalar(src, dst, 10);  

    // get output from filter into vector
    Image<float> output = cle.Pull<float>(dst); 

    return EXIT_SUCCESS;
}
```
See more complete example on usage by looking at the [validation tests](https://github.com/clEsperanto/CLIc_prototype/tree/master/test).

# Installation

## Requierements

- [OpenCL](https://www.khronos.org/opencl/) 
(
    [Nvidia](https://developer.nvidia.com/cuda-downloads), 
    [AMD](https://github.com/GPUOpen-LibrariesAndSDKs/OCL-SDK/releases), 
    [Intel](https://software.intel.com/content/www/us/en/develop/tools/opencl-sdk.html)
)
- [CMake](https://cmake.org/)

Guideline for installation requirements and troubleshooting [here](./docs/clic_prerequish_install.md).

## Install

Compiled API available for Windows 10, MaxOS 10.14 and newer, and Ubuntu 18.04 available [here](https://github.com/clEsperanto/CLIc_prototype/releases/tag/0.1).  

[See](./docs/clic_compile_guide.md) compilation guide for installation from source.

### Kernels
CLIc rely on the [CLIj OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels). They are required for the proper execution of CLIc API. Make sure you are downloading the repository submodule, or update the kernels folder directory variable during the CMake configuration to point to the kernels location.

# Feedback welcome!
clEsperanto is developed in the open because we believe in the [open source community](https://clij.github.io/clij2-docs/community_guidelines). Feel free to drop feedback as [github issue](https://github.com/clEsperanto/CLIc_prototype/issues) or via [image.sc](https://image.sc)

# Acknowledgement

CMake library configuration extended from [cmake-example-library](https://github.com/pablospe/cmake-example-library) by Pablo Speciale
