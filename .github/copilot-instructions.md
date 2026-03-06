# Project Overview
CLIc is a C++ library for GPU-accelerated image processing using OpenCL or CUDA.
It aims to provide a simple and consistent interface for users while taking care of the complexity of GPU programming and optimization under the hood.
The library is designed to also be wrapped in other languages such as Python and Java.
It is the direct evolution of the CLIj library (https://github.com/clij/clij2)

## Library Structure
- `cmake/` contains the advance CMake configuration files for the library, it is used to build the library and manage its dependencies.
- `tests/` contains the unit tests for the library, it is used to ensure the correctness of the library and to catch any regressions.
- `clic/` contains the main implementation of the library
```
clic/
├── CMakeLists.txt
├── clic.hpp.in
├── include/
│   ├── array.hpp
│   ├── backend.hpp
│   ├── cache.hpp
│   ├── cle.hpp
│   ├── device.hpp
│   ├── execution.hpp
│   ├── fft.hpp
│   ├── slicing.hpp
│   ├── statistics.hpp
│   ├── tier0.hpp
│   ├── tier1.hpp
│   ├── tier2.hpp
│   ├── tier3.hpp
│   ├── tier4.hpp
│   ├── tier5.hpp
│   ├── tier6.hpp
│   ├── tier7.hpp
│   ├── tier8.hpp
│   ├── transform.hpp
│   ├── translator.hpp
│   └── utils.hpp
├── src/
│   ├── array.cpp
│   ├── backendmanager.cpp
│   ├── cache.cpp
│   ├── cudaTranslator.cpp
│   ├── cudabackend.cpp
│   ├── cudadevice.cpp
│   ├── execution.cpp
│   ├── fft.cpp
│   ├── openclbackend.cpp
│   ├── opencldevice.cpp
│   ├── slicing.cpp
│   ├── statistics.cpp
│   ├── tier0.cpp
│   ├── transform.cpp
│   ├── tier1/ (52 files)
│   ├── tier2/ (34 files)
│   ├── tier3/ (20 files)
│   ├── tier4/ (9 files)
│   ├── tier5/ (8 files)
│   ├── tier6/ (7 files)
│   ├── tier7/ (11 files)
│   └── tier8/ (3 files)
└── thirdparty/
    └── CMakeLists.txt
```

### Backends & Devices
Backend and Device are an abstract class that defines the interface for OpenCL and CUDA backends and Devices.
This is the lowest level of the library
it uses shared_ptr to manage memory and GPU resources to ensure that they are properly released when they are no longer needed.

### Array (array.hpp)
The Array class hold a pointer to the GPU memory and the metadata associated with it (data type, shape, etc.).
It hold a pointer to the backend and device that is used to allocate the memory.
It provide a set of method to access and manipulate the data on the GPU, such as copy, fill, etc.
It provide a set of method to copy to and from the GPU, such as copyTo and readFrom.
The Array class is the main data structure used in the library and is used as input and output for mostly all the functions in the library.

### Execution (execution.hpp)
execution.hpp provides implementations of functions that, takes GPU code, set of arguments, and a device, and execute the code on the device.
- `execute` is using the CLIJ Opencl implementation, it expect specific keywords from the CLIJ ecosystem in the GPU code.
- `native_execute` do the same but for pure OpenCL code.

### translator (translator.hpp)
The library is focused on OpenCL code but to support CUDA devices, we have a translator that translate OpenCL code to CUDA code.
The translator is implemented in translator.hpp and translator.cpp, it uses a simple string replacement approach to translate the code, it is not a full fledged translator but it is enough for our use case.
The translator is used in the execution functions to translate the OpenCL code to CUDA code when the device is a CUDA device.

### cache (cache.hpp)
The library provide a hot and cold cache for the build GPU code.
The hot cache is a LRU list that hold the most recently used GPU code.
The cold cache is a list of .bin files stored on disk.
When a GPU code is executed, the library first check the hot cache, if the code is found, it is executed directly, if not, it check the cold cache, if the code is found, it is loaded to the hot cache and then executed, if not, the code is compiled and then stored in both caches before being executed.

### Aditional implementation
Some advance functionnality required complex implementation which are managed in dedicated files and then provided with a simple interface in the tierX.hpp files, such as:
- `fft.hpp` provides an interface for 1D, 2D, and 3D FFTs on the GPU, and rely on the third party libraries vkFFT.
- `slicing.hpp` provides an interface for slicing and indexing on the GPU, it rely on a custom implementation of slicing and indexing on the GPU.
- `statistics.hpp` provides an interface for computing statistics on the GPU of a labelled image, it rely on a custom implementation of statistics on the GPU.
- `transform.hpp` provides an interface for geometric transformations on the GPU, it rely on a custom implementation of geometric transformations on the GPU.

### Tier Structure

The image processing operations are a list of functions that are categorized into tiers based on their complexity. Tier 1 contains pure GPU implementations, tier 2 contains functions that uses tier 1 functions, tier 3 contains functions that uses tier 2 functions, etc.
Function in the tiers are the main function used by the users of the library with the Array class as input and output.
Tier0 is an exception as it contains functions that are in charge of allocating output memory for the functions in the other tiers and are not meant to be used by the users of the library directly.

## Coding Standards
The library uses C++17 standard and rely on the standard library as much as possible.
The library is designed to be cross-platform and should compile on Windows, Linux, and MacOS with both OpenCL and CUDA support.
The library is designed to be thread safe and should be able to be used in multi-threaded applications without any issues.
The library uses clang-format for code formatting
