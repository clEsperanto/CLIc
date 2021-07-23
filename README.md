[![CI-Build](https://github.com/clEsperanto/CLIc_prototype/actions/workflows/build.yml/badge.svg)](https://github.com/clEsperanto/CLIc_prototype/actions/workflows/build.yml)
[![CI-Tests](https://github.com/clEsperanto/CLIc_prototype/actions/workflows/tests_and_coverage.yml/badge.svg)](https://github.com/clEsperanto/CLIc_prototype/actions/workflows/tests_and_coverage.yml) 
[![Codecov](https://codecov.io/gh/clEsperanto/CLIc_prototype/branch/master/graph/badge.svg?token=QRSZHYDFIF)](https://codecov.io/gh/clEsperanto/CLIc_prototype)
[![License](https://img.shields.io/badge/license-BSD-informational)](https://github.com/clEsperanto/CLIc_prototype/blob/master/LICENSE)
[![Website](https://img.shields.io/website?url=http%3A%2F%2Fclesperanto.net)](http://clesperanto.net)

# CLIc

CLIc is a **prototype** for [CLesperanto](https://github.com/clEsperanto) - a multi-language framework for GPU-accelerated image processing. It uses [OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels/tree/development/src/main/java/net/haesleinhuepf/clij/kernels) from [CLIJ](https://clij.github.io/)

Right now, this is very preliminary, and mainly focussed on running a few kernel using the OpenCL C++ API.

# Usage example

```c++
#include "CLE.h"  // CLIc include

int main( int argc, char** argv)
{
    cle::GPU gpu();       // Initialise default GPU 
    cle::CLE cle(gpu);    // Create cle instance

    // store data to process in vector
    unsigned int dimensions[3] = {width, height, depth};
    std::vector<float> input (width * height * depth); 

    /*
     * ... fill input with data to process  
     */

    // push data into GPU, and create the output into GPU
    cle::Buffer src = cle.Push<float>(input, dimensions);
    cle::Buffer dst = cle.Create<float>(dimensions);

    // apply filter with parameters
    cle.AddImageAndScalar(src, dst, 10);  

    // pull output from GPU into vector
    std::vector<float> output = cle.Pull<float>(dst); 

    return EXIT_SUCCESS;
}
```
See more complete example on usage by looking at the [operations tests](https://github.com/clEsperanto/CLIc_prototype/tree/master/tests).

# Installation

## Requierements

CLIc rely on OpenCL language for parallelisation and CMake for configuration and build.  
Follow the [pre-requish installation guide](./prerequish.md) for a smooth and error-less installation. 

## Install

Clone the repository and submodule
```
git clone git@github.com:clEsperanto/CLIc_prototype.git CLIc
cd CLIc
git submodule update --init --recursive
```

Create a build folder and configure cmake to generate the adapted makefile
```
mkdir build && cd build
cmake .. -DPREFIX=/path/to/installation/folder
make
make test
make install
```

You can modify installation location using `-DPREFIX=/path/to/installation/folder` when calling cmake.

### Kernels
CLIc filters rely on the [CLIj OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels).

# Feedback welcome!
clEsperanto is developed in the open because we believe in the [open source community](https://clij.github.io/clij2-docs/community_guidelines). Feel free to drop feedback as [github issue](https://github.com/clEsperanto/CLIc_prototype/issues) or via [image.sc](https://image.sc)
