![CI-Build](https://github.com/clEsperanto/CLIc_prototype/workflows/CI-Build/badge.svg)
![Windows-build](https://github.com/clEsperanto/CLIc_prototype/workflows/Windows-build/badge.svg)
# CLIc

CLIc is a prototype for [CLesperanto](https://github.com/clEsperanto) - a multi-language framework for GPU-accelerated image processing. It uses [OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels/tree/development/src/main/java/net/haesleinhuepf/clij/kernels) from [CLIJ](https://clij.github.io/)

Right now, this is very preliminary, and mainly focussed on running a few kernel using the OpenCL C++ API.

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
```

## Troubleshooting

Cmake control the project configuration and link to required libraries. If the compiled libraries and includes folder are installed and specified in your environment PATH, it should automatically found it and manage the dependencies to the project.   
If it fail configurating the project, verify that both LibTiff and OpenCL are installed, and that CMake point to the correct libraries version via cmake-gui for an easy access to CMake variables.

### LibTiff

If not in PATH environment, it is possible to specify the library location in cmake via the GUI using the following variables:
- TIFF_LIBRARY → which should point to libtiff compiled library (.so for Unix, .lib for Windows)
- TIFF_INCLUDE_DIR → which should point to the include folder containing the tiffio<span>.h and tiff<span>.h  

#### Linux:
Running the following in a terminal will download and install libtiff on your system
```
sudo apt update
sudo apt install -y libtiff-dev 
```

#### MacOS:
Normally already installed, it is also possible to get it using [Homebrew](https://brew.sh/) via the following line in terminal
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
- OPENCL_LIBRARY → which should point to libOpenCL or OpenCL compiled library (.so for Unix, .lib for Windows)
- OPENCL_INCLUDE_DIR → which should point to a folder containg CL/cl<span>.htiff<span>.h 

#### Linux:
Running the following in a terminal will download and install OpenCL
```
sudo apt update
sudo apt install -y ocl-icd-opencl-dev 
```
It is however advised to get OpenCL via your GPU drivers and SDK.

#### MacOS:
Normally already installed on MacOS system. 
It is also possible to get it by installing both GPU drivers and SDK from the your GPU brand (NVidia, AMD, Intel, etc.).

#### Windows:
It is required to install it via the GPU drivers and SDK from the your GPU brand (NVidia, AMD, Intel, etc.).

### Kernels
CLIc rely on the [CLIj OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels) and they are required for execution. Make sure you are downloading the submodule of the repository, or change the folder path variable CLI_Path in CMake configuration to point to the kernels location.

# Testing

The current main code run sequentially the add_image_and_scalar and maximum_z_projection kernel.

| input  | add scalar = 127 | maximum z projection |
| :--------:  | :--------: | :--------: |
| <img src="./ressources/input.gif" width="200" height="200" /> | <img src="./ressources/output_add_image_and_scalar.gif" width="200" height="200" /> | <img src="./ressources/output_maximum_z_projection.png" width="200" height="200" /> |

## Current architecture compatibility

| OS  | GPU | Driver | OpenCL | Test Status | Info |
| :--------:  | :--------: | :--------: | :--------: | :--------: | :--------: |
| Ubuntu 18.04  | NVIDIA GeForce GTX 1050 Ti | x84_64 440 | NVIDIA OpenCL 2.2 | PASS |  |
| Windows 10  | NVIDIA Quadro P4000 | NVIDIA quadro 450 | NVIDIA OpenCL 1.2 | PASS | |
| OSX 10.14.1  | ? | included with OS | ? | PASS | Most cl function are deprecated. Required CL_MEM_COPY_HOST_PTR in clCreateBuffer in Push(). |

## Issues

- Running kernel limited to (for now)
    - add_image_and_scalar
    - maximum_z_projection  
- Only process buffer data, image2d_t and image3d_t not supported

