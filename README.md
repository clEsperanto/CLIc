# CLIc

CLIc is a prototype for [CLesperanto](https://github.com/clEsperanto) - a multi-language framework for GPU-accelerated image processing. It uses [OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels/tree/development/src/main/java/net/haesleinhuepf/clij/kernels) from [CLIJ](https://clij.github.io/)

Right now, this is very preliminary, and mainly focussed on running a few kernel using the OpenCL C++ API.

# Installation

## Requierements

- [LibTiff](https://en.wikipedia.org/wiki/Libtiff)
- [OpenCL](https://www.khronos.org/opencl/)  
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

# Testing

The current main code run sequentially the add_image_and_scalar and maximum_z_projection kernel.

| input  | add scalar = 127 | maximum z projection |
| :--------:  | :--------: | :--------: |
| <img src="./ressources/input.gif" width="200" height="200" /> | <img src="./ressources/output_add_image_and_scalar.gif" width="200" height="200" /> | <img src="./ressources/output_maximum_z_projection.png" width="200" height="200" /> |


# Issues

- Running kernel limited to (for now)
    - add_image_and_scalar
    - maximum_z_projection  
- Only process data in 1D array

