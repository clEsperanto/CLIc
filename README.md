# CLIc
[![Build OpenCL](https://github.com/clEsperanto/CLIc/actions/workflows/build-opencl.yml/badge.svg)](https://github.com/clEsperanto/CLIc/actions/workflows/build-opencl.yml)
[![Build CUDA](https://github.com/clEsperanto/CLIc/actions/workflows/build-cuda.yml/badge.svg)](https://github.com/clEsperanto/CLIc/actions/workflows/build-cuda.yml)
[![CI-Tests](https://github.com/clEsperanto/CLIc/actions/workflows/tests_and_coverage.yml/badge.svg)](https://github.com/clEsperanto/CLIc/actions/workflows/tests_and_coverage.yml)
[![Codecov](https://codecov.io/gh/clEsperanto/CLIc/branch/master/graph/badge.svg?token=QRSZHYDFIF)](https://codecov.io/gh/clEsperanto/CLIc)
[![License](https://img.shields.io/badge/license-BSD-informational)](https://github.com/clEsperanto/CLIc/blob/master/LICENSE)
[![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/clesperanto/CLIc?color=white&include_prereleases)](https://github.com/clEsperanto/CLIc/releases/latest)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.6075345.svg)](https://doi.org/10.5281/zenodo.6075345)

__CLIc__ is a backend library for [clEsperanto](https://github.com/clEsperanto) - a multi-language framework for GPU-accelerated image processing. It uses the [OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels/tree/clesperanto_kernels) originally developped for [CLIJ](https://clij.github.io/). It currently focussed on running with [OpenCL](https://github.com/KhronosGroup/OpenCL-CLHPP), and also propose a [CUDA](https://developer.nvidia.com/cuda/toolkit) compatibility.

The library is used for various user-oriented API libraries:
- :snake: [pyclesperanto](https://github.com/clEsperanto/pyclesperanto)
- :coffee: [clesperantoj](https://github.com/clEsperanto/clesperantoj_prototype)
- Fiji plugin - [clij3](https://github.com/clEsperanto/clij3)

# Usage example

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

# Documentation

A detail [documentation](https://clesperanto-doc.readthedocs.io/en/latest/) of the library, installation, and API is available.
If any information is missing, is incorrect, or you are facing any kind of difficulties, do not hesitate to raise an [issue](https://github.com/clEsperanto/CLIc/issues).

# Contributing
Contributions, of any kind, are very welcome. Before spending effort on coding and filing a pull-request, please get in touch with us, [file an issue](https://github.com/clEsperanto/CLIc/issues), and let's discuss your potential contribution.
More information on how to add new kernels to the library can be found in the [documentation](https://clesperanto.github.io/CLIc/master/en/doc/functions.html).

# Feedback welcome!
[clEsperanto](https://github.com/clEsperanto) is developed in the open because we believe in the [open source community](https://clij.github.io/clij2-docs/community_guidelines).
Feel free to drop feedback as [github issue](https://github.com/clEsperanto/CLIc/issues) or via [image.sc](https://image.sc).
And if you liked our work, star the repository, share it with your friends, and use it to make cool stuff!

# Acknowledgements

We acknowledge support by the Deutsche Forschungsgemeinschaft under Germany’s Excellence Strategy (EXC2068) Cluster of Excellence Physics of Life of TU Dresden and by the Institut Pasteur, Paris. This project has been made possible in part by grant number 2021-237734 ([GPU-accelerating Fiji and friends using distributed CLIJ, NEUBIAS-style, EOSS4](https://chanzuckerberg.com/eoss/proposals/gpu-accelerating-fiji-and-friends-using-distributed-clij-neubias-style/)) from the Chan Zuckerberg Initiative DAF, an advised fund of the Silicon Valley Community Foundation, and by support from the French National Research Agency via the [France BioImaging research infrastructure](https://france-bioimaging.org/) (ANR-24-INBS-0005 FBI BIOGEN).