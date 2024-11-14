# CLIc

[![Build & test](https://github.com/clEsperanto/CLIc/actions/workflows/build-test.yml/badge.svg)](https://github.com/clEsperanto/CLIc/actions/workflows/build-test.yml)
[![CI-Tests](https://github.com/clEsperanto/CLIc/actions/workflows/tests_and_coverage.yml/badge.svg)](https://github.com/clEsperanto/CLIc/actions/workflows/tests_and_coverage.yml)
[![Codecov](https://codecov.io/gh/clEsperanto/CLIc/branch/master/graph/badge.svg?token=QRSZHYDFIF)](https://codecov.io/gh/clEsperanto/CLIc)
[![License](https://img.shields.io/badge/license-BSD-informational)](https://github.com/clEsperanto/CLIc/blob/master/LICENSE)
[![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/clesperanto/CLIc?color=white&include_prereleases)](https://github.com/clEsperanto/CLIc/releases/latest)
[![CppStd](https://img.shields.io/badge/cpp--std-c%2B%2B17-blue)](https://en.cppreference.com/w/cpp/17)
[![OpenCL](https://img.shields.io/badge/OpenCL-3.0-green)](https://www.khronos.org/opencl/)
[![Website](https://img.shields.io/website?url=http%3A%2F%2Fclesperanto.net)](http://clesperanto.net)
[![GitHub issues](https://img.shields.io/github/issues-raw/clEsperanto/CLIc)](https://github.com/clEsperanto/CLIc/issues)
[![GitHub stars](https://img.shields.io/github/stars/clEsperanto/CLIc?style=social)](https://github.com/clEsperanto/CLIc)
[![GitHub forks](https://img.shields.io/github/forks/clEsperanto/CLIc?style=social)](https://github.com/clEsperanto/CLIc)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.6075345.svg)](https://doi.org/10.5281/zenodo.6075345)

CLIc is a **prototype** backend library for [CLesperanto](https://github.com/clEsperanto) - a multi-language framework for GPU-accelerated image processing. It uses the [OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels/tree/clesperanto_kernels) originally developped for [CLIJ](https://clij.github.io/). It currently focussed on running with [OpenCL](https://github.com/KhronosGroup/OpenCL-CLHPP).

The library is used for various user-oriented API libraries:
- :snake: [pyclesperanto](https://github.com/clEsperanto/pyclesperanto)
- :coffee: [clesperantoj](https://github.com/clEsperanto/clesperantoj_prototype)
- Fiji plugin - [clij3](https://github.com/clEsperanto/clij3)

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

# __Documentation__

A detail documentation is available at [https://clesperanto.github.io/CLIc](https://clesperanto.github.io/CLIc/master/en/index.html).
If any information is missing or is incorrect, do not hesitate to raise an issue.

# __Contributing__
Contributions are very welcome. Before spending effort on coding and filing a pull-request, please get in touch with us, [file an issue](https://github.com/clEsperanto/CLIc/issues), and let's discuss your potential contribution.
More information on how to add new kernels to the library can be found in the [documentation](https://clesperanto.github.io/CLIc/master/en/doc/functions.html).

# __Feedback welcome!__
clEsperanto is developed in the open because we believe in the [open source community](https://clij.github.io/clij2-docs/community_guidelines).
Feel free to drop feedback as [github issue](https://github.com/clEsperanto/CLIc/issues) or via [image.sc](https://image.sc).
And if you liked our work, star the repository, share it with your friends, and use it to make cool stuff!

## Acknowledgements

We acknowledge support by the Deutsche Forschungsgemeinschaft under Germany’s Excellence Strategy (EXC2068) Cluster of Excellence Physics of Life of TU Dresden.
This project has been made possible in part by grant number 2021-237734 ([GPU-accelerating Fiji and friends using distributed CLIJ, NEUBIAS-style, EOSS4](https://chanzuckerberg.com/eoss/proposals/gpu-accelerating-fiji-and-friends-using-distributed-clij-neubias-style/)) from the Chan Zuckerberg Initiative DAF, an advised fund of the Silicon Valley Community Foundation.
