#pragma once

#include <string>
#include <vector>
#include <regex>
#include <functional>
#include <unordered_map>

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
///   - Does not parse full AST; relies on regex/text substitution
///   - Complex image read/write may need manual intervention
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

private:
  // ── Replacement helpers ────────────────────────────────────────────

  /// Simple global find-and-replace (all occurrences).
  static auto
  replaceAll(std::string & str, const std::string & from, const std::string & to) -> void;

  /// Regex-based replacement (all matches).
  static auto
  regexReplaceAll(std::string & str, const std::regex & pattern, const std::string & replacement) -> void;

  /// Replace OpenCL compound-literal vector constructors:
  ///   (typeN){ a, b, ... };  →  make_typeN( a, b, ... );
  /// Handles nested parentheses inside the braces.
  static auto
  replaceVectorConstructor(std::string & code, const std::string & openclCtor, const std::string & cudaCtor) -> void;

  // ── Translation passes (applied in order) ──────────────────────────

  /// Remove or comment-out preprocessor directives that are OpenCL-specific.
  static auto
  translatePragmas(std::string & code) -> void;

  /// Translate sampler declarations and constants.
  static auto
  translateSamplers(std::string & code) -> void;

  /// Translate kernel and function qualifiers.
  static auto
  translateQualifiers(std::string & code) -> void;

  /// Translate address-space qualifiers (__global, __local, __constant, __private).
  static auto
  translateAddressSpaces(std::string & code) -> void;

  /// Translate work-item / work-group query functions.
  static auto
  translateWorkItemFunctions(std::string & code) -> void;

  /// Translate synchronization primitives.
  static auto
  translateSynchronization(std::string & code) -> void;

  /// Translate vector type constructors.
  static auto
  translateVectorConstructors(std::string & code) -> void;

  /// Translate vector component access (.s0–.sf → .x/.y/.z/.w).
  static auto
  translateVectorAccess(std::string & code) -> void;

  /// Translate atomic operations.
  static auto
  translateAtomics(std::string & code) -> void;

  /// Translate type conversion functions (convert_*).
  static auto
  translateTypeConversions(std::string & code) -> void;

  /// Translate math functions that differ between OpenCL and CUDA.
  static auto
  translateMathFunctions(std::string & code) -> void;

  /// Translate image read/write operations (basic support).
  static auto
  translateImageOperations(std::string & code) -> void;

  /// Translate miscellaneous built-in functions.
  static auto
  translateMiscBuiltins(std::string & code) -> void;

  /// Fix double-application issues (e.g., __device__ __device__).
  static auto
  cleanupDoubleQualifiers(std::string & code) -> void;
};