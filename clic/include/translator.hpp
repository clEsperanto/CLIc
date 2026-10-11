#pragma once

#include <string>

/// Translates OpenCL kernel source code to equivalent CUDA source code at runtime.
///
/// Supports:
///   - Kernel and function qualifiers (__kernel, inline, __global, __local, __constant, __private)
///   - Work-item functions (get_global_id, get_local_id, get_group_id, get_global_size, etc.)
///   - Vector type constructors: (typeN){...} → make_typeN(...)
///   - Synchronization primitives (barrier → __syncthreads)
///   - Atomic operations (atomic_add → atomicAdd, etc.)
///   - Type conversion functions (convert_int, convert_float, etc.)
///   - Math function differences (native_*, half_*)
///   - Sampler stubs
///   - Vector component access (.sN → .x/.y/.z/.w)
///
/// Limitations:
///   - Does not parse full AST; relies on text substitution
///   - OpenCL images (image2d_t, read_image*) are not translated: CUDA images are emulated as buffers
///   - Nested vector constructor braces may confuse brace matching
///   - OpenCL 2.0+ features (pipes, SVM, work_group_*) are not handled
class OpenCLToCUDATranslator
{
public:
  OpenCLToCUDATranslator() = default;
  ~OpenCLToCUDATranslator() = default;

  /// Translate a full OpenCL source string to CUDA. Returns new string.
  [[nodiscard]] auto
  translate(const std::string & openclSource) const -> std::string;

  /// Translate in-place.
  auto
  translateInPlace(std::string & code) const -> void;
};

/// Translates OpenCL kernel source code to equivalent Metal Shading Language (MSL)
/// source code at runtime.
///
/// Notes:
///   - This is a text-based translator (not AST based).
///   - Kernel signatures are augmented with Metal thread-position builtins so
///     get_global_id/get_local_id/get_group_id style calls can be rewritten.
///   - Image and macro preambles are expected to be handled by the caller.
///   - Atomic operations (atomic_add/min/max/etc. → atomic_fetch_*_explicit) are supported for
///     integer pointees; portable float atomics are expected to be authored in OpenCL C using a
///     compare-and-swap loop on the bit-reinterpreted value (as_uint/as_float), which this
///     translator supports via translateBitcast and a dedicated atomic_cmpxchg assignment rewrite
///     (translateCompareExchange) that reproduces OpenCL's return-old-value semantics.
///   - `printf(...)` calls are stripped (translateRemovePrintf): MSL's printf has a different
///     signature than OpenCL C's and is only used in upstream kernels as a workaround/debug aid,
///     so dropping the call is behavior-preserving for our kernels.
class OpenCLToMetalTranslator
{
public:
  OpenCLToMetalTranslator() = default;
  ~OpenCLToMetalTranslator() = default;

  [[nodiscard]] auto
  translate(const std::string & openclSource) const -> std::string;

  auto
  translateInPlace(std::string & code) const -> void;
};
