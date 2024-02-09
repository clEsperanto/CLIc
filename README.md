# CLIc

[![Build & test](https://github.com/clEsperanto/CLIc_prototype/actions/workflows/build-test.yml/badge.svg)](https://github.com/clEsperanto/CLIc_prototype/actions/workflows/build-test.yml)
[![CI-Tests](https://github.com/clEsperanto/CLIc_prototype/actions/workflows/tests_and_coverage.yml/badge.svg)](https://github.com/clEsperanto/CLIc_prototype/actions/workflows/tests_and_coverage.yml) 
[![Codecov](https://codecov.io/gh/clEsperanto/CLIc_prototype/branch/master/graph/badge.svg?token=QRSZHYDFIF)](https://codecov.io/gh/clEsperanto/CLIc_prototype)
[![License](https://img.shields.io/badge/license-BSD-informational)](https://github.com/clEsperanto/CLIc_prototype/blob/master/LICENSE)
[![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/clesperanto/CLIc_prototype?color=white&include_prereleases)](https://github.com/clEsperanto/CLIc_prototype/releases/latest)
[![CppStd](https://img.shields.io/badge/cpp--std-c%2B%2B17-blue)](https://en.cppreference.com/w/cpp/17)
[![OpenCL](https://img.shields.io/badge/OpenCL-3.0-green)](https://www.khronos.org/opencl/)
[![Website](https://img.shields.io/website?url=http%3A%2F%2Fclesperanto.net)](http://clesperanto.net)
[![GitHub issues](https://img.shields.io/github/issues-raw/clEsperanto/CLIc_prototype)](https://github.com/clEsperanto/CLIc_prototype/issues)
[![GitHub stars](https://img.shields.io/github/stars/clEsperanto/CLIc_prototype?style=social)](https://github.com/clEsperanto/CLIc_prototype)
[![GitHub forks](https://img.shields.io/github/forks/clEsperanto/CLIc_prototype?style=social)](https://github.com/clEsperanto/CLIc_prototype)

CLIc is a **prototype** backend library for [CLesperanto](https://github.com/clEsperanto) - a multi-language framework for GPU-accelerated image processing. It uses the [OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels/tree/development/src/main/java/net/haesleinhuepf/clij/kernels) from [CLIJ](https://clij.github.io/)

It is a preliminary projet and mainly focussed on running a kernel using the [OpenCL C++ API from Khronos](https://github.com/KhronosGroup/OpenCL-CLHPP).

# __Usage example__

```c++
#include "clesperanto.h"  // CLIc include

int main( int argc, char** argv)
{
    // Initialisation of clEsperanto with default device
    cle::BackendManager::getInstance().setBackend();
    auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

    // host vector to hold input and output
    std::vector<float> input (width * height * depth); 
    std::vector<float> output (input.size()); 

    /*
     * ... fill input with data to process  
     */

    // push data from host to device
    auto gpu_src = cle::Push<float>(input.data(), width, height, depth, device);
    // apply filter with parameters
    auto gpu_dst = cle::tier1::AddImageAndScalar(device, gpu_src, nullptr, 10);
    // pull output from device to host
    cle::Pull<float>(gpu_dst, output.data()); 

    return EXIT_SUCCESS;
}
```
See more complete example on usage by looking at the kernels [tests](./tests/).

# __Installation__

## Requirements

CLIc rely on OpenCL language for parallelisation and CMake for configuration and build.  
Follow the [installation guide](./docs/guidelines.md) for helps on compilation and installation. 

## Quick install

Clone the repository and update the submodules
```
git clone git@github.com:clEsperanto/CLIc_prototype.git
cd CLIc_prototype
```

Create a build folder and configure cmake to generate the adapted makefile.
Then compile the library and install it at specified location on your system.
```
cmake -S . -B ./build -D CMAKE_INSTALL_PREFIX=/path/to/installation/folder
cmake --build ./build --parallel 10 --target install
```

You can modify installation location using `-D CMAKE_INSTALL_PREFIX=/path/to/installation/folder`. If not specify, the default location is `/usr/local`.

CLIc can be uninstall by running the following command
```
cmake --build ./build --target uninstall
```

## Kernels
CLIc filters rely on the clEsperanto branch of [CLIj OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels). They are managed as a submodule of this repository.

# __Contributing__
Contributions are very welcome. Before spending effort on coding and filing a pull-request, please get in touch with us, [file an issue](https://github.com/clEsperanto/CLIc_prototype/issues), and let's discuss your potential contribution. 
More information on how to add new kernels to the library can be found in the [documentation](./docs/add_new_kernel/add_new_kernel.md).

# __Feedback welcome!__
clEsperanto is developed in the open because we believe in the [open source community](https://clij.github.io/clij2-docs/community_guidelines). 
Feel free to drop feedback as [github issue](https://github.com/clEsperanto/CLIc_prototype/issues) or via [image.sc](https://image.sc).  
And if you liked our work, star the repository, share it with your friends, and use it to make cool stuff!

## Acknowledgements

We acknowledge support by the Deutsche Forschungsgemeinschaft under Germany’s Excellence Strategy (EXC2068) Cluster of Excellence Physics of Life of TU Dresden.
This project has been made possible in part by grant number 2021-237734 ([GPU-accelerating Fiji and friends using distributed CLIJ, NEUBIAS-style, EOSS4](https://chanzuckerberg.com/eoss/proposals/gpu-accelerating-fiji-and-friends-using-distributed-clij-neubias-style/)) from the Chan Zuckerberg Initiative DAF, an advised fund of the Silicon Valley Community Foundation.
