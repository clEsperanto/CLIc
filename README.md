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
cd CLIc
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
- TIFF_LIBRARY
- TIFF_INCLUDE_DIR  

### OpenCL
OpenCL is usually provided in the SDK corresponding to your GPU platform (Nvidia, Intel, etc.). If SDK in the PATH, the *find_package()* cmake function shoudl find it, otherwise both can be set using the variables:
- OPENCL_LIBRARY
- OPENCL_INCLUDE_DIR 

### Others
CLIj kernels and preamble opencl file are required for execution, the path to both of them is defined by CLI_Path and CLP_Path in cmake.

# Testing

The current main code run sequentially the add_image_and_scalar and maximum_z_projection kernel.

| input  | add scalar = 127 | maximum z projection |
| :--------:  | :--------: | :--------: |
| <img src="./ressources/input.gif" width="200" height="200" /> | <img src="./ressources/output_add_image_and_scalar.gif" width="200" height="200" /> | <img src="./ressources/output_maximum_z_projection.png" width="200" height="200" /> |

## Current architecture compatibility

| OS  | GPU | Driver | OpenCL | Test Status |
| :--------:  | :--------: | :--------: | :--------: | :--------: |
| Ubuntu 18.04  | NVIDIA GeForce GTX 1050 Ti | x84_64 440 | NVIDIA OpenCL 1.2 | PASS |
| Windows 10  | NVIDIA Quadro P4000 | nvidia quadro 450 | NVIDIA OpenCL 1.2 | PASS |

## Issues

- Running kernel limited to (for now)
    - add_image_and_scalar
    - maximum_z_projection  
- Only process buffer data, image2d_t and image3d_t not supported

