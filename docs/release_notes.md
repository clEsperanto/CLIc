# 0.5.3 - On-going
## New features
- add new kernel operations [[#71](https://github.com/clEsperanto/CLIc_prototype/pull/71)]
  - binary operation and, or, xor, not, subtract
  - histogram
  - threshold otsu
  - mask
  - masked voronoi labeling
  - extend labeling via voronoi
  - voronoi otsu labeling
- Add Constant variable declaration.
  - This allows to define a constant value accessible by all threads executing the same kernel.
- Add GPU method to list all devices
## Miscellaneous
- Fixing some warning [[#71](https://github.com/clEsperanto/CLIc_prototype/pull/71)]
- Rename thirdparty submodules
  - clij-opencl-kernels => clesperanto-kernels
  - add opencl-headers
  - regroupe opencl submodules into opencl folder
- Consolidation of CMakeList.txt
- Renaming `labelling` into `labeling`

# 0.5.0 - February 14th 2022
## Known issues
- Incompatiblity using Image with kernel [cleBlockEnumerateKernel](https://github.com/clEsperanto/CLIc_prototype/blob/master/clic/includes/tier1/cleBlockEnumerateKernel.hpp).
  - Kernel must use Buffer memory object or will fail, possible issue in kernel code.
- `Object::Image` only works with `float` data type for now.
## Bug fixes
- Remove extra context manager class left in Tier1 folder.
## Enhancement
- Improve error management
  - try/catch blocks for OpenCL error management.
  - throw runtime_error if fail during execution.
  - OpenCL error code to string translation, improve debuging.
- Rework of GPU branch ([#52](https://github.com/clEsperanto/CLIc_prototype/pull/52)).
    - Remove all Manager classes into single GPU class.
    - Force 1 Platform - 1 Device - 1 Context - 1 Command Queue instance. If dealing with multiple GPUs, the current usage is to declare multiple instance of CLE.
    - `Push`, `Pull`, `Create`, and other low-level operation are now done at the GPU class level.
- Rework of data branch
    - Creation of `Object` which replace `Buffer` or `Image` class.
    - `Object` class hold a `cl::Memory` which can be a `cl::Buffer`, `cl::Image1D`, `cl::Image2D`, `cl::Image3D`.
    - Kernel I/O is now `Object`, no matter if its holding a Buffer or an Image. 
    - Remove `shared_ptr` usage in favor of reference/copy.
    - `Object` holding `Buffer` or `Image` can swapped seamlessly, at user will.
    - Reset method, allow to free memory hold in `Object`, to be handle carefully.
- Extension of buffer to 1d compatibility ([#57](https://github.com/clEsperanto/CLIc_prototype/issues/57))
- Extension of CLIJ kernels to nd clEsperanto kernels ([#16](https://github.com/clEsperanto/clij-opencl-kernels/issues/17)).
- Rework how `GlobalNDRange` is defined ([#69](https://github.com/clEsperanto/CLIc_prototype/issues/69)). 
  - Default, is set to `dst.Shape()` before `enqueueNDRange()` is call. 
  - `SetGlobalNDRange()` methods is available at kernel class level for defining specific range.
- Rework of kernels test to cover as much as possible usage (shape, data-type, object-type, etc.).
## New features  
- Introduce library core class unity test.
- Introduce new data class `Object` holding a cl::Memory, generic OpenCL holder for (`cl_buffer`,`cl_image1d`,`cl_image2d`,`cl_image3d`) ([#52](https://github.com/clEsperanto/CLIc_prototype/pull/52)). 
## Miscellaneous
- Replace `gcovr` by `lcov` for coverage report generation to code-cov.
- Consolidation of CMake code ([#53](https://github.com/clEsperanto/CLIc_prototype/pull/53))
- Fixing MSVC warning ([#54](https://github.com/clEsperanto/CLIc_prototype/pull/54), [#56](https://github.com/clEsperanto/CLIc_prototype/pull/56))
- Increase project OpenCL-CLHPP flexibility ([#55](https://github.com/clEsperanto/CLIc_prototype/pull/55))
- Add documentation comments in core classes.
# 0.4.0 - August 20th 2021
## Bug fixes
- Fixe display compiling log when error occur during program build.
## New features  
- Rework of the Data classes (`LightObject`, `Int`, `Float`, `Buffer`) into (`LightObject`, `Scalar<T>`, `Buffer`).
- Add new type for `Scalar` and `Buffer`: float, double, int, unsigned int, char, unsigned char, short, unsigned short.
- General speed up through smart pointer usage.
- Integrate `GPU` class in `clEsperanto` class.
- Add `CustomKernel` class to run kernel from source (not integrated into `clEsperanto` class, must be run from lower level).
- Improve error message details during kernel construction and compilation.
## Miscellaneous
- Rename `.h` extension to `.hpp` for the library header. Auto-generation of OpenCL Header file remain in `.h`.
- Rename `CLE` class into `clEsperanto` class for clarity.
# 0.3.0 - July 23th 2021
## Bug fixes
- Fix issue of `#include <cl2.hpp>` vs `#include <opencl.hpp>` ([#37](https://github.com/clEsperanto/CLIc_prototype/issues/37))
- Fix cross-platform OpenCL include `#include <opencl.hpp>` vs `#include <CL/opencl.hpp>` vs `#include <OpenCL/opencl.hpp>`
## New features  
- OpenCL kernels are now stored included into header instead of behind read at execution time. ([#32](https://github.com/clEsperanto/CLIc_prototype/issues/32))
- Rework of CMake project configuration
  - CMake search for OpenCL system installation: `find_package(OpenCL REQUIRED)`
  - Use [OpenCL API C++ Binding](https://github.com/KhronosGroup/OpenCL-CLHPP) submodule if not installed on system.
  - Run Python script `stringify.py` at build time for converting `.cl` into `.h`
  - Run Python script `autogeneration_script.py` at build time for generating repetitive code (e.g. `cleKernelList.h`)
  - New configuration options for building test, benchmark, code coverage, and documentations
  - Define OpenCL standard version to use (Default: 1.2)
- Rewrite of installation documentation
- Organising filters into Tiers folders
- `cleGPU` now display information on all platform and device or on specific device.
- User can now select specific device based on name.
## Miscellaneous
- Split library tests into two sets, low level for testing library classes, methods, and functions (developer oriented, unity tests, etc.), and high level to test get-way operations (user oriented, clEsperanto functionalities, etc.).
- Remove 2D and 3D from function call name. Dimensionality is defined by 3rd dimension parameters not equal to 0. 
# 0.2.0 - February 1st 2020 (not released)
## Bug fixes
- Fix memory leak from GPU due to a wrong object deletion
## New features
- CI github actions for testing build
- Generic benchmark for execution speed comparison between two filters
- `cleGPU` now allows save and reload compiled kernel. Avoid recompiling source if not needed. 
- Implementation of complex filters:
  - block_enumerate
  - maximum_of_all_pixels
  - minimum_of_all_pixels
  - sum_of_all_pixels
  - difference_of_Gaussian
  - close_index_gaps_in_label_map
  - connected_component_labelling_box
## Miscellaneous
- Rework on Manager classes, now relying on [OpenCL API C++ Binding](https://github.com/KhronosGroup/OpenCL-CLHPP).
# 0.1.0 - November 30th 2020
First release for I2K2020 Tutorial.
## New features
- Setup of CMake configuration
- Creation of the core classes
  * Manager classes: `cleGPU`, `clePlatformManager`, `cleDeviceManager`, `cleContextManager`, `cleCommandQueueManager` 
  * Data and filters classes: `cleBuffer`, `cleKernel`
  * DataType classes: `cleLightObject`, `cleInt`, `cleFloat`
  * Data operations: `Pull`, `Push`, `Create`
- Creation of a Gateway class for users `CLE`
- Implementation of several basic filters
  * absolute
  * add_image_and_scalar
  * add_images_weighted
  * copy
  * detect_maxima
  * dilate_sphere
  * equal
  * equal_constant
  * erode_sphere
  * flag_existing_labels
  * gaussian_blur_separable
  * greater
  * greater_constant
  * greater_or_equal
  * greater_or_equal_constant
  * maximum_separable
  * maximum_x_projection
  * maximum_y_projection
  * maximum_z_projection
  * mean_separable
  * mean_sphere
  * minimum_separable
  * minimum_x_projection
  * minimum_y_projection
  * minimum_z_projection
  * nonzero_minimum_box
  * not_equal
  * not_equal_constant
  * replace_intensities
  * replace_intensity
  * set
  * set_column
  * set_nonzero_pixels_to_pixelindex
  * smaller
  * smaller_constant
  * smaller_or_equal
  * smaller_or_equal_constant
  * sobel
  * sum_reduction
  * sum_x_projection
  * sum_y_projection
  * sum_z_projection
