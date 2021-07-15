![CI-Build](https://github.com/clEsperanto/CLIc_prototype/workflows/CI-Build/badge.svg) [![CI-Tests](https://github.com/clEsperanto/CLIc_prototype/actions/workflows/tests_and_coverage.yml/badge.svg)](https://github.com/clEsperanto/CLIc_prototype/actions/workflows/tests_and_coverage.yml) [![codecov](https://codecov.io/gh/clEsperanto/CLIc_prototype/branch/master/graph/badge.svg?token=QRSZHYDFIF)](https://codecov.io/gh/clEsperanto/CLIc_prototype)
# CLIc

CLIc is a **prototype** for [CLesperanto](https://github.com/clEsperanto) - a multi-language framework for GPU-accelerated image processing. It uses [OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels/tree/development/src/main/java/net/haesleinhuepf/clij/kernels) from [CLIJ](https://clij.github.io/)

Right now, this is very preliminary, and mainly focussed on running a few kernel using the OpenCL C++ API.

# Usage example

```c++
#include "CLE.h"  // CLIc include

int main( int argc, char** argv)
{
    cle::GPU gpu();       // Initialise GPU 
    cle::CLE cle(gpu);    // Initialise cle

    // store data to process in vector
    unsigned int dimensions[3] = {width, height, depth};
    std::vector<float> input [width * height * depth]; 

    /*
     * ... fill input with data to process  
     */

    // push data into GPU, and create the output into GPU
    cle::Buffer src = cle.Push<float>(input, dimensions);

    // create output GPU object and apply filter
    cle::Buffer dst = cle.Create<float>(dimensions);
    cle.AddImageAndScalar(src, dst, 10);  

    // get output from filter into vector
    Image<float> output = cle.Pull<float>(dst); 

    return EXIT_SUCCESS;
}
```
See more complete example on usage by looking at the [validation tests](https://github.com/clEsperanto/CLIc_prototype/tree/master/test).

# Installation

## Requierements

CLIc rely on OpenCL language for parallelisation and CMake for configuration and build.  
Follow the [pre-requish installation guide](./prerequish.md) for a smooth and error-less installation. 

## Install

Clone the repository and submodule
```
git clone git@github.com:clEsperanto/CLIc_prototype.git CLIc
cd CLIc_prototype
git submodule update --init --recursive
```

Create a build folder and configure cmake to generate the adapted makefile
```
mkdir build && cd build
cmake ..
make
make test
make install
```

You can modify installation location using `-DPREFIX=/installation/path` when calling cmake.  
Other configuration variable available through cmake configuration (for advance user). 

### Kernels
CLIc filters rely on the [CLIj OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels).

# Feedback welcome!
clEsperanto is developed in the open because we believe in the [open source community](https://clij.github.io/clij2-docs/community_guidelines). Feel free to drop feedback as [github issue](https://github.com/clEsperanto/CLIc_prototype/issues) or via [image.sc](https://image.sc)
