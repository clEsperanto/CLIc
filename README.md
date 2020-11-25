![CI-Build](https://github.com/clEsperanto/CLIc_prototype/workflows/CI-Build/badge.svg)
# CLIc

CLIc is a **prototype** for [CLesperanto](https://github.com/clEsperanto) - a multi-language framework for GPU-accelerated image processing. It uses [OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels/tree/development/src/main/java/net/haesleinhuepf/clij/kernels) from [CLIJ](https://clij.github.io/)

Right now, this is very preliminary, and mainly focussed on running a few kernel using the OpenCL C++ API.

# Usage example

Include main header and initialise class with GPU information
```
#include "CLE.h"  
```
Initialise CLEsperanto with GPU usage preferences (more to come ... one day)
```
cle::GPU gpu();     
cle::CLE cle(gpu);
```
You can then push an image to the GPU and create memory there:
```
cle::Buffer src = cle.Push<float>(input);
cle::Buffer dst = cle.Create<float>(input);
```
And then you can call methods in the GPU and pull the output:
```
cle.AddImageAndScalar(srd, dst, scalar);  
Image<float> output = cle.Pull<float>(dst); 
```
See more complete example on usage by looking at the [validation tests](https://github.com/clEsperanto/CLIc_prototype/tree/master/test).

Empty CLIc script project available [here](https://github.com/StRigaud/CLIc_project_template)

# Installation

## Requierements

- [LibTiff](http://www.simplesystems.org/libtiff/)
- [OpenCL](https://www.khronos.org/opencl/) 
(
    [Nvidia](https://developer.nvidia.com/cuda-downloads), 
    [AMD](https://github.com/GPUOpen-LibrariesAndSDKs/OCL-SDK/releases), 
    [Intel](https://software.intel.com/content/www/us/en/develop/tools/opencl-sdk.html)
)
- [CMake](https://cmake.org/)

Guideline for requirements installation and troubleshooting [here](./docs/clic_prerequish_install.md).

## Install

Latest release compiled library is available for Windows, MacOS, and Linux Ubuntu [here](./README.md).  

Guide for compiling sources available [here](./docs/clic_compile_guide.md)

### Kernels
CLIc rely on the [CLIj OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels). They are required for the proper execution of CLIc. Make sure you are downloading the repository submodule, or update the kernels folder path variable CLI_Path during the CMake configuration to point to the kernels location.

# Feedback welcome!
clEsperanto is developed in the open because we believe in the [open source community](https://clij.github.io/clij2-docs/community_guidelines). Feel free to drop feedback as [github issue](https://github.com/clEsperanto/CLIc_prototype/issues) or via [image.sc](https://image.sc)