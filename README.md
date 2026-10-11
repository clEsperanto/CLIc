# CLIc
[![Build OpenCL](https://github.com/clEsperanto/CLIc/actions/workflows/build-opencl.yml/badge.svg)](https://github.com/clEsperanto/CLIc/actions/workflows/build-opencl.yml)
[![Build CUDA](https://github.com/clEsperanto/CLIc/actions/workflows/build-cuda.yml/badge.svg)](https://github.com/clEsperanto/CLIc/actions/workflows/build-cuda.yml)
[![Build Metal](https://github.com/clEsperanto/CLIc/actions/workflows/build-metal.yml/badge.svg)](https://github.com/clEsperanto/CLIc/actions/workflows/build-metal.yml)
[![CI-Tests](https://github.com/clEsperanto/CLIc/actions/workflows/tests_and_coverage.yml/badge.svg)](https://github.com/clEsperanto/CLIc/actions/workflows/tests_and_coverage.yml)
[![Codecov](https://codecov.io/gh/clEsperanto/CLIc/branch/master/graph/badge.svg?token=QRSZHYDFIF)](https://codecov.io/gh/clEsperanto/CLIc)
[![License](https://img.shields.io/badge/license-BSD-informational)](https://github.com/clEsperanto/CLIc/blob/master/LICENSE)
[![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/clesperanto/CLIc?color=white&include_prereleases)](https://github.com/clEsperanto/CLIc/releases/latest)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.6075345.svg)](https://doi.org/10.5281/zenodo.6075345)

__CLIc__ is a backend library for [clEsperanto](https://github.com/clEsperanto) - a multi-language framework for GPU-accelerated image processing. It relies on [OpenCL kernels](https://github.com/clEsperanto/clij-opencl-kernels/tree/clesperanto_kernels) originally developped for [CLIJ](https://clij.github.io/) and can run on [OpenCL](https://www.khronos.org/opencl/), [CUDA](https://developer.nvidia.com/cuda-zone), or [Metal](https://developer.apple.com/metal/).

The library is used for various user-oriented API libraries:
- :snake: [pyclesperanto](https://github.com/clEsperanto/pyclesperanto)
- :coffee: [clesperantoj](https://github.com/clEsperanto/clesperantoj_prototype)

# Documentation

A detail [documentation](https://clesperanto-doc.readthedocs.io/en/latest/) of the library, installation, and API is available.
If any information is missing, is incorrect, or you are facing any kind of difficulties, do not hesitate to raise an [issue](https://github.com/clEsperanto/CLIc/issues).

# Usage example

```c++
#include "cle.hpp" // CLIc umbrella include

#include <cstdlib>
#include <iostream>
#include <vector>

int main(int argc, char ** argv)
{
  // Select the backend compiled into the library ("opencl", "cuda" or "metal")
  // and get a device: the name is a case-insensitive substring, "" picks a default one
  cle::BackendManager::getInstance().setBackend("opencl");
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  std::cout << device->getName() << std::endl;

  // host vectors holding input and output data (x is the fastest axis)
  const size_t       width = 64, height = 64, depth = 1;
  std::vector<float> input(width * height * depth, 1.0f);
  std::vector<float> output(input.size());

  // allocate a 2D float array on the device and push the host data to it
  auto gpu_src = cle::Array::create(width, height, depth, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_src->writeFrom(input.data());

  // apply operations; passing nullptr as destination lets the function allocate it
  auto gpu_tmp = cle::tier1::add_image_and_scalar_func(device, gpu_src, nullptr, 10);
  auto gpu_dst = cle::tier1::gaussian_blur_func(device, gpu_tmp, nullptr, 2, 2, 0);

  // pull the result back from the device to the host
  gpu_dst->readTo(output.data());

  return EXIT_SUCCESS;
}
```
Device memory is released automatically when the last `cle::Array::Pointer` referencing it goes out of scope.
See more complete examples of usage by looking at the [tests](./tests/).

# Feedback & Contribution

[clEsperanto](https://github.com/clEsperanto) is developed in the open because we believe in the [open source community](https://clij.github.io/clij2-docs/community_guidelines).
Feedback and contributions, of any kind, are very much welcome. Do not hesitate to contact us via [github issue](https://github.com/clEsperanto/CLIc/issues) or [image.sc](https://image.sc).
And if you liked our work, star the repository, share it with your friends, and use it to make cool stuff!

# Acknowledgements

We acknowledge support by the Deutsche Forschungsgemeinschaft under Germany’s Excellence Strategy (EXC2068) Cluster of Excellence Physics of Life of TU Dresden and by the Institut Pasteur, Paris. This project has been made possible in part by grant number 2021-237734 ([GPU-accelerating Fiji and friends using distributed CLIJ, NEUBIAS-style, EOSS4](https://chanzuckerberg.com/eoss/proposals/gpu-accelerating-fiji-and-friends-using-distributed-clij-neubias-style/)) from the Chan Zuckerberg Initiative DAF, an advised fund of the Silicon Valley Community Foundation, and by support from the French National Research Agency via the [France BioImaging research infrastructure](https://france-bioimaging.org/) (ANR-24-INBS-0005 FBI BIOGEN).
