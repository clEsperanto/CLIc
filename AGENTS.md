# CLIc - Agent Guidelines

## Project Overview

**CLIc** is a C++17 GPU image-processing framework providing a unified API over **OpenCL, CUDA, and Metal** backends. It is the C++ core of the [clEsperanto](https://github.com/clEsperanto) project, wrapped by:
- [pyclesperanto](https://github.com/clEsperanto/pyclesperanto) (Python)
- [clesperantoj](https://github.com/clEsperanto/clesperantoj_prototype) (Java)

Public API lives in `namespace cle` (and `cle::tierN`). Single umbrella header: `cle.hpp`.

## Repository Layout

```
clic/include/      Public headers (cle.hpp, array.hpp, device.hpp, backend.hpp,
                   execution.hpp, cache.hpp, fft.hpp, transform.hpp, statistics.hpp,
                   translator.hpp, utils.hpp, tier0.hpp ... tier8.hpp)
clic/src/          Implementations
  tier0.cpp        Memory utilities (create_dst, create_like, create_one, ...)
  tierN/           Tier N function implementations (grouped by topic, NOT one file per function)
  *backend.cpp     openclbackend.cpp, cudabackend.cpp, metalbackend.mm
  *device.cpp      opencldevice.cpp, cudadevice.cpp, metaldevice.mm
  *Translator.cpp  cudaTranslator.cpp / metalTranslator.cpp (convert OpenCL C to CUDA / MSL)
  array.cpp, execution.cpp, cache.cpp, backendmanager.cpp, fft.cpp,
  statistics.cpp, transform.cpp
tests/tierN/       GoogleTest tests, registered via add_clic_test_suite(test_tierN tierN)
tests/test_utils.hpp  Provides getParameters() -> backend list ("opencl"/"cuda"/"metal")
cmake/             CMakeSetEnv.cmake, CMakeBackends.cmake, presets/{linux,macos,windows}
CMakePresets.json  Top-level preset aggregator
benchmark/, docs/
```

> Note: a single `tierN/foo_ops.cpp` typically defines several related public functions (e.g. `math_unary_ops.cpp` contains `absolute_func`, `square_root_func`, `exponential_func`, ...). Look for existing grouping before creating a new file.

## Tier Architecture

Functions are layered. **Tier N may call any Tier < N** (not just N-1). Tier 0 is the foundation (memory helpers, no algorithms).

| Tier | Purpose                              | Typical content                                  |
|------|--------------------------------------|--------------------------------------------------|
| 0    | Memory management utilities          | `create_dst`, `create_like`, `create_one`        |
| 1    | Pixel-wise / basic ops               | arithmetic, trig, logical, copy, crop, filters base |
| 2    | Filtering / morphology               | convolution, erode/dilate, gaussian, hessian      |
| 3    | Feature detection / enhancement      | edges, thresholding, local maxima                 |
| 4    | Advanced filtering                   | bilateral, multi-scale gaussian                   |
| 5    | Reconstruction                       | inpainting, restoration                           |
| 6    | Segmentation utilities               | region operations                                 |
| 7    | Labeling / connected components      | CCL, label remapping                              |
| 8    | High-level analysis                  | statistics, feature extraction                    |

## Build, Test, Format

```bash
# Configure (default backend = OPENCL, also CUDA or METAL)
cmake -B build -S . -DCLE_BACKEND=OPENCL -DBUILD_TESTING=ON
# Or use a preset:
cmake --preset <preset-name>           # see CMakePresets.json

# Build
cmake --build build -j

# Run tests (tests are auto-discovered via gtest_discover_tests)
ctest --test-dir build --output-on-failure
ctest --test-dir build -R tier1 -j 4 -V        # filter by tier / name
```

- C++ standard: **C++17**. CMake >= 3.20. Version in `CMakeLists.txt` (`project(CLIc VERSION ...)`).
- Format: `clang-format -i <file>` (config in `.clang-format`, **requires clang-format 8.0.0**, BasedOnStyle Mozilla).
- Pre-commit hooks configured in `.pre-commit-config.yaml`.
- CI: GitHub Actions runs OpenCL / CUDA / Metal builds + Codecov.

## Core Types (quick reference)

```cpp
cle::Device::Pointer    // shared_ptr<Device>, obtained via BackendManager
cle::Array::Pointer     // shared_ptr<Array>, GPU memory + image metadata
cle::dType              // FLOAT, INT32, UINT8, ...
cle::mType              // BUFFER, IMAGE
```

Acquire a device:
```cpp
cle::BackendManager::getInstance().setBackend("opencl"); // or "cuda" / "metal"
auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
```

Allocate an array:
```cpp
auto arr = cle::Array::create(width, height, depth, /*dimension*/ 3,
                              cle::dType::FLOAT, cle::mType::BUFFER, device);
arr->writeFrom(host_ptr);
arr->readTo(host_ptr);
```

## Implementing a New Function

1. **Pick a tier** based on the highest tier it transitively depends on.
2. **Declare** in `clic/include/tierN.hpp` with a full Doxygen block (see template below). The public symbol uses the **`_func` suffix** in C++; the `@name` tag must omit it.
3. **Implement** in an existing `clic/src/tierN/<group>.cpp` if a related group exists; otherwise add a new `.cpp` and reference it in `clic/src/tierN/CMakeLists.txt`.
4. **Kernel source**: embedded as a `R"CLC( ... )CLC"` raw string in OpenCL C, using CLIc macros: `IMAGE_src_TYPE`, `READ_IMAGE`, `WRITE_IMAGE`, `POS_src_INSTANCE`, `CONVERT_dst_PIXEL_TYPE`, etc. Translators auto-convert to CUDA/Metal.
5. **Add test** in `tests/tierN/test_<name>.cpp` (the `add_clic_test_suite` macro picks it up automatically).
6. **Build & test** before declaring done.

### Canonical implementation pattern

```cpp
#include "tier0.hpp"
#include "tier1.hpp"
#include "utils.hpp"

namespace cle::tier1
{
namespace {
const char * kernel_source = R"CLC(
__kernel void my_kernel(IMAGE_src_TYPE src, IMAGE_dst_TYPE dst) {
  const int x = get_global_id(0), y = get_global_id(1), z = get_global_id(2);
  float v = (float) READ_IMAGE(src, sampler, POS_src_INSTANCE(x,y,z,0)).x;
  WRITE_IMAGE(dst, POS_dst_INSTANCE(x,y,z,0), CONVERT_dst_PIXEL_TYPE(OP(v)));
})CLC";
} // namespace

auto
my_op_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer
{
  tier0::create_like(src, dst);                                  // allocate dst if null
  const RangeArray    global_range = { dst->width(), dst->height(), dst->depth() };
  const KernelInfo    kernel       = { "my_kernel", kernel_source };
  const ParameterList params       = { { "src", src }, { "dst", dst } };
  const ConstantList  constants    = { { "OP(x)", "fabs(x)" } };  // optional macro substitutions
  execute(device, kernel, params, global_range, { 0, 0, 0 }, constants);
  return dst;
}
} // namespace cle::tier1
```

Public signature convention (header):
```cpp
auto function_name_func(const Device::Pointer & device,
                        const Array::Pointer &  src,
                        Array::Pointer          dst,
                        /* extra params with defaults */) -> Array::Pointer;
```

### Canonical test pattern

Tests are **parameterized over all enabled backends** via `getParameters()`:

```cpp
#include "cle.hpp"
#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestMyOp : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, N> input  = { /* ... */ };
  std::array<float, N> valid  = { /* ... */ };
  std::array<float, N> output;

  std::string                        backend;
  cle::Device::Pointer               device;

  virtual void
  SetUp()
  {
    std::string param = GetParam();
    cle::BackendManager::getInstance().setBackend(param);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestMyOp, execute)
{
  auto gpu_in  = cle::Array::create(W, H, D, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_in->writeFrom(input.data());
  auto gpu_out = cle::tier1::my_op_func(device, gpu_in, nullptr);
  gpu_out->readTo(output.data());
  for (size_t i = 0; i < output.size(); ++i) EXPECT_EQ(output[i], valid[i]);
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMyOp, ::testing::ValuesIn(getParameters()));
```

## Doxygen Convention (strict — drives wrapper auto-generation)

Required order: `@name`, `@brief`, `@param`s, `@return`. Optional: `@note`, `@see`, `@deprecated`.

Rules:
- `@name` = public name **without** `_func` suffix.
- Every `@param` line ends with a type annotation in brackets: `[const Array::Pointer &]`, `[int ( = 8 )]`, `[Array::Pointer ( = None )]`.
- Use `<pre>...</pre>` for formulas inside `@brief`.
- `@note` is a comma-separated, single-quoted tag list (e.g. `'filter', 'in assistant', 'bia-bob-suggestion'`) consumed by downstream tools.
- Link to the matching CLIJ reference via `@see` when one exists.

Reference example:
```cpp
/**
 * @name absolute
 * @brief Computes the absolute value of every individual pixel x in a given image. <pre>f(x) = |x|</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src    The input image to be processed. [const Array::Pointer &]
 * @param dst    Output image where results are written into. [Array::Pointer ( = None )]
 * @return Array::Pointer
 *
 * @note 'filter', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_absolute
 */
auto absolute_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst)
  -> Array::Pointer;
```

## Backend Notes

| Backend | Kernel language       | Translator             | Source files                            |
|---------|-----------------------|------------------------|-----------------------------------------|
| OpenCL  | OpenCL C              | minimal (passthrough)  | `openclbackend.cpp`, `opencldevice.cpp` |
| CUDA    | CUDA C++              | `cudaTranslator.cpp`   | `cudabackend.cpp`, `cudadevice.cpp`     |
| Metal   | Metal Shading Lang.   | `metalTranslator.cpp`  | `metalbackend.mm`, `metaldevice.mm`     |

Kernels are written **once in OpenCL C** with CLIc macros; translators rewrite them for CUDA/Metal at runtime. Compiled kernels are cached (`cache.cpp`).

## Dependencies

- **Eigen** 3.4.0 (linear algebra) — pinned via `eigen_lib_version_tag` in `CMakeLists.txt`
- **vkFFT** 1.3.4 (FFT)
- **Google Test** (testing)
- Backend SDKs: OpenCL / CUDA / Metal (Metal: macOS only)
- Kernel repository tag: `clesperanto_kernels` (`kernel_version_tag`)

## Debugging Quick Hits

- Kernel compile error → inspect the relevant `*Translator.cpp` output; ensure macros (`IMAGE_*`, `POS_*`, `CONVERT_*`) are used correctly.
- Wrong result → check `global_range` matches `dst` dimensions; verify `ParameterList` names match kernel argument names.
- Flaky / backend-specific failure → re-run with `device->setWaitToFinish(true)` (already used in tests) and `ctest -R <name> -V`.
- Memory issue → all `Array`s are managed via `shared_ptr`; do not call raw `delete`.

## Documentation

- Main docs: https://clesperanto-doc.readthedocs.io/
- Kernels repo: https://github.com/clEsperanto/clij-opencl-kernels
- CLIJ reference (for `@see` links): https://clij.github.io/

## Agent Working Rules

- **Do not duplicate function logic**: check for an existing helper in `tier0` or a grouped `*_ops.cpp` before adding a new file.
- **Keep public signatures stable**: changing a `tierN.hpp` declaration ripples into pyclesperanto / clesperantoj auto-generation.
- **Always update both** header declaration (with full Doxygen) and the implementation.
- **Always add or update tests** under `tests/tierN/`; they are picked up automatically — no CMake edit unless creating a new tier directory.
- **Run** `cmake --build build` and the relevant `ctest -R` filter before reporting completion.
- **Format** modified C++ files with `clang-format -i` (v8.0.0).
