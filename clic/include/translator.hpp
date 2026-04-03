#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

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
///   - Image/sampler stubs
///   - Vector component access (.sN → .x/.y/.z/.w)
///
/// Limitations:
///   - Does not parse full AST; relies on text substitution
///   - Complex image read/write may need manual intervention
///   - Nested vector constructor braces may confuse brace matching
///   - OpenCL 2.0+ features (pipes, SVM, work_group_*) are not handled
///
/// ToDo:
///   - Add support for convert_<type>_sat functions
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

private:
  // ── Replacement helpers ────────────────────────────────────────────

  /// Simple global find-and-replace (all occurrences).
  static auto
  replaceAll(std::string & str, const std::string & from, const std::string & to) -> void;

  /// Word-boundary-aware replacement: only matches 'from' when not surrounded
  /// by alphanumeric or underscore characters.
  static auto
  replaceWord(std::string & str, const std::string & from, const std::string & to) -> void;

  /// Suffix-boundary-aware replacement: only matches 'from' when not followed
  /// by an alphanumeric or underscore character (enforces word boundary at end only).
  static auto
  replaceSuffix(std::string & str, const std::string & from, const std::string & to) -> void;

  /// Replace OpenCL compound-literal vector constructors:
  ///   (typeN){ a, b, ... };  →  make_typeN( a, b, ... );
  /// Handles nested parentheses inside the braces.
  static auto
  replaceVectorConstructor(std::string & code, const std::string & openclCtor, const std::string & cudaCtor) -> void;

  /// Collapse runs of multiple spaces into a single space, in-place.
  static auto
  collapseSpaces(std::string & code) -> void;

  /// Collapse runs of 3+ newlines into exactly 2, in-place.
  static auto
  collapseNewlines(std::string & code) -> void;

  /// Replace clamp(a, b, c) with min(max(a, b), c) using parenthesis-matching.
  static auto
  replaceClamp(std::string & code) -> void;

  // ── Translation passes (applied in order) ──────────────────────────

  static auto
  translatePragmas(std::string & code) -> void;

  static auto
  translateSamplers(std::string & code) -> void;

  static auto
  translateQualifiers(std::string & code) -> void;

  static auto
  translateAddressSpaces(std::string & code) -> void;

  static auto
  translateWorkItemFunctions(std::string & code) -> void;

  static auto
  translateSynchronization(std::string & code) -> void;

  static auto
  translateVectorConstructors(std::string & code) -> void;

  static auto
  translateVectorAccess(std::string & code) -> void;

  static auto
  translateAtomics(std::string & code) -> void;

  static auto
  translateTypeConversions(std::string & code) -> void;

  static auto
  translateMathFunctions(std::string & code) -> void;

  static auto
  translateImageOperations(std::string & code) -> void;

  static auto
  translateMiscBuiltins(std::string & code) -> void;

  static auto
  cleanupDoubleQualifiers(std::string & code) -> void;
};

/// Translates OpenCL kernel source code to equivalent Metal Shading Language (MSL)
/// source code at runtime.
///
/// Notes:
///   - This is a text-based translator (not AST based).
///   - Kernel signatures are augmented with Metal thread-position builtins so
///     get_global_id/get_local_id/get_group_id style calls can be rewritten.
///   - Image and macro preambles are expected to be handled by the caller.
class OpenCLToMetalTranslator
{
public:
  OpenCLToMetalTranslator() = default;
  ~OpenCLToMetalTranslator() = default;

  [[nodiscard]] auto
  translate(const std::string & openclSource) const -> std::string;

  auto
  translateInPlace(std::string & code) const -> void;

private:
  static auto
  replaceAll(std::string & str, const std::string & from, const std::string & to) -> void;

  static auto
  replaceWord(std::string & str, const std::string & from, const std::string & to) -> void;

  static auto
  collapseSpaces(std::string & code) -> void;

  static auto
  collapseNewlines(std::string & code) -> void;

  static auto
  appendMetalBuiltinKernelArgs(std::string & code) -> void;

  static auto
  translateKernelScalarArgs(std::string & code) -> void;

  static auto
  translatePragmas(std::string & code) -> void;

  static auto
  translateSamplers(std::string & code) -> void;

  static auto
  translateQualifiers(std::string & code) -> void;

  static auto
  translateAddressSpaces(std::string & code) -> void;

  static auto
  translateVectorConstructors(std::string & code) -> void;

  static auto
  translateWorkItemFunctions(std::string & code) -> void;

  static auto
  translateSynchronization(std::string & code) -> void;

  static auto
  translateMathFunctions(std::string & code) -> void;

  static auto
  cleanupCode(std::string & code) -> void;
};
