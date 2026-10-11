#include "translator.hpp"
#include "translator_common.hpp"

#include <sstream>
#include <utility>
#include <vector>

using namespace cle::detail;

namespace
{

// OpenCL atomic_cmpxchg takes `volatile` pointers, CUDA atomicCAS does not: drop the qualifier. Each operand is
// deduced separately and converted to the pointee type, as OpenCL code often mixes signed and unsigned values.
constexpr const char * atomicCmpxchgHelper = R"CU(
template <typename T, typename C, typename V>
__device__ inline T cle_atomic_cmpxchg(volatile T * ptr, C cmp, V val)
{
  return atomicCAS(const_cast<T *>(ptr), static_cast<T>(cmp), static_cast<T>(val));
}
)CU";

using Replacements = std::vector<std::pair<std::string, std::string>>;

/// Apply the replacements in order (all occurrences of each)
auto
replaceEach(std::string & code, const Replacements & replacements) -> void
{
  for (const auto & [from, to] : replacements)
  {
    replaceAll(code, from, to);
  }
}

const std::vector<std::string> scalarTypes = { "char", "uchar", "short", "ushort", "int", "uint", "long", "ulong", "float", "double" };

/**
 * @brief Give every kernel the global size requested by the host (3 trailing `int` parameters, passed by
 *        CUDABackend::executeKernel), as OpenCL does.
 *
 * CUDA launches whole blocks: the grid covers a multiple of the block size, so without this some threads run
 * beyond the global size (e.g. 96 threads for a width of 80), and get_global_size() returns the rounded size.
 *  - get_global_size(d) inside a kernel body returns the requested size;
 *  - out-of-range threads return immediately (they do not exist in OpenCL). Not done when the code uses barriers:
 *    every thread of a block must reach them, such kernels handle the range themselves.
 * A kernel whose signature or body cannot be delimited is left unchanged (the extra arguments are then ignored).
 */
auto
addGlobalSizeParameters(std::string & code) -> void
{
  static const std::string marker = "__kernel void";
  static const std::string params = "const int cle_global_size_0, const int cle_global_size_1, const int cle_global_size_2";
  const bool               uses_barrier = code.find("barrier(") != std::string::npos;
  const std::string guard = "\n  if (get_global_id(0) >= cle_global_size_0 || get_global_id(1) >= cle_global_size_1 || get_global_id(2) >= "
                            "cle_global_size_2) return;";

  forEachKernelSignature(code, marker, [&](size_t start, size_t open_paren, size_t close_paren) -> size_t {
    const size_t open_brace = code.find('{', close_paren);
    const size_t close_brace = (open_brace == std::string::npos) ? open_brace : findClosing(code, open_brace, '{', '}');
    if (close_brace == std::string::npos || code.find_first_not_of(" \t\r\n", close_paren + 1) != open_brace) // e.g. a prototype
    {
      return start + marker.size();
    }

    // body: get_global_size(d) -> requested size
    std::string body = code.substr(open_brace + 1, close_brace - open_brace - 1);
    for (const char dim : { '0', '1', '2' })
    {
      replaceAll(body, std::string("get_global_size(") + dim + ")", std::string("cle_global_size_") + dim);
    }
    if (!uses_barrier)
    {
      body.insert(0, guard);
    }

    // parameters
    const std::string current = code.substr(open_paren + 1, close_paren - open_paren - 1);
    const bool        no_params = current.find_first_not_of(" \t\r\n") == std::string::npos || current == "void";
    const std::string new_params = no_params ? params : current + ", " + params;

    const std::string kernel =
      code.substr(start, open_paren + 1 - start) + new_params + ")" + code.substr(close_paren + 1, open_brace - close_paren) + body + "}";
    code.replace(start, close_brace + 1 - start, kernel);
    return start + kernel.size();
  });
}

/// Comment out OpenCL pragmas and version defines
auto
translatePragmas(std::string & code) -> void
{
  replaceAll(code, "#pragma OPENCL", "// #pragma OPENCL");

  // remaining #pragma lines (unroll, ...), not commented twice
  std::istringstream stream(code);
  std::ostringstream result;
  std::string        line;
  while (std::getline(stream, line))
  {
    auto trimmed = line;
    trimmed.erase(0, trimmed.find_first_not_of(" \t"));
    if (trimmed.rfind("#pragma", 0) == 0 && trimmed.find("// #pragma") == std::string::npos)
    {
      result << "// ";
    }
    result << line << "\n";
  }
  code = result.str();

  replaceAll(code, "#define CL_VERSION_", "// #define CL_VERSION_");
}

/// Samplers become plain ints, sampler flags integer literals (CUDA doesn't use them)
auto
translateSamplers(std::string & code) -> void
{
  replaceEach(code,
              { { "__constant sampler_t", "__device__ int" },
                { "const sampler_t", "__device__ int" },
                { "sampler_t", "int" },
                { "CLK_NORMALIZED_COORDS_FALSE", "0" },
                { "CLK_NORMALIZED_COORDS_TRUE", "1" },
                { "CLK_ADDRESS_CLAMP_TO_EDGE", "0" },
                { "CLK_ADDRESS_CLAMP", "0" },
                { "CLK_ADDRESS_REPEAT", "0" },
                { "CLK_ADDRESS_NONE", "0" },
                { "CLK_FILTER_NEAREST", "0" },
                { "CLK_FILTER_LINEAR", "0" } });
}

/// Kernel entry points and device functions
auto
translateQualifiers(std::string & code) -> void
{
  replaceEach(code,
              { { "__kernel void", "extern \"C\" __global__ void" },
                { "kernel void", "extern \"C\" __global__ void" },
                { "__attribute__((reqd_work_group_size(", "// __attribute__((reqd_work_group_size(" } });
  replaceWord(code, "inline", "__device__ inline");
}

auto
translateAddressSpaces(std::string & code) -> void
{
  replaceSuffix(code, "__global", ""); // CUDA uses plain pointers; keeps "__global__"
  replaceAll(code, "__local ", "__shared__ ");
  replaceSuffix(code, "__constant", "__constant__");
  replaceEach(code, { { "__private ", "" }, { "__private", "" } });
}

/// get_<function>(d) -> CUDA built-ins, cast to int as they are unsigned
auto
translateWorkItemFunctions(std::string & code) -> void
{
  // '@' stands for the axis x, y or z
  static const Replacements functions = {
    { "get_global_id", "((int)(blockDim.@ * blockIdx.@ + threadIdx.@))" },
    { "get_local_id", "((int)threadIdx.@)" },
    { "get_group_id", "((int)blockIdx.@)" },
    { "get_local_size", "((int)blockDim.@)" },
    { "get_global_size", "((int)(gridDim.@ * blockDim.@))" },
    { "get_num_groups", "((int)gridDim.@)" },
  };
  for (const auto & [function, builtin] : functions)
  {
    for (const auto & [dim, axis] : { std::pair{ '0', "x" }, std::pair{ '1', "y" }, std::pair{ '2', "z" } })
    {
      std::string to = builtin;
      replaceAll(to, "@", axis);
      replaceAll(code, function + "(" + dim + ")", to);
    }
  }
}

auto
translateSynchronization(std::string & code) -> void
{
  // read_/write_ fences first: "mem_fence(" is a suffix of their names. __threadfence is approximate.
  replaceEach(code,
              { { "barrier(CLK_LOCAL_MEM_FENCE)", "__syncthreads()" },
                { "barrier(CLK_GLOBAL_MEM_FENCE)", "__syncthreads()" },
                { "barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE)", "__syncthreads()" },
                { "barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE)", "__syncthreads()" },
                { "read_mem_fence(CLK_GLOBAL_MEM_FENCE)", "__threadfence()" },
                { "write_mem_fence(CLK_GLOBAL_MEM_FENCE)", "__threadfence()" },
                { "mem_fence(CLK_GLOBAL_MEM_FENCE)", "__threadfence()" },
                { "mem_fence(CLK_LOCAL_MEM_FENCE)", "__threadfence_block()" } });
}

/// (typeN)(a, b) and (typeN){ a, b } -> make_typeN(a, b)
auto
translateVectorConstructors(std::string & code) -> void
{
  std::vector<std::string> vector_types;
  for (const auto & type : scalarTypes)
  {
    for (const char n : { '2', '3', '4' })
    {
      vector_types.push_back(type + n);
    }
  }

  // functional casts first; preceded by an alphanumeric character, it is not a cast
  for (const auto & type : vector_types)
  {
    const std::string pattern = "(" + type + ")(";
    const std::string replacement = "make_" + type + "(";
    size_t            pos = 0;
    while ((pos = code.find(pattern, pos)) != std::string::npos)
    {
      if (pos > 0 && std::isalnum(static_cast<unsigned char>(code[pos - 1])))
      {
        pos += pattern.size();
        continue;
      }
      code.replace(pos, pattern.size(), replacement);
      pos += replacement.size();
    }
  }

  // compound literals: the matching '}' becomes ')' (nested braces allowed)
  for (const auto & type : vector_types)
  {
    const std::string pattern = "(" + type + "){";
    const std::string replacement = "make_" + type + "(";
    for (size_t pos = code.find(pattern); pos != std::string::npos; pos = code.find(pattern, pos))
    {
      code.replace(pos, pattern.size(), replacement);
      pos += replacement.size();
      int    depth = 1;
      size_t i = pos;
      for (; i < code.size() && depth > 0; ++i)
      {
        depth += (code[i] == '{') - (code[i] == '}');
      }
      if (depth == 0)
      {
        code[i - 1] = ')';
      }
    }
  }
}

/// .s0 - .s3 -> .x - .w (the higher components have no CUDA equivalent)
auto
translateVectorAccess(std::string & code) -> void
{
  for (const auto & [from, to] : Replacements{ { ".s0", ".x" }, { ".s1", ".y" }, { ".s2", ".z" }, { ".s3", ".w" } })
  {
    replaceSuffix(code, from, to);
  }
}

auto
translateAtomics(std::string & code) -> void
{
  replaceEach(code,
              { { "atomic_add(", "atomicAdd(" },
                { "atomic_sub(", "atomicSub(" },
                { "atomic_xchg(", "atomicExch(" },
                { "atomic_inc(", "atomicAdd(" }, // NOTE: CUDA atomicInc has different semantics
                { "atomic_dec(", "atomicSub(" }, // NOTE: CUDA atomicDec has different semantics
                { "atomic_min(", "atomicMin(" },
                { "atomic_max(", "atomicMax(" },
                { "atomic_and(", "atomicAnd(" },
                { "atomic_or(", "atomicOr(" },
                { "atomic_xor(", "atomicXor(" },
                { "atomic_cmpxchg(", "cle_atomic_cmpxchg(" }, // atomicCAS without volatile, see atomicCmpxchgHelper
                // OpenCL 2.0 C11-style atomics (basic mapping)
                { "atomic_fetch_add(", "atomicAdd(" },
                { "atomic_fetch_sub(", "atomicSub(" },
                { "atomic_fetch_min(", "atomicMin(" },
                { "atomic_fetch_max(", "atomicMax(" },
                { "atomic_fetch_and(", "atomicAnd(" },
                { "atomic_fetch_or(", "atomicOr(" },
                { "atomic_fetch_xor(", "atomicXor(" } });
}

/// convert_<type>[_sat][_rounding](x) -> (type)(x), convert_<type>N(x) -> make_<type>N(x), as_<type> -> intrinsics
auto
translateTypeConversions(std::string & code) -> void
{
  // rounding / saturation have no cast equivalent: dropped. Longest suffixes first.
  static const std::vector<std::string> suffixes = { "_sat_rte", "_sat_rtz", "_sat_rtp", "_sat_rtn", "_sat",
                                                     "_rte",     "_rtz",     "_rtp",     "_rtn",     "" };
  for (const auto & type : scalarTypes)
  {
    for (const auto & suffix : suffixes)
    {
      replaceAll(code, "convert_" + type + suffix + "(", "(" + type + ")(");
    }
  }
  // rough approximation: a true component-wise conversion would need per-element casts
  for (const auto & type : scalarTypes)
  {
    for (const char * n : { "2", "3", "4", "8", "16" })
    {
      replaceAll(code, "convert_" + type + n + "(", "make_" + type + n + "(");
    }
  }
  replaceEach(code, { { "as_float(", "__int_as_float(" }, { "as_int(", "__float_as_int(" }, { "as_uint(", "__float_as_uint(" } });
}

/// native_* and half_* (fast, lower accuracy) -> CUDA fast intrinsics; mad -> fma
auto
translateMathFunctions(std::string & code) -> void
{
  static const Replacements functions = {
    { "sin", "__sinf" },     { "cos", "__cosf" },  { "tan", "__tanf" },      { "exp", "__expf" },        { "exp2", "exp2f" },
    { "exp10", "__exp10f" }, { "log", "__logf" },  { "log2", "__log2f" },    { "log10", "__log10f" },    { "sqrt", "__fsqrt_rn" },
    { "rsqrt", "rsqrtf" },   { "powr", "__powf" }, { "recip", "__frcp_rn" }, { "divide", "__fdividef" },
  };
  for (const std::string prefix : { "native_", "half_" })
  {
    for (const auto & [function, intrinsic] : functions)
    {
      replaceAll(code, prefix + function + "(", intrinsic + "(");
    }
  }
  replaceAll(code, "mad(", "fma(");
}

/// clamp(x, lo, hi) -> min(max(x, lo), hi) (no clamp built-in in CUDA)
auto
replaceClamp(std::string & code) -> void
{
  const std::string tag = "clamp";
  for (size_t pos = code.find(tag); pos != std::string::npos; pos = code.find(tag, pos))
  {
    size_t open = pos + tag.size();
    while (open < code.size() && code[open] == ' ')
    {
      ++open;
    }
    const bool   is_call = !(pos > 0 && isWordChar(code[pos - 1])) && open < code.size() && code[open] == '(';
    const size_t close = is_call ? findClosing(code, open) : std::string::npos;
    const auto   commas = (close == std::string::npos) ? std::vector<size_t>{} : topLevelCommas(code, open + 1, close);
    if (commas.size() < 2)
    {
      pos += tag.size();
      continue;
    }
    const std::string replacement = "min(max(" + code.substr(open + 1, commas[0] - open - 1) + "," +
                                    code.substr(commas[0] + 1, commas[1] - commas[0] - 1) + ")," +
                                    code.substr(commas[1] + 1, close - commas[1] - 1) + ")";
    code.replace(pos, close + 1 - pos, replacement);
    pos += replacement.size();
  }
}

auto
translateMiscBuiltins(std::string & code) -> void
{
  replaceEach(code, { { "CLK_LOCAL_MEM_FENCE", "0" }, { "CLK_GLOBAL_MEM_FENCE", "0" } });

  // OpenCL type aliases missing in NVRTC; whole words only (uchar2, uint3, ... are CUDA built-ins)
  replaceWord(code, "uchar", "unsigned char");
  replaceWord(code, "ushort", "unsigned short");
  replaceWord(code, "uint", "unsigned int");
  replaceWord(code, "ulong", "unsigned long");

  replaceClamp(code);
}

/// Duplicate qualifiers introduced by the passes, and the whitespace left by removals
auto
cleanup(std::string & code) -> void
{
  replaceEach(code,
              { { "__device__ __device__", "__device__" },
                { "__global__ __global__", "__global__" },
                { "__shared__ __shared__", "__shared__" },
                { "__constant__ __constant__", "__constant__" },
                { "extern \"C\" extern \"C\"", "extern \"C\"" } });
  collapseWhitespace(code);
}

} // namespace

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
  addGlobalSizeParameters(code); // first: works on the OpenCL kernel signatures
  translatePragmas(code);
  translateSamplers(code);
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
  cleanup(code);

  // Added last, so that the passes above do not rewrite it
  if (code.find("cle_atomic_cmpxchg(") != std::string::npos)
  {
    code.insert(0, atomicCmpxchgHelper);
  }
}
