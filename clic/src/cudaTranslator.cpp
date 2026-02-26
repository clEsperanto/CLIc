#include "translator.hpp"

#include <algorithm>
#include <cassert>
#include <sstream>

// ═══════════════════════════════════════════════════════════════════════════
//  Public API
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::translate(const std::string & openclSource) const -> std::string
{
  std::string code = openclSource;
  translateInPlace(code);
  return code;
}

auto
OpenCLToCUDATranslator::translateInPlace(std::string & code) const -> void
{
  // Order matters — some passes depend on results of earlier ones.
  translatePragmas(code);
  translateSamplers(code);
  translateImageOperations(code);
  translateQualifiers(code);
  translateAddressSpaces(code);
  translateWorkItemFunctions(code);
  translateSynchronization(code);
  translateVectorConstructors(code);
  translateVectorAccess(code);
  translateAtomics(code);
  translateTypeConversions(code);
  translateMathFunctions(code);
  translateMiscBuiltins(code);
  cleanupDoubleQualifiers(code);
}

// ═══════════════════════════════════════════════════════════════════════════
//  Replacement helpers
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::replaceAll(std::string & str, const std::string & from, const std::string & to) -> void
{
  if (from.empty())
    return;
  size_t pos = 0;
  while ((pos = str.find(from, pos)) != std::string::npos)
  {
    str.replace(pos, from.size(), to);
    pos += to.size();
  }
}

auto
OpenCLToCUDATranslator::regexReplaceAll(std::string & str, const std::regex & pattern, const std::string & replacement)
  -> void
{
  str = std::regex_replace(str, pattern, replacement);
}

auto
OpenCLToCUDATranslator::replaceVectorConstructor(std::string &       code,
                                                  const std::string & openclCtor,
                                                  const std::string & cudaCtor) -> void
{
  // Match:  (typeN){ expr, expr, ... }
  // Replace with:  make_typeN( expr, expr, ... )
  //
  // We do brace-depth tracking so nested expressions work correctly.
  size_t pos = 0;
  while ((pos = code.find(openclCtor, pos)) != std::string::npos)
  {
    // Replace the opening  "(typeN){"  with  "make_typeN("
    code.replace(pos, openclCtor.size(), cudaCtor);
    pos += cudaCtor.size();

    // Now find the matching closing brace, respecting nesting.
    int    depth = 1;
    size_t i = pos;
    while (i < code.size() && depth > 0)
    {
      if (code[i] == '{')
        ++depth;
      else if (code[i] == '}')
        --depth;
      ++i;
    }

    if (depth == 0)
    {
      // i now points one past the '}'. Replace '}' (at i-1) with ')'.
      code[i - 1] = ')';
    }
  }
}

// ═══════════════════════════════════════════════════════════════════════════
//  Pragmas
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::translatePragmas(std::string & code) -> void
{
  // Comment out OpenCL pragmas (extensions, FP-contract, etc.)
  replaceAll(code, "#pragma OPENCL", "// #pragma OPENCL");

  // General catch-all: comment out remaining #pragma if not already commented
  // We do a line-by-line scan to avoid double-commenting.
  std::istringstream stream(code);
  std::ostringstream result;
  std::string        line;
  while (std::getline(stream, line))
  {
    // Trim leading whitespace to check content
    auto trimmed = line;
    trimmed.erase(0, trimmed.find_first_not_of(" \t"));

    if (trimmed.rfind("#pragma", 0) == 0 && trimmed.find("// #pragma") == std::string::npos)
    {
      result << "// " << line << "\n";
    }
    else
    {
      result << line << "\n";
    }
  }
  code = result.str();

  // Remove OpenCL version defines that make no sense in CUDA
  replaceAll(code, "#define CL_VERSION_", "// #define CL_VERSION_");
}

// ═══════════════════════════════════════════════════════════════════════════
//  Samplers
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::translateSamplers(std::string & code) -> void
{
  // "__constant sampler_t <name> = ..." → "__device__ int <name> = ..."
  replaceAll(code, "__constant sampler_t", "__device__ int");
  replaceAll(code, "const sampler_t", "__device__ int");
  replaceAll(code, "sampler_t", "int");

  // Sampler flag constants → integer literals (CUDA doesn't use them the same way)
  replaceAll(code, "CLK_NORMALIZED_COORDS_FALSE", "0");
  replaceAll(code, "CLK_NORMALIZED_COORDS_TRUE", "1");
  replaceAll(code, "CLK_ADDRESS_CLAMP_TO_EDGE", "0");
  replaceAll(code, "CLK_ADDRESS_CLAMP", "0");
  replaceAll(code, "CLK_ADDRESS_REPEAT", "0");
  replaceAll(code, "CLK_ADDRESS_NONE", "0");
  replaceAll(code, "CLK_FILTER_NEAREST", "0");
  replaceAll(code, "CLK_FILTER_LINEAR", "0");
}

// ═══════════════════════════════════════════════════════════════════════════
//  Kernel and function qualifiers
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::translateQualifiers(std::string & code) -> void
{
  // Kernel entry point
  replaceAll(code, "__kernel void", "extern \"C\" __global__ void");
  replaceAll(code, "kernel void", "extern \"C\" __global__ void");
  replaceAll(code, "__attribute__((reqd_work_group_size(", "// __attribute__((reqd_work_group_size(");

  // Non-kernel helper functions declared 'inline' should become __device__
  // We use word boundary matching via regex to avoid mangling "inline" inside strings.
  {
    // Match standalone "inline" that isn't already preceded by __device__
    static const std::regex inlineRe(R"((?<![_a-zA-Z])inline(?![_a-zA-Z]))");
    regexReplaceAll(code, inlineRe, "__device__ inline");
  }
}

// ═══════════════════════════════════════════════════════════════════════════
//  Address-space qualifiers
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::translateAddressSpaces(std::string & code) -> void
{
  // __global / global → (nothing, CUDA uses plain pointers for global memory)
  // But we must be careful not to remove it when it's part of __global__
  // Strategy: replace "__global " (with trailing space) and "global " when used as qualifier

  // Handle "__global" as OpenCL address-space qualifier (not CUDA's __global__)
  // Since we already converted __kernel → __global__, we need to be careful.
  // We look for __global that is *not* followed by '__' (i.e., not __global__).
  {
    static const std::regex globalQualRe(R"(__global(?!_))");
    regexReplaceAll(code, globalQualRe, "");
  }

  // __local → __shared__
  replaceAll(code, "__local ", "__shared__ ");

  // __constant (standalone, not already replaced) → __constant__
  {
    static const std::regex constQualRe(R"(__constant(?!_))");
    regexReplaceAll(code, constQualRe, "__constant__");
  }

  // __private → (nothing, CUDA local variables are private by default)
  replaceAll(code, "__private ", "");
  replaceAll(code, "__private", "");
}

// ═══════════════════════════════════════════════════════════════════════════
//  Work-item / work-group functions
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::translateWorkItemFunctions(std::string & code) -> void
{
  // get_global_id(dim)
  replaceAll(code, "get_global_id(0)", "(blockDim.x * blockIdx.x + threadIdx.x)");
  replaceAll(code, "get_global_id(1)", "(blockDim.y * blockIdx.y + threadIdx.y)");
  replaceAll(code, "get_global_id(2)", "(blockDim.z * blockIdx.z + threadIdx.z)");

  // get_local_id(dim)
  replaceAll(code, "get_local_id(0)", "threadIdx.x");
  replaceAll(code, "get_local_id(1)", "threadIdx.y");
  replaceAll(code, "get_local_id(2)", "threadIdx.z");

  // get_group_id(dim)
  replaceAll(code, "get_group_id(0)", "blockIdx.x");
  replaceAll(code, "get_group_id(1)", "blockIdx.y");
  replaceAll(code, "get_group_id(2)", "blockIdx.z");

  // get_local_size(dim)
  replaceAll(code, "get_local_size(0)", "blockDim.x");
  replaceAll(code, "get_local_size(1)", "blockDim.y");
  replaceAll(code, "get_local_size(2)", "blockDim.z");

  // get_global_size(dim)
  replaceAll(code, "get_global_size(0)", "(gridDim.x * blockDim.x)");
  replaceAll(code, "get_global_size(1)", "(gridDim.y * blockDim.y)");
  replaceAll(code, "get_global_size(2)", "(gridDim.z * blockDim.z)");

  // get_num_groups(dim)
  replaceAll(code, "get_num_groups(0)", "gridDim.x");
  replaceAll(code, "get_num_groups(1)", "gridDim.y");
  replaceAll(code, "get_num_groups(2)", "gridDim.z");
}

// ═══════════════════════════════════════════════════════════════════════════
//  Synchronization
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::translateSynchronization(std::string & code) -> void
{
  // barrier() → __syncthreads()
  replaceAll(code, "barrier(CLK_LOCAL_MEM_FENCE)", "__syncthreads()");
  replaceAll(code, "barrier(CLK_GLOBAL_MEM_FENCE)", "__syncthreads()");
  replaceAll(code, "barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE)", "__syncthreads()");
  replaceAll(code, "barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE)", "__syncthreads()");

  // mem_fence → __threadfence()  (approximate — semantics differ slightly)
  replaceAll(code, "mem_fence(CLK_GLOBAL_MEM_FENCE)", "__threadfence()");
  replaceAll(code, "mem_fence(CLK_LOCAL_MEM_FENCE)", "__threadfence_block()");
  replaceAll(code, "read_mem_fence(CLK_GLOBAL_MEM_FENCE)", "__threadfence()");
  replaceAll(code, "write_mem_fence(CLK_GLOBAL_MEM_FENCE)", "__threadfence()");
}

// ═══════════════════════════════════════════════════════════════════════════
//  Vector constructors
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::translateVectorConstructors(std::string & code) -> void
{
  // List of all standard OpenCL vector types and their sizes.
  // OpenCL compound literal:  (typeN){ a, b, ... }
  // CUDA equivalent:          make_typeN( a, b, ... )

  static const std::vector<std::pair<std::string, std::string>> vecTypes = {
    // char
    { "(char2){", "make_char2(" },
    { "(char3){", "make_char3(" },
    { "(char4){", "make_char4(" },
    // uchar
    { "(uchar2){", "make_uchar2(" },
    { "(uchar3){", "make_uchar3(" },
    { "(uchar4){", "make_uchar4(" },
    // short
    { "(short2){", "make_short2(" },
    { "(short3){", "make_short3(" },
    { "(short4){", "make_short4(" },
    // ushort
    { "(ushort2){", "make_ushort2(" },
    { "(ushort3){", "make_ushort3(" },
    { "(ushort4){", "make_ushort4(" },
    // int
    { "(int2){", "make_int2(" },
    { "(int3){", "make_int3(" },
    { "(int4){", "make_int4(" },
    // uint
    { "(uint2){", "make_uint2(" },
    { "(uint3){", "make_uint3(" },
    { "(uint4){", "make_uint4(" },
    // long
    { "(long2){", "make_long2(" },
    { "(long3){", "make_long3(" },
    { "(long4){", "make_long4(" },
    // ulong
    { "(ulong2){", "make_ulong2(" },
    { "(ulong3){", "make_ulong3(" },
    { "(ulong4){", "make_ulong4(" },
    // float
    { "(float2){", "make_float2(" },
    { "(float3){", "make_float3(" },
    { "(float4){", "make_float4(" },
    // double
    { "(double2){", "make_double2(" },
    { "(double3){", "make_double3(" },
    { "(double4){", "make_double4(" },
  };

  for (const auto & [from, to] : vecTypes)
  {
    replaceVectorConstructor(code, from, to);
  }
}

// ═══════════════════════════════════════════════════════════════════════════
//  Vector component access
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::translateVectorAccess(std::string & code) -> void
{
  // OpenCL .s0–.s3 → CUDA .x/.y/.z/.w
  // OpenCL .s4–.s7 and higher are not directly supported in CUDA vector types.
  // We only handle the first four which map to CUDA's built-in vector fields.

  // Must be careful to only replace when preceded by an identifier character
  // (letter, digit, underscore, or ')') — indicating member access, not arbitrary text.

  // Use regex for safety: match ".sN" that follows a word character or ')'
  static const std::vector<std::pair<std::regex, std::string>> accessMap = {
    { std::regex(R"(\.s0(?![0-9a-fA-F]))"), ".x" },
    { std::regex(R"(\.s1(?![0-9a-fA-F]))"), ".y" },
    { std::regex(R"(\.s2(?![0-9a-fA-F]))"), ".z" },
    { std::regex(R"(\.s3(?![0-9a-fA-F]))"), ".w" },
  };

  for (const auto & [pattern, replacement] : accessMap)
  {
    regexReplaceAll(code, pattern, replacement);
  }

  // Also handle the .lo / .hi accessors for 2-component vectors
  // .lo → .x   .hi → .y   (only valid for type2, rough approximation)
  // These are tricky in general so we leave them for now — uncomment if needed:
  // replaceAll(code, ".lo", ".x");
  // replaceAll(code, ".hi", ".y");
}

// ═══════════════════════════════════════════════════════════════════════════
//  Atomic operations
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::translateAtomics(std::string & code) -> void
{
  // OpenCL 1.x atomics → CUDA atomics
  replaceAll(code, "atomic_add(", "atomicAdd(");
  replaceAll(code, "atomic_sub(", "atomicSub(");
  replaceAll(code, "atomic_xchg(", "atomicExch(");
  replaceAll(code, "atomic_inc(", "atomicAdd("); // NOTE: CUDA atomicInc has different semantics
  replaceAll(code, "atomic_dec(", "atomicSub("); // NOTE: CUDA atomicDec has different semantics
  replaceAll(code, "atomic_min(", "atomicMin(");
  replaceAll(code, "atomic_max(", "atomicMax(");
  replaceAll(code, "atomic_and(", "atomicAnd(");
  replaceAll(code, "atomic_or(", "atomicOr(");
  replaceAll(code, "atomic_xor(", "atomicXor(");
  replaceAll(code, "atomic_cmpxchg(", "atomicCAS(");

  // OpenCL 2.0 C11-style atomics (basic mapping)
  replaceAll(code, "atomic_fetch_add(", "atomicAdd(");
  replaceAll(code, "atomic_fetch_sub(", "atomicSub(");
  replaceAll(code, "atomic_fetch_min(", "atomicMin(");
  replaceAll(code, "atomic_fetch_max(", "atomicMax(");
  replaceAll(code, "atomic_fetch_and(", "atomicAnd(");
  replaceAll(code, "atomic_fetch_or(", "atomicOr(");
  replaceAll(code, "atomic_fetch_xor(", "atomicXor(");
}

// ═══════════════════════════════════════════════════════════════════════════
//  Type conversion functions
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::translateTypeConversions(std::string & code) -> void
{
  // OpenCL convert_<type>(x) → CUDA cast or make_ function
  //
  // Scalar: convert_int(x)     → (int)(x)
  // Vector: convert_float4(x)  → make_float4(...)  — imperfect but functional for simple cases

  // Handle scalar conversions with optional rounding/saturation suffixes
  // e.g., convert_int_rte, convert_float_sat, convert_int_sat_rte
  static const std::vector<std::string> scalarTypes = { "char",  "uchar", "short", "ushort",
                                                        "int",   "uint",  "long",  "ulong",
                                                        "float", "double" };

  for (const auto & type : scalarTypes)
  {
    // Match convert_<type> possibly followed by _sat, _rte, _rtz, _rtp, _rtn
    // We strip the rounding/saturation suffix (CUDA doesn't have direct equivalents for casts)
    std::string pattern = "convert_" + type + "_sat_rte(";
    replaceAll(code, pattern, "(" + type + ")(");
    pattern = "convert_" + type + "_sat_rtz(";
    replaceAll(code, pattern, "(" + type + ")(");
    pattern = "convert_" + type + "_sat_rtp(";
    replaceAll(code, pattern, "(" + type + ")(");
    pattern = "convert_" + type + "_sat_rtn(";
    replaceAll(code, pattern, "(" + type + ")(");
    pattern = "convert_" + type + "_sat(";
    replaceAll(code, pattern, "(" + type + ")(");
    pattern = "convert_" + type + "_rte(";
    replaceAll(code, pattern, "(" + type + ")(");
    pattern = "convert_" + type + "_rtz(";
    replaceAll(code, pattern, "(" + type + ")(");
    pattern = "convert_" + type + "_rtp(";
    replaceAll(code, pattern, "(" + type + ")(");
    pattern = "convert_" + type + "_rtn(";
    replaceAll(code, pattern, "(" + type + ")(");
    pattern = "convert_" + type + "(";
    replaceAll(code, pattern, "(" + type + ")(");
  }

  // Vector conversions: convert_<type>N(...) → make_<type>N(...)
  // This is a rough approximation — true component-wise conversion would need per-element casts
  static const std::vector<std::string> vecSizes = { "2", "3", "4", "8", "16" };
  for (const auto & type : scalarTypes)
  {
    for (const auto & n : vecSizes)
    {
      std::string from = "convert_" + type + n + "(";
      std::string to = "make_" + type + n + "(";
      replaceAll(code, from, to);
    }
  }

  // as_<type> reinterpret casts → reinterpret_cast or __<type>_as_<type> intrinsics
  // Basic approximation: as_float(x) → __int_as_float(x), etc.
  replaceAll(code, "as_float(", "__int_as_float(");
  replaceAll(code, "as_int(", "__float_as_int(");
  replaceAll(code, "as_uint(", "__float_as_uint(");
}

// ═══════════════════════════════════════════════════════════════════════════
//  Math functions
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::translateMathFunctions(std::string & code) -> void
{
  // OpenCL native_* and half_* → CUDA fast intrinsics (__*f)
  // native_* are "fast, lower-accuracy" variants.
  // CUDA provides __sinf, __cosf, __expf, __logf, __powf, __sqrtf, etc.

  replaceAll(code, "native_sin(", "__sinf(");
  replaceAll(code, "native_cos(", "__cosf(");
  replaceAll(code, "native_tan(", "__tanf(");
  replaceAll(code, "native_exp(", "__expf(");
  replaceAll(code, "native_exp2(", "exp2f(");
  replaceAll(code, "native_exp10(", "__exp10f(");
  replaceAll(code, "native_log(", "__logf(");
  replaceAll(code, "native_log2(", "__log2f(");
  replaceAll(code, "native_log10(", "__log10f(");
  replaceAll(code, "native_sqrt(", "__fsqrt_rn(");
  replaceAll(code, "native_rsqrt(", "rsqrtf(");
  replaceAll(code, "native_powr(", "__powf(");
  replaceAll(code, "native_recip(", "__frcp_rn(");
  replaceAll(code, "native_divide(", "__fdividef(");

  // half_* are similar to native_*
  replaceAll(code, "half_sin(", "__sinf(");
  replaceAll(code, "half_cos(", "__cosf(");
  replaceAll(code, "half_tan(", "__tanf(");
  replaceAll(code, "half_exp(", "__expf(");
  replaceAll(code, "half_exp2(", "exp2f(");
  replaceAll(code, "half_exp10(", "__exp10f(");
  replaceAll(code, "half_log(", "__logf(");
  replaceAll(code, "half_log2(", "__log2f(");
  replaceAll(code, "half_log10(", "__log10f(");
  replaceAll(code, "half_sqrt(", "__fsqrt_rn(");
  replaceAll(code, "half_rsqrt(", "rsqrtf(");
  replaceAll(code, "half_powr(", "__powf(");
  replaceAll(code, "half_recip(", "__frcp_rn(");
  replaceAll(code, "half_divide(", "__fdividef(");

  // mad() → fma() in CUDA (fused multiply-add)
  replaceAll(code, "mad(", "fma(");
}

// ═══════════════════════════════════════════════════════════════════════════
//  Image operations
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::translateImageOperations(std::string & code) -> void
{
  // Image support is highly variable depending on the framework.
  // Below is a stub that handles the most common patterns.
  //
  // In many GPGPU frameworks (e.g., CLIJ), images are actually passed as
  // flat arrays with index macros, so the image types rarely appear raw.
  //
  // For raw OpenCL image usage, a full translation would require
  // CUDA texture objects and surface objects, which have very different APIs.

  // Image type declarations in kernel parameters:
  //   __read_only image2d_t → cudaTextureObject_t  (approximate)
  //   __write_only image2d_t → cudaSurfaceObject_t  (approximate)
  replaceAll(code, "__read_only image3d_t", "cudaTextureObject_t");
  replaceAll(code, "__write_only image3d_t", "cudaSurfaceObject_t");
  replaceAll(code, "__read_only image2d_t", "cudaTextureObject_t");
  replaceAll(code, "__write_only image2d_t", "cudaSurfaceObject_t");
  replaceAll(code, "read_only image3d_t", "cudaTextureObject_t");
  replaceAll(code, "write_only image3d_t", "cudaSurfaceObject_t");
  replaceAll(code, "read_only image2d_t", "cudaTextureObject_t");
  replaceAll(code, "write_only image2d_t", "cudaSurfaceObject_t");
  replaceAll(code, "image2d_t", "cudaTextureObject_t");
  replaceAll(code, "image3d_t", "cudaTextureObject_t");

  // Basic image read/write — these are very approximate:
  // read_imagef(img, sampler, coord)  → tex2D<float4>(img, coord.x, coord.y)
  // This would need proper parenthesis matching for a real implementation.
  // Left as markers for now:
  replaceAll(code, "read_imagef(", "tex2D<float4>(");
  replaceAll(code, "read_imagei(", "tex2D<int4>(");
  replaceAll(code, "read_imageui(", "tex2D<uint4>(");

  // get_image_width/height/depth
  replaceAll(code, "get_image_width(", "/* get_image_width */ (");
  replaceAll(code, "get_image_height(", "/* get_image_height */ (");
  replaceAll(code, "get_image_depth(", "/* get_image_depth */ (");
}

// ═══════════════════════════════════════════════════════════════════════════
//  Miscellaneous built-ins
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::translateMiscBuiltins(std::string & code) -> void
{
  // select(a, b, cond) → (cond) ? b : a
  // This is difficult to do with simple replacement because of arbitrary expressions.
  // We'll leave a comment marker for now and handle simple cases:
  // (A full implementation would need expression-aware parenthesis matching.)

  // printf — CUDA supports printf in device code since compute capability 2.0
  // No change needed, but OpenCL printf has %v format specifiers that CUDA doesn't support.

  // CLK_* constants that might remain
  replaceAll(code, "CLK_LOCAL_MEM_FENCE", "0");
  replaceAll(code, "CLK_GLOBAL_MEM_FENCE", "0");

  // OpenCL type aliases
  replaceAll(code, "uchar", "unsigned char");
  replaceAll(code, "ushort", "unsigned short");
  // Note: "uint" is already defined in CUDA headers, usually fine as-is.
  // "ulong" → "unsigned long" if needed:
  // replaceAll(code, "ulong", "unsigned long");
}

// ═══════════════════════════════════════════════════════════════════════════
//  Cleanup pass — fix double qualifiers
// ═══════════════════════════════════════════════════════════════════════════

auto
OpenCLToCUDATranslator::cleanupDoubleQualifiers(std::string & code) -> void
{
  // In case multiple passes introduced duplicate qualifiers
  replaceAll(code, "__device__ __device__", "__device__");
  replaceAll(code, "__global__ __global__", "__global__");
  replaceAll(code, "__shared__ __shared__", "__shared__");
  replaceAll(code, "__constant__ __constant__", "__constant__");
  replaceAll(code, "extern \"C\" extern \"C\"", "extern \"C\"");

  // Clean up double spaces introduced by removals
  {
    static const std::regex doubleSpace(R"(  +)");
    regexReplaceAll(code, doubleSpace, " ");
  }

  // Clean up empty lines (more than 2 consecutive newlines → 2)
  {
    static const std::regex multiNewline(R"(\n{3,})");
    regexReplaceAll(code, multiNewline, "\n\n");
  }
}