![CI-Build](https://github.com/clEsperanto/CLIc_prototype/workflows/CI-Build/badge.svg)
# CLIc

CLIc is a prototype for [CLesperanto](https://github.com/clEsperanto) - a multi-language framework for GPU-accelerated image processing. It uses [OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels/tree/development/src/main/java/net/haesleinhuepf/clij/kernels) from [CLIJ](https://clij.github.io/)

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

## Install

clone the repository
```
git clone git@github.com:clEsperanto/CLIc_prototype.git
```
download the submodule required (here the CLIJ kernels)
```
cd CLIc_prototype
git submodule update --init --recursive
```
create de build folder and call cmake from it to generate a make file
```
mkdir build  
cd build  
ccmake -DCMAKE_BUILD_TYPE=Release ..  
```
call make to compile.
```
make 
make test
```

## Troubleshooting

Cmake control the project configuration and linking to required libraries (aka OpenCL and LibTiff). If the libraries and their respective includes folder are installed and/or specified in your environment PATH, it should automatically find it and manage the dependencies to the project. Otherwise, CMake will fail the configuration steps.

### LibTiff

If not in PATH environment, it is possible to specify the library location in cmake via the following variables:
- TIFF_LIBRARY → which should point to libtiff compiled library
- TIFF_INCLUDE_DIR → which should point to the include folder   

Either by using cmake GUI (recommended) or by adding -DTIFF_LIBRARY=<path_to_library> and -DTIFF_INCLUDE_DIR=<path_to_include_dir> when calling ccmake.

#### Install:
With apt if using Linux OS
```
sudo apt update
sudo apt install -y libtiff-dev 
```

Already provided in MacOS, otherwise can be installed with [Homebrew](https://brew.sh/) via the following line in terminal
```
sudo brew update
sudo brew install libtiff
```

#### Windows:
Download the latest source code version on the library [website](http://www.simplesystems.org/libtiff/). The code can be compile using the following commande line in the [x64 Native Tools Command Prompt](https://docs.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=vs-2019) of windows
```
nmake /f makefile.vc
```
Both compiled library and includes will be found in the libtiff folder.
More information on libtiff compilation [here](http://www.simplesystems.org/libtiff/build.html)

### OpenCL

Same as for the TIFF library, it is possible to specify the library location via CMake GUI with the variables:
- OPENCL_LIBRARY → which should point to libOpenCL or OpenCL compiled library
- OPENCL_INCLUDE_DIR → which should point to the include folder

Either by using cmake GUI (recommended) or by adding -DOPENCL_LIBRARY=<path_to_library> and -DOPENCL_INCLUDE_DIR=<path_to_include_dir> when calling ccmake.

#### Install
Follow the GPU SDK installation guide of your vendor (NVidia, AMD, Intel, ...) 

### Kernels
CLIc rely on the [CLIj OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels). They are required for the proper execution of CLIc. Make sure you are downloading the repository submodule, or update the kernels folder path variable CLI_Path during the CMake configuration to point to the kernels location.

## Issues
- Only process buffer object. image2d_t and image3d_t not yet supported
- CI Workflow do not properly work, current code compile and runs on
    - Ubuntu 18.04 - NVidia GeForce device
    - MacOS 10.14 - NVidia GeForce device
    - Windows 10 - NVidia GeForce device

