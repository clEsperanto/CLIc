# CLIc pre-requish installation

In order to use CLIc, a minimal set of API and software is required to be install on your workstation.  
- [LibTiff](http://www.simplesystems.org/libtiff/)
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
ToDo

### OpenCL
ToDo

### LibTiff
ToDo

### Cmake
[Download and install](https://github.com/Kitware/CMake/releases/download/v3.19.0/cmake-3.19.0-win64-x64.msi) and install the .msi from the CMake website

---
## MacOS

### C++ Compiler
In a terminal window, run the follwing command
```
xcode-select --install
```

### OpenCL
MacOS comes with OpenCL already installed. Nothing to do here normally.

### LibTiff
MacOS comes with LibTiff already installed. Nothing to do here normally.

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

### LibTiff
You can check if already installed by running the following command in a terminal
```
apt-cache search libtiff-dev
```
If not installed, run the following command
```
sudo apt update
sudo apt install libtiff-dev 
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







