# CLIc

CLIc is a prototype for [CLesperanto](https://github.com/clEsperanto) - a multi-language framework for GPU-accelerated image processing. It uses [OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels/tree/development/src/main/java/net/haesleinhuepf/clij/kernels) from [CLIJ](https://clij.github.io/)

Right now, this is very preliminary, and mainly focussed on running a few kernel using the OpenCL C++ API.

# Installation

## Requierements

- [LibTiff](http://www.simplesystems.org/libtiff/)
- [OpenCL](https://www.khronos.org/opencl/) (version 1.2) 
    - [Nvidia Platform](https://developer.nvidia.com/cuda-downloads)  
    - AMD Platform  
    - [Intel Platform](https://software.intel.com/content/www/us/en/develop/tools/opencl-sdk.html)
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
```

## Troubleshooting

Cmake control the project configuration and path to required libraries. If it fail generating the make file, open the cmake-gui to easily access key configuration variable.

### LibTiff
It is require to link the include folder and the compiled library to the project. This is done automatically if already in PATH through the *find_package()* cmake function, otherwise can be specified to cmake through the following variables:
- TIFF_LIBRARY → which should point to libtiff (.so for Unix, .lib for Windows)
- TIFF_INCLUDE_DIR → which should point folder containing the tiffio<span>.h and tiff<span>.h  

Both can be found in the libtiff folder provided in the last version of the library [LibTiff](http://www.simplesystems.org/libtiff/).

### OpenCL
OpenCL is usually already provided by your OS or can be found in the SDK corresponding to your GPU platform (Nvidia, Intel, etc.). If SDK in the PATH, the *find_package()* cmake function shoudl find it, otherwise both can be set using the variables:
- OPENCL_LIBRARY → which should point to libOpenCL or OpenCL (.so for Unix, .lib for Windows)
- OPENCL_INCLUDE_DIR → which should point to a folder containg CL/cl<span>.h

### Others
CLIj kernels opencl file are required for execution, those are provided by the submodules. The folder can be redefined using the variable CLI_Path in cmake-gui.

# Testing

The current main code run sequentially the add_image_and_scalar and maximum_z_projection kernel.

| input  | add scalar = 127 | maximum z projection |
| :--------:  | :--------: | :--------: |
| <img src="./ressources/input.gif" width="200" height="200" /> | <img src="./ressources/output_add_image_and_scalar.gif" width="200" height="200" /> | <img src="./ressources/output_maximum_z_projection.png" width="200" height="200" /> |

## Current architecture compatibility

| OS  | GPU | Driver | OpenCL | Test Status | Info |
| :--------:  | :--------: | :--------: | :--------: | :--------: | :--------: |
| Ubuntu 18.04  | NVIDIA GeForce GTX 1050 Ti | x84_64 440 | NVIDIA OpenCL 2.2 | PASS | clCreateCommandQueue is deprecated |
| Windows 10  | NVIDIA Quadro P4000 | NVIDIA quadro 450 | NVIDIA OpenCL 1.2 | PASS | clCreateCommandQueueWithProperties Not Recognised, use clCreateCommandQueue |
| OSX 10.14.1  | ? | included with OS | ? | PASS | All cl function are deprecated. Required CL_MEM_COPY_HOST_PTR in clCreateBuffer in Push(). |

## Issues

- Running kernel limited to (for now)
    - add_image_and_scalar
    - maximum_z_projection  
- Only process buffer data, image2d_t and image3d_t not supported

