#include "translator.hpp"
#include "translator_common.hpp"

#include <sstream>
#include <utility>
#include <vector>

using namespace cle::detail;

namespace
{

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

/// True when `token` is one of the recognized scalar type names.
auto
isScalarType(const std::string & token) -> bool
{
  static const char * types[] = { "float", "int", "uint", "short", "ushort", "char", "uchar", "long", "ulong", "half", "double" };
  for (const auto * t : types)
  {
    if (token == t)
    {
      return true;
    }
  }
  return false;
}

/// Parse a scalar parameter "[const] <type> <name> [suffix]"; false if it is not one (pointer, array, ...).
auto
parseScalarParam(const std::string & trimmed, bool & isConst, std::string & type, std::string & name, std::string & suffix) -> bool
{
  if (trimmed.find('*') != std::string::npos || trimmed.find('[') != std::string::npos)
  {
    return false;
  }

  std::vector<std::string> tokens;
  std::istringstream       stream(trimmed);
  for (std::string token; stream >> token;)
  {
    tokens.push_back(token);
  }

  size_t idx = 0;
  isConst = idx < tokens.size() && tokens[idx] == "const";
  idx += isConst ? 1 : 0;
  if (idx >= tokens.size())
  {
    return false;
  }

  // type: a single word ("float") or "unsigned"/"signed" + int/short/char/long
  if (tokens[idx] == "unsigned" || tokens[idx] == "signed")
  {
    const bool valid = idx + 1 < tokens.size() &&
                       (tokens[idx + 1] == "int" || tokens[idx + 1] == "short" || tokens[idx + 1] == "char" || tokens[idx + 1] == "long");
    if (!valid)
    {
      return false;
    }
    type = tokens[idx] + " " + tokens[idx + 1];
    idx += 2;
  }
  else if (isScalarType(tokens[idx]))
  {
    type = tokens[idx++];
  }
  else
  {
    return false;
  }

  // name: an identifier
  if (idx >= tokens.size() || !(std::isalpha(static_cast<unsigned char>(tokens[idx][0])) || tokens[idx][0] == '_'))
  {
    return false;
  }
  name = tokens[idx++];

  // remaining tokens: suffix
  suffix.clear();
  for (; idx < tokens.size(); ++idx)
  {
    suffix += (suffix.empty() ? "" : " ") + tokens[idx];
  }
  return true;
}

/// Insert "thread " before IMAGE_<key>_PIXEL_TYPE when followed by '*' (helpers receiving stack arrays)
auto
addThreadToPixelTypePtrs(std::string & code) -> void
{
  const std::string tag = "_PIXEL_TYPE";
  const std::string prefix = "thread ";
  size_t            pos = 0;
  while ((pos = code.find(tag, pos)) != std::string::npos)
  {
    // start of the identifier, which must be "IMAGE_..."
    size_t start = pos;
    while (start > 0 && isWordChar(code[start - 1]))
    {
      --start;
    }
    const size_t after_tag = pos + tag.size();
    size_t       star = after_tag;
    while (star < code.size() && code[star] == ' ')
    {
      ++star;
    }
    if (code.compare(start, 6, "IMAGE_") != 0)
    {
      pos += tag.size();
    }
    else if (star >= code.size() || code[star] != '*')
    {
      pos = after_tag;
    }
    else if (start >= prefix.size() && code.compare(start - prefix.size(), prefix.size(), prefix) == 0)
    {
      pos = star + 1; // already prefixed
    }
    else
    {
      code.insert(start, prefix);
      pos = star + prefix.size() + 1;
    }
  }
}

/// Metal thread-position built-ins appended to every kernel signature (get_global_id & co. are rewritten to them)
auto
appendBuiltinKernelArgs(std::string & code) -> void
{
  static const std::string builtins = "uint3 __cle_gid [[thread_position_in_grid]], "
                                      "uint3 __cle_tid [[thread_position_in_threadgroup]], "
                                      "uint3 __cle_tgid [[threadgroup_position_in_grid]], "
                                      "uint3 __cle_tptg [[threads_per_threadgroup]], "
                                      "uint3 __cle_tgpg [[threadgroups_per_grid]], "
                                      "uint3 __cle_tpg [[threads_per_grid]]";

  forEachKernelSignature(code, "kernel void", [&](size_t, size_t open, size_t close) -> size_t {
    const std::string params = code.substr(open + 1, close - open - 1);
    if (params.find("__cle_gid") != std::string::npos)
    {
      return close + 1;
    }
    const std::string insertion = (params.find_first_not_of(" \t\n\r") == std::string::npos) ? builtins : ", " + builtins;
    code.insert(close, insertion);
    return close + insertion.size() + 1;
  });
}

/**
 * @brief Scalar kernel parameters: Metal passes them by reference from the constant address space.
 *
 * `const T x` becomes `constant T& x`. A non-const `T x` (modifiable in OpenCL) becomes a hidden
 * `constant T& __cle_arg_x` copied into a mutable local `T x` at kernel entry.
 */
auto
translateKernelScalarArgs(std::string & code) -> void
{
  forEachKernelSignature(code, "kernel void", [&](size_t, size_t open, size_t close) -> size_t {
    // drop `// ...` comments so that their commas do not split the parameter list
    std::string params;
    bool        in_comment = false;
    for (size_t k = open + 1; k < close; ++k)
    {
      if (!in_comment && code.compare(k, 2, "//") == 0)
      {
        in_comment = true;
      }
      else if (code[k] == '\n')
      {
        in_comment = false;
      }
      if (!in_comment)
      {
        params += code[k];
      }
    }

    // parameters, split on every comma
    std::vector<std::string> mutable_locals;
    std::string              new_params;
    bool                     changed = false;
    size_t                   start = 0;
    for (bool first = true, last = false; !last; first = false)
    {
      const size_t comma = params.find(',', start);
      last = (comma == std::string::npos);
      std::string param = params.substr(start, last ? std::string::npos : comma - start);
      start = comma + 1;

      bool        is_const = false;
      std::string type, name, suffix;
      if (parseScalarParam(trimWhitespace(param), is_const, type, name, suffix))
      {
        param = "constant " + type + "& " + (is_const ? "" : "__cle_arg_") + name + (suffix.empty() ? "" : " " + suffix);
        if (!is_const)
        {
          mutable_locals.push_back(type + " " + name + " = __cle_arg_" + name + ";");
        }
        changed = true;
      }
      new_params += (first ? "" : ", ") + param;
    }

    if (!changed)
    {
      return close + 1;
    }
    code.replace(open + 1, close - open - 1, new_params);
    size_t resume = open + 1 + new_params.size() + 1;
    if (!mutable_locals.empty())
    {
      const size_t body = code.find('{', resume);
      if (body != std::string::npos)
      {
        std::string locals = "\n";
        for (const auto & local : mutable_locals)
        {
          locals += " " + local + "\n";
        }
        code.insert(body + 1, locals);
        resume = body + 1 + locals.size();
      }
    }
    return resume;
  });
}

auto
translateSamplers(std::string & code) -> void
{
  // The Metal preamble defines sampler_t as int and CLK_* as integer constants: only the address space changes
  replaceEach(code, { { "__constant sampler_t", "constant sampler_t" }, { "const sampler_t", "constant sampler_t" } });
  // a variable named 'sampler' is ambiguous with metal::sampler
  replaceWord(code, "sampler", "__cle_sampler");
}

auto
translateAddressSpaces(std::string & code) -> void
{
  replaceEach(code,
              { { "__global ", "device " },
                { " global ", " device " },
                { "__local ", "threadgroup " },
                { " local ", " threadgroup " },
                { "__constant ", "constant " },
                { "__private ", "" } });

  addThreadToPixelTypePtrs(code);
  replaceAll(code, " private ", " ");

  // IMAGE_<key>_TYPE pointer macros expand to kernel parameters: MSL requires an explicit address space
  std::istringstream stream(code);
  std::ostringstream result;
  std::string        line;
  while (std::getline(stream, line))
  {
    const size_t type_end = line.find("_TYPE");
    if (line.find("#define IMAGE_") != std::string::npos && type_end != std::string::npos && line.find('*') != std::string::npos &&
        line.find("device ") == std::string::npos && line.find("threadgroup ") == std::string::npos &&
        line.find("constant ") == std::string::npos)
    {
      size_t at = type_end + 5;
      while (at < line.size() && line[at] == ' ')
      {
        ++at;
      }
      line.insert(at, "device ");
    }
    result << line << "\n";
  }
  code = result.str();
}

/// (typeN)(a, b) -> typeN(a, b), which MSL parses as a constructor
auto
translateVectorConstructors(std::string & code) -> void
{
  for (const char * type : { "char", "uchar", "short", "ushort", "int", "uint", "long", "ulong", "float", "half", "double" })
  {
    for (const char n : { '2', '3', '4' })
    {
      replaceAll(code, "(" + std::string(type) + n + ")(", std::string(type) + n + "(");
    }
  }
}

auto
translateWorkItemFunctions(std::string & code) -> void
{
  // threads_per_grid is the exact grid size of dispatchThreads (as OpenCL's global size); threadgroup size times
  // threadgroup count would round it up to whole threadgroups
  static const Replacements functions = {
    { "get_global_id", "__cle_gid" },   { "get_local_id", "__cle_tid" },    { "get_group_id", "__cle_tgid" },
    { "get_local_size", "__cle_tptg" }, { "get_num_groups", "__cle_tgpg" }, { "get_global_size", "__cle_tpg" },
  };
  for (const auto & [function, builtin] : functions)
  {
    for (const auto & [dim, axis] : { std::pair{ '0', 'x' }, std::pair{ '1', 'y' }, std::pair{ '2', 'z' } })
    {
      replaceAll(code, function + "(" + dim + ")", "((int)" + builtin + "." + axis + ")");
    }
  }
}

auto
translateSynchronization(std::string & code) -> void
{
  // read_/write_ fences first: "mem_fence(" is a suffix of their names
  const std::string threadgroup = "threadgroup_barrier(mem_flags::mem_threadgroup)";
  const std::string device = "threadgroup_barrier(mem_flags::mem_device)";
  replaceEach(code,
              { { "barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE)", threadgroup },
                { "barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE)", threadgroup },
                { "barrier(CLK_LOCAL_MEM_FENCE)", threadgroup },
                { "barrier(CLK_GLOBAL_MEM_FENCE)", device },
                { "read_mem_fence(CLK_LOCAL_MEM_FENCE)", threadgroup },
                { "read_mem_fence(CLK_GLOBAL_MEM_FENCE)", device },
                { "write_mem_fence(CLK_LOCAL_MEM_FENCE)", threadgroup },
                { "write_mem_fence(CLK_GLOBAL_MEM_FENCE)", device },
                { "mem_fence(CLK_LOCAL_MEM_FENCE)", threadgroup },
                { "mem_fence(CLK_GLOBAL_MEM_FENCE)", device } });
}

/**
 * @brief old = atomic_cmpxchg(ptr, cmp, val);  ->  MSL compare-exchange (in/out "expected" pointer, returns a bool)
 *
 * It cannot be substituted at the expression level: the whole "<lhs> = atomic_cmpxchg(...);" statement is
 * rewritten, the only form the CAS-loop helpers use. Other forms are left unchanged.
 */
auto
translateCompareExchange(std::string & code) -> void
{
  const std::string marker = "atomic_cmpxchg(";
  size_t            pos = 0;
  while (const auto call = nextCall(code, marker, pos))
  {
    pos = call->close + 1;
    const auto commas = topLevelCommas(code, call->open + 1, call->close);
    if (commas.size() != 2)
    {
      continue;
    }

    // a plain assignment "lhs = atomic_cmpxchg(...);"
    size_t eq = call->start;
    while (eq > 0 && std::isspace(static_cast<unsigned char>(code[eq - 1])))
    {
      --eq;
    }
    const bool   is_assignment = eq > 0 && code[eq - 1] == '=' &&
                                 !(eq > 1 && (code[eq - 2] == '=' || code[eq - 2] == '!' || code[eq - 2] == '<' || code[eq - 2] == '>'));
    const size_t semi = code.find_first_not_of(" \t\r\n\f\v", call->close + 1);
    if (!is_assignment || semi == std::string::npos || code[semi] != ';')
    {
      continue;
    }

    const size_t lhs_end = eq - 1;
    size_t       stmt_start = code.find_last_of(";{", lhs_end > 0 ? lhs_end - 1 : 0);
    stmt_start = (stmt_start == std::string::npos) ? 0 : stmt_start + 1;
    const std::string lhs = trimWhitespace(code.substr(stmt_start, lhs_end - stmt_start));
    const std::string ptr = trimWhitespace(code.substr(call->open + 1, commas[0] - call->open - 1));
    const std::string cmp = trimWhitespace(code.substr(commas[0] + 1, commas[1] - commas[0] - 1));
    const std::string val = trimWhitespace(code.substr(commas[1] + 1, call->close - commas[1] - 1));

    const std::string tmp = "__cle_cmpxchg_expected_" + std::to_string(stmt_start);
    const std::string replacement = "{ auto " + tmp + " = " + cmp +
                                    "; atomic_compare_exchange_weak_explicit((volatile device atomic_uint*)" + ptr + ", &" + tmp + ", " +
                                    val + ", memory_order_relaxed, memory_order_relaxed); " + lhs + " = " + tmp + "; }";
    code.replace(stmt_start, semi + 1 - stmt_start, replacement);
    pos = stmt_start + replacement.size();
  }
}

/// atomic_op(&p, v) -> atomic_op_explicit((volatile device atomic_int*)&p, v, memory_order_relaxed)
auto
translateAtomics(std::string & code) -> void
{
  // atomic_cmpxchg is handled by translateCompareExchange
  static const Replacements functions = {
    { "atomic_add(", "atomic_fetch_add_explicit(" }, { "atomic_sub(", "atomic_fetch_sub_explicit(" },
    { "atomic_min(", "atomic_fetch_min_explicit(" }, { "atomic_max(", "atomic_fetch_max_explicit(" },
    { "atomic_and(", "atomic_fetch_and_explicit(" }, { "atomic_or(", "atomic_fetch_or_explicit(" },
    { "atomic_xor(", "atomic_fetch_xor_explicit(" }, { "atomic_xchg(", "atomic_exchange_explicit(" },
  };
  for (const auto & [from, to] : functions)
  {
    size_t pos = 0;
    while (const auto call = nextCall(code, from, pos))
    {
      const auto commas = topLevelCommas(code, call->open + 1, call->close);
      if (commas.empty())
      {
        pos = call->start + from.size();
        continue;
      }
      const std::string replacement = to + "(volatile device atomic_int*)" + code.substr(call->open + 1, commas[0] - call->open - 1) +
                                      code.substr(commas[0], call->close - commas[0]) + ", memory_order_relaxed)";
      code.replace(call->start, call->close + 1 - call->start, replacement);
      pos = call->start + replacement.size();
    }
  }
}

/// as_float / as_uint / as_int (bit reinterpretation, used by float-atomic CAS loops) -> as_type<T>
auto
translateBitcast(std::string & code) -> void
{
  replacePrefix(code, "as_float(", "as_type<float>(");
  replacePrefix(code, "as_uint(", "as_type<uint>(");
  replacePrefix(code, "as_int(", "as_type<int>(");
}

/// printf(...); statements are dropped: MSL's printf differs, and upstream kernels only use it as a debug aid
auto
translateRemovePrintf(std::string & code) -> void
{
  const std::string marker = "printf(";
  size_t            pos = 0;
  while (const auto call = nextCall(code, marker, pos))
  {
    pos = call->start;
    const size_t next = code.find_first_not_of(" \t\r\n\f\v", call->close + 1);
    const size_t end = (next != std::string::npos && code[next] == ';') ? next + 1 : call->close + 1;
    code.erase(call->start, end - call->start);
  }
}

auto
translateMathFunctions(std::string & code) -> void
{
  // cbrt / pow: preamble helpers. sqrt needs metal:: (native_execute bypasses the preamble, no macro possible)
  replaceWord(code, "cbrt", "__cle_cbrt");
  replaceWord(code, "pow", "__cle_pow");
  replaceWord(code, "sqrt", "metal::sqrt");

  replaceEach(code,
              { // affine transform boundary check: preamble macro with a built-in tolerance
                { "x2 >= 0 && y2 >= 0 && z2 >= 0 && x2 < Nx && y2 < Ny && z2 < Nz", "AFFINE_BOUNDS_CHECK(x2, y2, z2, Nx, Ny, Nz)" },
                { "POS_src_INSTANCE(x2, y2, z2, 0)", "POS_src_INSTANCE(x2 + 1e-5f, y2 + 1e-5f, z2 + 1e-5f, 0)" },
                { "native_", "fast::" },
                { "half_", "fast::" } });
}

auto
cleanup(std::string & code) -> void
{
  replaceEach(code,
              { { "device device ", "device " }, { "threadgroup threadgroup ", "threadgroup " }, { "constant constant ", "constant " } });
  collapseWhitespace(code);
}

} // namespace

auto
OpenCLToMetalTranslator::translate(const std::string & openclSource) const -> std::string
{
  std::string code = openclSource;
  translateInPlace(code);
  return code;
}

auto
OpenCLToMetalTranslator::translateInPlace(std::string & code) const -> void
{
  replaceAll(code, "#pragma OPENCL", "// #pragma OPENCL");
  translateSamplers(code);
  replaceEach(
    code, { { "__kernel void", "kernel void" }, { "__attribute__((reqd_work_group_size(", "// __attribute__((reqd_work_group_size(" } });
  translateAddressSpaces(code);
  translateVectorConstructors(code);
  appendBuiltinKernelArgs(code);
  translateKernelScalarArgs(code);
  translateWorkItemFunctions(code);
  translateSynchronization(code);
  translateCompareExchange(code);
  translateAtomics(code);
  translateBitcast(code);
  translateRemovePrintf(code);
  translateMathFunctions(code);
  cleanup(code);
}
