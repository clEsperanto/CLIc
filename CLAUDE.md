# CLAUDE.md

## What CLIc is

CLIc is the C++17 backend library of [clEsperanto](https://github.com/clEsperanto): GPU-accelerated image processing used by
pyclesperanto (Python) and clesperantoj (Java). It runs the CLIJ OpenCL kernels on **OpenCL** (default), **CUDA**, or **Metal**.
The backend is chosen at **configure time** (`CLE_BACKEND`). All public API lives in `namespace cle`.

> The usage example in `README.md` is outdated (`clesperanto.h`, `cle::Push`, `AddImageAndScalar`). Use `tests/` as the
> reference for the current API: `#include "cle.hpp"`, `cle::Array::create(...)`, `cle::tier1::add_image_and_scalar_func(...)`.

## Build & test

```bash
# Presets (cmake/presets/{linux,macos,windows}); build dir = build/<preset>
cmake --preset macos-ninja-multi                       # configure (Ninja Multi-Config)
cmake --build --preset macos-ninja-multi-release       # or -debug
ctest --preset macos-ninja-multi-release               # run all tests

# Manual configure, choosing a backend
cmake -S . -B build/x -DCLE_BACKEND=OPENCL   # OPENCL | CUDA | METAL (METAL requires Apple)
```

Running a subset of tests (one executable per suite: `test_core`, `test_tier1` … `test_tier8`):

```bash
ctest --test-dir build/macos-ninja-multi -C Release -R TestAbsoluteDifference --output-on-failure
ctest --test-dir build/macos-ninja-multi -C Release -L tier2          # by label (= suite name)
build/macos-ninja-multi/tests/tier2/Release/test_tier2 --gtest_filter='*AbsoluteDifference*'
ctest --test-dir build/macos-ninja-multi -C Release -L package       # install + find_package(CLIc) consumer
```

`tests/package/` is a standalone consumer project, not a test suite: the `package_*` tests (skipped with
`BUILD_COVERAGE`) install CLIc into the build tree and build it with `find_package(CLIc)`; `.github/workflows/package.yml`
also builds it with `-DCLIC_SOURCE_DIR=<repo>` (`add_subdirectory`). Run it after changing targets, link dependencies or
install rules: a public dependency must also be found in `cmake/CLIcConfig.cmake.in`.

Useful CMake options (see `cmake/CMakeSetEnv.cmake`): `BUILD_TESTING` (ON), `BUILD_SHARED_LIBS` (ON), `BUILD_COVERAGE` (OFF),
`CLEAR_KERNEL_CACHE` (OFF; wipes `~/.cache/clesperanto` at configure, or build the `clic_clear_cache` target).
`BUILD_BENCHMARK` exists, but `benchmark/` and `docs/` are **not** added by the top-level `CMakeLists.txt`.

CLIc is consumed as a subproject (FetchContent / `add_subdirectory`) by pyclesperanto and clesperantoj, both with
`BUILD_SHARED_LIBS=OFF`; clesperantoj also runs CLIc's install rules. `CLIC_IS_TOP_LEVEL` (root `CMakeLists.txt`) guards
everything that would change a parent project: cache variables, build type, `USE_FOLDERS`, tests (never built as a
subproject). Keep new options and variables `CLIC_`/`CLE_`-prefixed, prefer normal variables to cache entries, and check
changes to the CMake files against both bindings (build them with `-DFETCHCONTENT_SOURCE_DIR_CLIC=<this repo>`).

One build tree = one backend, and tests only run on the backend that was compiled in. To test another backend, use a
separate tree, e.g. Metal on a Mac:

```bash
cmake -S . -B build/metal -G Ninja -DCMAKE_BUILD_TYPE=Release -DCLE_BACKEND=METAL
cmake --build build/metal -j 18 && ctest --test-dir build/metal -j 8
```

Let each tree fetch its own dependencies: overriding `FETCHCONTENT_SOURCE_DIR_*` breaks the build, because the DLPack and
VkFFT include paths in `clic/CMakeLists.txt` are hard-coded under `${FETCHCONTENT_BASE_DIR}`.

To validate a change to the backend layer, run the full suite on every available backend **twice**: once with a warm disk
cache (exercises loading kernels from disk) and once with `CLESPERANTO_NO_CACHE=1` (compiles everything from source).
To prove a new regression test catches a bug, run it against the original code with `git stash push -- clic` (then
`git stash pop`).

For performance changes, compare the original and the modified library with `benchmark/benchmark_backend_overhead.cpp`
(procedure and reference numbers in `BACKEND_REVIEW_PLAN.md`, "Benchmark"). Test both `setWaitToFinish(false)` and
`(true)`: an optimisation for one mode can be a regression in the other (it happened with Metal blits).

Tests need a working GPU runtime. CI (`.github/workflows/tests_and_coverage.yml`) runs OpenCL on CPU through **pocl**.
`build-{opencl,cuda,metal}.yml` only check that the code compiles.

### Dependencies (FetchContent, `clic/thirdparty/CMakeLists.txt`)

Version tags are set in the root `CMakeLists.txt` (normal variables, override with `-D<name>=<tag>`).

| Dependency | Purpose |
|---|---|
| `clij-opencl-kernels` (`CLIC_KERNELS_TAG`) | OpenCL kernel sources, turned into headers `cle_<name>.h` with `constexpr const char* kernel::<name>`, plus `cle_preamble_{cl,cu,metal}.h` |
| Eigen (`CLIC_EIGEN_TAG`) | Host-side matrix math |
| VkFFT (`CLIC_VKFFT_TAG`) | GPU FFT, header-only, target `clic_vkfft`; `VKFFT_BACKEND` follows `CLE_BACKEND` (3=OpenCL, 1=CUDA, 5=Metal) |
| DLPack | Zero-copy tensor interop (`Array::toDLPack` / `fromDLPack`) |
| GoogleTest v1.14 | Tests, always built static |

The kernel sources are **not in this repo**. To read one, look in `build/<preset>/_deps/clekernels-src/` or
`_deps/clekernels-build/include/cle_<name>.h`. To change a kernel, change it upstream and bump `CLIC_KERNELS_TAG`.

## Code layout

```
clic/
  clic.hpp.in          -> configured to build/.../include/clic.hpp (USE_OPENCL/USE_CUDA/USE_METAL macros, backend headers)
  include/             public headers (one header per tier)
    cle.hpp            umbrella include (everything public)
    array.hpp          Array: the main data structure
    device.hpp         Device + OpenCLDevice / CUDADevice / MetalDevice
    backend.hpp        Backend (abstract) + OpenCL/CUDA/MetalBackend + BackendManager singleton
    execution.hpp      execute / execute_separable / native_execute / evaluate
    cache.hpp          ProgramCache (in-memory LRU, per device) + DiskCache singleton
    translator.hpp     OpenCLToCUDATranslator, OpenCLToMetalTranslator (text-based source translation)
    utils.hpp          dType/mType enums, promoteType, castTo, sigma2kernelsize, renderTemplate, DLPack type mapping
    tier0.hpp          destination allocation helpers (create_like, create_dst, create_one, ...)
    tier1..8.hpp       operations, cle::tierN namespaces
    fft.hpp            cle::fft (VkFFT: fft, ifft, convolve, deconvolve)
    transform.hpp      cle::transform (affine transforms)
    statistics.hpp     per-label statistics helpers
  src/                 implementations; src/tierN/*.cpp (one file per op or op family)
    *backend.cpp|.mm, *device.cpp|.mm, *Translator.cpp   backend-specific code (Metal = Objective-C++ .mm)
    backend_common.hpp private helpers shared by the backends (cle::detail, not installed)
tests/                 GoogleTest, tests/<core|tierN>/test_*.cpp, one executable per directory
cmake/                 SetEnv (options, C++17), Backends (find OpenCL/CUDA/Metal), Install, presets
benchmark/             standalone benchmarks (not wired into the root build)
BACKEND_REVIEW_PLAN.md status of the backend/device review (done / remaining items): read it before backend work
```

Sources are picked up with `GLOB_RECURSE`, so a new `.cpp` under `clic/src/` (or a new test under `tests/<dir>/`) is built
automatically. **Re-run the CMake configure step** after adding files.

## Core architecture

### Backend / Device
- `BackendManager::getInstance().setBackend("opencl"|"cuda"|"metal")` and then `.getBackend().getDevice(name, "gpu"|"cpu")`.
  `name` is a case-insensitive substring. With an empty or unmatched name, OpenCL returns the **last** device of the type,
  CUDA and Metal the **first**. This is intentional, don't unify it: OpenCL often lists a CPU device first, CUDA/Metal
  have no CPU device. OpenCL device indices restart at 0 per platform, also by design (it's the DLPack `device_id`).
  CUDA ignores the type filter; Metal returns nothing for `"cpu"`.
  Only the backend compiled in with `CLE_BACKEND` is usable.
- Backend-specific code is guarded with `#if USE_OPENCL / USE_CUDA / USE_METAL` (from `clic.hpp`). The compile definitions
  `CLE_OPENCL / CLE_CUDA / CLE_METAL` also exist. Every backend class has to keep compiling when its backend is off.
- `Backend` is an abstract interface (allocate/free, read/write/copy buffer & image, rect variants, `executeKernel`,
  `getPreamble`). Adding a capability means touching **all three** backend implementations and their headers in `backend.hpp`.
- Each backend creates its devices once (on first query) and returns the same `Device::Pointer` afterwards.
  Shared backend helpers (`findDeviceByName`, `visitDType`, `computeBlockSize`, `scaleToBytes`, `translateAroundPreamble`,
  `cachedBuild`) live in the private header `clic/src/backend_common.hpp` (namespace `cle::detail`, not installed). Use them
  instead of duplicating code across the three backends.
- Low-level memory calls (`writeMemory`, `readMemory`, `copyMemory*`, `setMemory`) take shapes, origins and regions **in
  elements** by non-const reference, and the backends **modify them in place** (x scaled to bytes). Pass copies.
- `Array::copyTo(dst)` (whole-array copy) describes a contiguous buffer with a **flat shape `{size, 1, 1}`** while the region
  stays `{w, h, d}`. Backend copy code must treat a flat shape as a packed buffer, not use `shape[0]` as the row pitch.
- `freeMemory` is a no-op: device memory is freed by the deleter of the `shared_ptr` from `allocateMemory`.
- Kernel arguments reach `executeKernel` as `args` (pointers to the values, or the device handle itself for memory) plus
  `sizes`. Backends recognise device memory by **size == pointer size**, so `size_t` parameters are narrowed to
  `unsigned int` in `marshalParameters`. Valid scalar parameter types: `float`, `int`, `unsigned int` (and `size_t`).
- Metal submits kernels without waiting: `MetalDevice::waitForCompletion()` must be called before any host access
  to buffer contents (the Metal read/write/copy/fill paths already do it). Wrap every `.mm` entry point that calls
  metal-cpp getters (`commandBuffer()`, `NS::String::string()`, `name()`, ...) in `@autoreleasepool { }` (in a `.cpp`,
  use an `NS::AutoreleasePool` guard, see `fft.cpp`): they return autoreleased objects and the host (C++ or Python) has
  no pool, so they would leak. **Never** open a pool around VkFFT's `initializeVkFFT` / `deleteVkFFT`: the Metal FFT
  segfaults when it is drained.
- Metal copies: GPU blit in asynchronous mode, host copy in synchronous mode (cheaper on unified memory). Host copies of
  contiguous data go in 4 KB chunks: one large `memcpy` on shared Metal buffers measured ~35% slower.
- The device hash used as cache key is `deviceHash(*device)` (computed once from `getInfo()`). `cachedBuild` keys the
  in-memory cache with `std::hash` (cheap, computed on every launch) and only computes the stable FNV-1a disk-cache
  hash on a miss: keep per-launch work out of the hot path.
- `Device::Pointer` is a `std::shared_ptr<Device>`. `device->setWaitToFinish(true)` makes calls synchronous, as the tests do.

### Array
- Always handled as `Array::Pointer` (`std::shared_ptr<Array>`). Constructors are private: use `Array::create(...)`,
  `Array::create(other)` (same shape, type and device, new memory), `reshape`, `shallow_copy`, `view`.
- Device memory is held in a `std::shared_ptr<void>` with a backend-specific deleter, which frees it automatically. Never free raw pointers.
- Shape is `width, height, depth` (x, y, z) plus `dim` (1–3). Host data is row-major with x fastest:
  `index = z*H*W + y*W + x`.
- `dType`: INT8/UINT8/INT16/UINT16/INT32/UINT32/FLOAT/COMPLEX (+UNKNOWN). Prefer the semantic aliases `LABEL`/`INDEX` (=UINT32),
  `BOOL` (=UINT8), `INT` (=INT32). No 64-bit or double types (Metal and GPU limits). `mType`: BUFFER (default) or IMAGE (OpenCL only;
  CUDA/Metal treat everything as buffers).
- Views (`view()`, non-zero offset or non-contiguous strides) are supported for read/write/copy, **but not as kernel
  parameters**: `execute` throws. Make the data contiguous first.
- Host I/O: `writeFrom(ptr)`, `readTo(ptr)` (with region/origin overloads), `copyTo(dst)`, `fill(value)`.

### Kernel execution pipeline (`src/execution.cpp`)
`execute(device, {name, source}, ParameterList, global_range, local_range, ConstantList)`:
1. Generates `#define`s for every array parameter, keyed by **parameter name**: `IMAGE_<key>_TYPE`, `READ_<key>_IMAGE`,
   `WRITE_<key>_IMAGE`, `IMAGE_SIZE_<key>_WIDTH/HEIGHT/DEPTH`, `POS_<key>_INSTANCE`, ... plus `USE_<DTYPE>` and `USE_<N>D`.
   This means the **ParameterList keys must match the kernel argument names exactly** (`"src"`, `"dst"`, `"src0"`, ...).
   For IMAGE memory, keys containing `dst`/`destination`/`output` become write-only.
2. Prepends the backend preamble (`getPreamble()`) and passes `defines + preamble + kernel` to `Backend::executeKernel`.
   The assembled source is cached per **launch signature** (kernel, backend, each array's name/dtype/mtype/shape/dim,
   constants): when changing what the defines depend on, add it to `launchSignature()` too. The signature is also
   passed to the backend as `cache_key` (optional last parameter of `executeKernel` / `buildKernel`) so it does not
   re-hash the source: a `cache_key` must always designate the same source. The generated text must stay
   byte-identical unless intended (it feeds the cache keys): see "Checking that generated kernel code is unchanged" in
   `BACKEND_REVIEW_PLAN.md`.
3. The backend compiles through a 3-level cache, shared by all backends in `detail::cachedBuild`
   (`src/backend_common.hpp`): **in-memory `ProgramCache`** (per device, LRU 128, holds the ready-to-launch kernel
   object) → **`DiskCache`** (`~/.cache/clesperanto/clic-<version>-r<CODEGEN_REVISION>/<device_hash>/<source_hash>.{bin|ptx|msl}`,
   atomic writes; an unusable entry is rebuilt; disable with `CLESPERANTO_NO_CACHE` or `cle::use_cache(false)`) → compile. For CUDA/Metal, only the part after the preamble is translated from OpenCL C (`OpenCLToCUDATranslator` →
   NVRTC/PTX, `OpenCLToMetalTranslator` → MSL). The preambles themselves are already native CUDA/MSL.

Other entry points:
- `execute_separable`: runs a 1D kernel along x, y, z in turn (gaussian and similar). Takes `sigma`, `radius`, `orders`.
- `native_execute`: raw kernel, no defines or preamble added.
- `evaluate(device, "a * x + b * y", {arrs/scalars...}, dst)`: builds a JIT element-wise kernel from an expression.
  Variables are bound by order of first appearance. Use it for simple arithmetic instead of writing a new kernel.

The translators work by **text substitution, not an AST**. When a kernel misbehaves only on CUDA or Metal, suspect the
translator first (`src/cudaTranslator.cpp`, `src/metalTranslator.cpp`) and check the limitations listed in `translator.hpp`.

## Operations and the tier system

- **tier0**: allocation helpers. `create_like`, `create_dst`, `create_one`, `create_vector`, `create_xy`/`yx`/..., and
  `create_or_check_broadcast_dst`. They allocate `dst` only when it is `nullptr`.
- **tier1**: a **direct kernel execution** (or an `evaluate` expression) built on `cle_<name>.h`.
- **tier2 … tier8**: composed **only from lower-tier functions**. Never include a kernel header or call `execute` there.
  A tier-N function may only call tiers `< N`.
- `fft`, `transform`, `statistics` are separate namespaces outside the tier numbering.

### Function conventions (follow exactly; wrappers rely on them)
Signature:
```cpp
auto
<snake_name>_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst, <scalars...>) -> Array::Pointer;
```
- Function names are `snake_case` with a `_func` suffix. `device` always comes first. Inputs are `const Array::Pointer &`.
- The output `dst` is passed **by value** (`Array::Pointer dst`) and may be `nullptr`. Allocate it with a tier0 helper and
  `return dst`.
- Scalars are `float`, `int`, `bool`, or `std::string`. Defaults are documented in the header comment, not in the C++ signature.

Typical tier1 implementation (`src/tier1/sobel.cpp`):
```cpp
#include "tier0.hpp"
#include "tier1.hpp"
#include "utils.hpp"
#include "cle_sobel.h"

namespace cle::tier1
{
auto
sobel_func(const Device::Pointer & device, const Array::Pointer & src, Array::Pointer dst) -> Array::Pointer
{
  tier0::create_like(src, dst, dType::FLOAT);
  const KernelInfo    kernel = { "sobel", kernel::sobel };
  const ParameterList params = { { "src", src }, { "dst", dst } };
  const RangeArray    range = { dst->width(), dst->height(), dst->depth() };
  execute(device, kernel, params, range);
  return dst;
}
} // namespace cle::tier1
```

Header doc block. This format is **parsed by downstream wrapper generators** (pyclesperanto, clesperantoj), so keep the
exact syntax:
```cpp
/**
 * @name add_images_weighted
 * @brief One-line description. <pre>formula</pre>
 *
 * @param device Device to perform the operation on. [const Device::Pointer &]
 * @param src0 First input image to add. [const Array::Pointer &]
 * @param dst Output image where results are written into. [Array::Pointer ( = None )]
 * @param factor1 Multiplication factor ... [float ( = 1 )]
 * @return Array::Pointer
 *
 * @note 'combine', 'in assistant'
 * @see https://clij.github.io/clij2-docs/reference_addImagesWeighted
 * @deprecated (optional) This function is deprecated. Consider using X instead.
 */
```
- `@name` is the function name without `_func`. Every `@param` ends with `[type ( = default )]`.
- `@note` holds the category tags (`'filter'`, `'label processing'`, `'in assistant'`, `'bia-bob-suggestion'`, ...).
  Reuse existing tags; grep the tier headers for them.

### Adding a new operation (checklist)
1. Pick the lowest tier that matches its dependencies.
2. Declare it with the doc block in `clic/include/tierN.hpp`.
3. Implement it in `clic/src/tierN/<name>.cpp`, or append it to an existing family file such as `math_unary_ops.cpp`.
4. Add `tests/tierN/test_<name>.cpp` (see below) and re-run configure.
5. Build and run the suite on the local backend.

## Tests

The pattern is parameterized over the compiled backends:
```cpp
#include "cle.hpp"
#include "test_utils.hpp"
#include <gtest/gtest.h>

class TestFoo : public ::testing::TestWithParam<std::string> { /* host std::array input/valid */ };

TEST_P(TestFoo, execute)
{
  cle::BackendManager::getInstance().setBackend(GetParam());
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);
  auto gpu_in = cle::Array::create(W, H, D, DIM, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_in->writeFrom(input.data());
  auto gpu_out = cle::tierN::foo_func(device, gpu_in, nullptr, ...);
  gpu_out->readTo(output.data());
  // EXPECT_EQ / EXPECT_NEAR against valid
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestFoo, ::testing::ValuesIn(getParameters()));
```

## Code style

- clang-format (`.clang-format`, Mozilla-based) with **ColumnLimit 140** and 2-space indent. The return type goes on its
  own line (`auto` / newline / `name(...) -> T`). Braces go on a new line after functions, classes and control statements.
  Pointers and references are written `const Type & x`. Consecutive declarations are aligned.
- Pre-commit (`.pre-commit-config.yaml`) runs clang-format (v23, `.mm` excluded) and whitespace/EOF hooks. pre-commit.ci
  pushes `style(pre-commit.ci): auto fixes` commits. Run `pre-commit run --files <changed>` before committing. If
  pre-commit/clang-format are not installed, `pip install --target <scratch dir> clang-format==23.1.0` and run
  `clang-format -i` on the changed C/C++ files (not `.mm`).
- Use trailing return types (`auto f(...) -> T`) and `[[nodiscard]]` on getters. Check null inputs with `Array::check_ptr`.
  Report errors with `std::runtime_error`/`std::invalid_argument`.
- Static builds use hidden symbol visibility. Don't depend on internal symbols being exported.

## Gotchas

- Changing `CLE_BACKEND` needs a fresh build directory, or at least a reconfigure, because `clic.hpp` is generated.
- A stale disk cache can hide translator or codegen changes (the key is the OpenCL source). **Bump `CODEGEN_REVISION`
  in `src/cache.cpp`** when changing generated code (defines, preambles, translators); while debugging, set
  `CLESPERANTO_NO_CACHE=1`.
- No CUDA toolkit on the dev Mac: CUDA code can only be type-checked, not run (procedure in `BACKEND_REVIEW_PLAN.md`:
  `clang++ -fsyntax-only` with headers from the `nvidia-cuda-*-cu12` pip wheels and a `clic.hpp` configured with
  `USE_CUDA 1`). Say so explicitly when CUDA changes were not executed. `fft.cpp` does not type-check against CUDA 12.9
  `nvrtc.h` (VkFFT 1.3.4 issue, pre-existing).
- `transform::affine_transform(..., interpolate=true)` catches any exception from the buffer→image copy and **silently**
  falls back to non-interpolated output: an image-copy bug shows up only as wrong values in `TestAffineTransform`.
- `cle::castTo(value, dtype)` does not convert to `dtype` (it returns a common type). Casting a negative float to an
  unsigned type is undefined behaviour, and backends disagree on the result: avoid it in tests.
- `cle::use_cache(false)` sets an environment variable, so it disables the disk cache for the rest of the test process.
- Metal requires Objective-C++ (`.mm`) and is macOS-only. CUDA images are emulated as buffers.
- Kernels compute in the destination type, so pick the `dst` dtype in tier0 deliberately (filters usually use `dType::FLOAT`,
  labels `dType::LABEL`, binary `dType::BOOL`). Use `promoteType` for binary ops.
- Third-party code (`_deps`, VkFFT) produces warnings that are silenced on Apple. Don't "fix" them in this repo.
- Files copied from Windows come back with every file mode set to 755 (and new files 700): `git diff --summary` shows
  hundreds of `mode change` entries. Restore 644 before committing.
- Local shell is zsh: variables holding several file names are not word-split (`$FILES` is one argument), and `ls` is
  aliased with icon glyphs. In scripts, list files with `find` and pass file names explicitly.
