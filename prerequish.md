# CLIc pre-requish installation

In order to use CLIc, a minimal set of API and software is required to be install on your workstation.  
- [OpenCL](https://www.khronos.org/opencl/) 
(
    [Nvidia](https://developer.nvidia.com/cuda-downloads), 
    [AMD](https://github.com/GPUOpen-LibrariesAndSDKs/OCL-SDK/releases), 
    [Intel](https://software.intel.com/content/www/us/en/develop/tools/opencl-sdk.html)
)
- [OpenCL API C++ Binding](https://github.com/KhronosGroup/OpenCL-CLHPP)
- [CMake](https://cmake.org/)
- A C++ compiler

---

## Windows

### C++ Compiler
[Visual Studio](https://visualstudio.microsoft.com/downloads/) will provide a compiler and an IDE for heavy project compilation. 

### OpenCL Library

Note: This should be done after installing Visual Studio.

#### OpenCL-SDK

Install an OpenCL SDK, for example from [Intel](https://software.intel.com/content/www/us/en/develop/tools/opencl-sdk/choose-download.html).  After installation you will find the OpenCL includes under `C:\Program Files (x86)\IntelSWTools\system_studio_2020\OpenCL\sdk\includes` and the OpenCL library under `C:\Program Files (x86)\IntelSWTools\system_studio_2020\OpenCL\sdk\lib\x64\OpenCL.lib`.

Alternative possibility for NVidia users, install [CUDA SDK](https://developer.nvidia.com/cuda-downloads) which come with OpenCL. After installation you will find the OpenCL includes under `C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\include` and the OpenCL library under `C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\lib\x64\OpenCL.lib`.

#### OpenCL API C++ Binding

[Download](https://github.com/KhronosGroup/OpenCL-CLHPP/blob/master/include/CL/opencl.hpp) and copy the file in the OpenCL-SDK includes folder (may require admin privileges)

### Cmake
[Download](https://cmake.org/download/) and install the lastest version.

---
## MacOS

### C++ Compiler
In a terminal window, run the following command: `xcode-select --install`

#### OpenCL-SDK

MacOS should come with OpenCL already installed. Nothing to do here.

#### OpenCL API C++ Binding

[Download](https://github.com/KhronosGroup/OpenCL-CLHPP/blob/master/include/CL/opencl.hpp) and copy the file in the OpenCL-SDK includes folder (may require admin privileges)
Default location should be in: `/System/Library/Frameworks/OpenCL.framework/Headers/OpenCL`

### Cmake
[Download](https://cmake.org/download/) and install the lastest version.

---
## Linux

### C++ Compiler
You can check if already installed by running the following command in a terminal
```
which g++
g++ --version
```
If not installed, run the following command
```
sudo apt update
sudo apt install build-essential
```

### OpenCL

#### OpenCL-SDK

In a terminal window, run the following command
```
sudo apt update
sudo apt install opencl-headers ocl-icd-opencl-dev 
```

#### OpenCL API C++ Binding

[Download](https://github.com/KhronosGroup/OpenCL-CLHPP/blob/master/include/CL/opencl.hpp) and copy the file in the OpenCL-SDK includes folder (may require admin privileges). 
Default location should be in: `/usr/include/CL`

### Cmake
You can check if already installed by running the following command in a terminal
```
which cmake-gui
cmake-gui --version
```
If not installed, run the following command
```
sudo apt update
sudo apt install cmake cmake-qt-gui 
```
Once installed, CMake (GUI) can be start by running the following command in a terminal
```
cmake-gui 
```
Alternatively you can [Download](https://cmake.org/download/) and install the lastest version from their website.







