# CLIc pre-requish installation

In order to use CLIc, a minimal set of API and software is required to be install on your workstation.  
- [OpenCL](https://www.khronos.org/opencl/) 
(
    [Nvidia](https://developer.nvidia.com/cuda-downloads), 
    [AMD](https://github.com/GPUOpen-LibrariesAndSDKs/OCL-SDK/releases), 
    [Intel](https://software.intel.com/content/www/us/en/develop/tools/opencl-sdk.html)
)
- [CMake](https://cmake.org/)
- A C++ compiler

None of those are OS specific, and therefore CLIc can compile and run on Windows, Mac, and Linux.

---

## Windows

### C++ Compiler
[Visual Studio](https://visualstudio.microsoft.com/downloads/) will provide a compiler and an IDE for heavy project compilation. It is compatible with CMake project generator.  

### OpenCL
Note: This should be done after installing Visual Studio.

Install an OpenCL SDK, for example from [Intel](https://software.intel.com/content/www/us/en/develop/tools/opencl-sdk/choose-download.html).  After installation you will find the OpenCL includes under `C:\Program Files (x86)\IntelSWTools\system_studio_2020\OpenCL\sdk\includes` and the OpenCL library under `C:\Program Files (x86)\IntelSWTools\system_studio_2020\OpenCL\sdk\lib\x64\OpenCL.lib`.

Alternative possibility for NVidia users, install [CUDA SDK](https://developer.nvidia.com/cuda-downloads) which come with OpenCL. After installation you will find the OpenCL includes under `C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\include` and the OpenCL library under `C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\lib\x64\OpenCL.lib`.

### Cmake
[Download and install](https://github.com/Kitware/CMake/releases/download/v3.19.0/cmake-3.19.0-win64-x64.msi) and install the .msi from the CMake website.

---
## MacOS

### C++ Compiler
In a terminal window, run the following command: `xcode-select --install`

### OpenCL
MacOS should come with OpenCL already installed. Nothing to do here.

### Cmake
[Download and install](https://github.com/Kitware/CMake/releases/download/v3.19.0/cmake-3.19.0-Darwin-x86_64.dmg) and install the .dmg from the CMake website

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
In a terminal window, run the following command
```
sudo apt update
sudo apt install ocl-icd-opencl-dev 
```

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

## Others

Possible missing includes of `CL/cl2.hpp` error requires the install of [OpenCL-CLHPP-Headers](https://github.com/KhronosGroup/OpenCL-CLHPP).







