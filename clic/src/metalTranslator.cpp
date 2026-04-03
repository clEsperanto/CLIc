#include "translator.hpp"

#include <cctype>
#include <sstream>
#include <vector>

static auto
isWordChar(char c) -> bool
{
  return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

static auto
trimWhitespace(const std::string & s) -> std::string
{
  size_t b = 0;
  while (b < s.size() && std::isspace(static_cast<unsigned char>(s[b])))
    ++b;
  size_t e = s.size();
  while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1])))
    --e;
  return s.substr(b, e - b);
}

// Returns true when `token` is one of the recognized scalar type names.
static auto
isScalarType(const std::string & token) -> bool
{
  static const char * types[] = { "float", "int", "uint", "short", "ushort",
                                  "char",  "uchar", "long", "ulong", "half", "double" };
  for (const auto * t : types)
    if (token == t)
      return true;
  return false;
}

// Try to parse a scalar parameter "[const] <type> <name> [suffix]".
// Returns true on success, filling isConst/type/name/suffix.
static auto
parseScalarParam(const std::string & trimmed, bool & isConst, std::string & type, std::string & name, std::string & suffix) -> bool
{
  if (trimmed.find('*') != std::string::npos || trimmed.find('[') != std::string::npos ||
      trimmed.find("[[") != std::string::npos)
    return false;

  // Tokenize on whitespace.
  std::vector<std::string> tokens;
  {
    size_t i = 0;
    while (i < trimmed.size())
    {
      while (i < trimmed.size() && std::isspace(static_cast<unsigned char>(trimmed[i])))
        ++i;
      if (i >= trimmed.size())
        break;
      size_t start = i;
      while (i < trimmed.size() && !std::isspace(static_cast<unsigned char>(trimmed[i])))
        ++i;
      tokens.push_back(trimmed.substr(start, i - start));
    }
  }
  if (tokens.empty())
    return false;

  size_t idx = 0;
  isConst = false;
  if (tokens[idx] == "const")
  {
    isConst = true;
    ++idx;
  }

  // Type: either a single word ("float") or "unsigned"/"signed" + word.
  if (idx >= tokens.size())
    return false;

  if (tokens[idx] == "unsigned" || tokens[idx] == "signed")
  {
    if (idx + 1 >= tokens.size())
      return false;
    const std::string & second = tokens[idx + 1];
    if (second == "int" || second == "short" || second == "char" || second == "long")
    {
      type = tokens[idx] + " " + second;
      idx += 2;
    }
    else
      return false;
  }
  else if (isScalarType(tokens[idx]))
  {
    type = tokens[idx];
    ++idx;
  }
  else
    return false;

  // Name: must be an identifier.
  if (idx >= tokens.size())
    return false;
  name = tokens[idx];
  if (name.empty() || !(std::isalpha(static_cast<unsigned char>(name[0])) || name[0] == '_'))
    return false;
  ++idx;

  // Remaining tokens become the suffix.
  suffix.clear();
  for (; idx < tokens.size(); ++idx)
  {
    if (!suffix.empty())
      suffix += ' ';
    suffix += tokens[idx];
  }
  return true;
}

// Insert "thread " before IMAGE_xxx_PIXEL_TYPE when followed by '*'.
// Replaces the regex: \b(IMAGE_[A-Za-z0-9_]+_PIXEL_TYPE)\s*\*
static auto
addThreadToPixelTypePtrs(std::string & code) -> void
{
  const std::string tag = "_PIXEL_TYPE";
  size_t            pos = 0;
  while ((pos = code.find(tag, pos)) != std::string::npos)
  {
    // Walk back to find the start of "IMAGE_..."
    size_t start = pos;
    while (start > 0 && (std::isalnum(static_cast<unsigned char>(code[start - 1])) || code[start - 1] == '_'))
      --start;

    // Check it begins with "IMAGE_"
    if (code.compare(start, 6, "IMAGE_") != 0)
    {
      pos += tag.size();
      continue;
    }

    // Check word boundary before the identifier.
    if (start > 0 && isWordChar(code[start - 1]))
    {
      pos += tag.size();
      continue;
    }

    size_t afterTag = pos + tag.size();
    // Skip whitespace after _PIXEL_TYPE
    size_t starPos = afterTag;
    while (starPos < code.size() && code[starPos] == ' ')
      ++starPos;

    if (starPos < code.size() && code[starPos] == '*')
    {
      // Already has "thread " prefix?
      const std::string prefix = "thread ";
      if (start >= prefix.size() && code.compare(start - prefix.size(), prefix.size(), prefix) == 0)
      {
        pos = starPos + 1;
        continue;
      }
      code.insert(start, prefix);
      pos = starPos + prefix.size() + 1;
    }
    else
    {
      pos = afterTag;
    }
  }
}

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
  translatePragmas(code);
  translateSamplers(code);
  translateQualifiers(code);
  translateAddressSpaces(code);
  translateVectorConstructors(code);
  appendMetalBuiltinKernelArgs(code);
  translateKernelScalarArgs(code);
  translateWorkItemFunctions(code);
  translateSynchronization(code);
  translateMathFunctions(code);
  cleanupCode(code);
}

auto
OpenCLToMetalTranslator::replaceAll(std::string & str, const std::string & from, const std::string & to) -> void
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
OpenCLToMetalTranslator::replaceWord(std::string & str, const std::string & from, const std::string & to) -> void
{
  if (from.empty())
    return;

  size_t pos = 0;
  while ((pos = str.find(from, pos)) != std::string::npos)
  {
    const bool wordBefore = (pos > 0 && isWordChar(str[pos - 1]));
    const bool wordAfter = (pos + from.size() < str.size() && isWordChar(str[pos + from.size()]));

    if (!wordBefore && !wordAfter)
    {
      str.replace(pos, from.size(), to);
      pos += to.size();
    }
    else
    {
      pos += from.size();
    }
  }
}

auto
OpenCLToMetalTranslator::collapseSpaces(std::string & code) -> void
{
  size_t write = 0;
  bool   prevSpace = false;
  for (size_t read = 0; read < code.size(); ++read)
  {
    if (code[read] == ' ')
    {
      if (!prevSpace)
      {
        code[write++] = ' ';
        prevSpace = true;
      }
    }
    else
    {
      code[write++] = code[read];
      prevSpace = false;
    }
  }
  code.resize(write);
}

auto
OpenCLToMetalTranslator::collapseNewlines(std::string & code) -> void
{
  size_t write = 0;
  size_t nlCount = 0;
  for (size_t read = 0; read < code.size(); ++read)
  {
    if (code[read] == '\n')
    {
      ++nlCount;
      if (nlCount <= 2)
      {
        code[write++] = '\n';
      }
    }
    else
    {
      nlCount = 0;
      code[write++] = code[read];
    }
  }
  code.resize(write);
}

auto
OpenCLToMetalTranslator::appendMetalBuiltinKernelArgs(std::string & code) -> void
{
  const std::string marker = "kernel void";
  size_t            pos = 0;

  while ((pos = code.find(marker, pos)) != std::string::npos)
  {
    size_t parenStart = code.find('(', pos + marker.size());
    if (parenStart == std::string::npos)
      break;

    int    depth = 1;
    size_t i = parenStart + 1;
    while (i < code.size() && depth > 0)
    {
      if (code[i] == '(')
        ++depth;
      else if (code[i] == ')')
        --depth;
      ++i;
    }

    if (depth != 0)
    {
      pos += marker.size();
      continue;
    }

    const size_t      parenEnd = i - 1;
    const std::string params = code.substr(parenStart + 1, parenEnd - parenStart - 1);
    if (params.find("__cle_gid") != std::string::npos)
    {
      pos = parenEnd + 1;
      continue;
    }

    std::string builtins =
      "uint3 __cle_gid [[thread_position_in_grid]], "
      "uint3 __cle_tid [[thread_position_in_threadgroup]], "
      "uint3 __cle_tgid [[threadgroup_position_in_grid]], "
      "uint3 __cle_tptg [[threads_per_threadgroup]], "
      "uint3 __cle_tgpg [[threadgroups_per_grid]]";

    std::string insertion;
    if (params.find_first_not_of(" \t\n\r") == std::string::npos)
      insertion = builtins;
    else
      insertion = ", " + builtins;

    code.insert(parenEnd, insertion);
    pos = parenEnd + insertion.size() + 1;
  }
}

auto
OpenCLToMetalTranslator::translateKernelScalarArgs(std::string & code) -> void
{
  // In Metal, scalar kernel parameters are passed by reference from constant
  // address space. For non-const OpenCL scalars we preserve by-value semantics
  // by binding a hidden constant ref argument and creating a mutable local copy
  // at kernel entry.
  const std::string marker = "kernel void";
  size_t            pos = 0;

  while ((pos = code.find(marker, pos)) != std::string::npos)
  {
    size_t parenStart = code.find('(', pos + marker.size());
    if (parenStart == std::string::npos)
      break;

    int    depth = 1;
    size_t i = parenStart + 1;
    while (i < code.size() && depth > 0)
    {
      if (code[i] == '(')
        ++depth;
      else if (code[i] == ')')
        --depth;
      ++i;
    }
    if (depth != 0)
    {
      pos += marker.size();
      continue;
    }

    const size_t parenEnd = i - 1;
    std::string  params = code.substr(parenStart + 1, parenEnd - parenStart - 1);

    // Drop inline `// ...` comments so comma-splitting of parameter lists is
    // not confused by commas inside comments.
    {
      std::string cleaned;
      cleaned.reserve(params.size());
      bool inLineComment = false;
      for (size_t k = 0; k < params.size(); ++k)
      {
        const char c = params[k];
        if (!inLineComment && c == '/' && (k + 1) < params.size() && params[k + 1] == '/')
        {
          inLineComment = true;
          ++k;
          continue;
        }
        if (inLineComment)
        {
          if (c == '\n')
          {
            inLineComment = false;
            cleaned.push_back(c);
          }
          continue;
        }
        cleaned.push_back(c);
      }
      params.swap(cleaned);
    }

    std::vector<std::string> mutableScalarLocals;
    std::vector<std::string> parts;
    size_t                   start = 0;
    while (start <= params.size())
    {
      size_t comma = params.find(',', start);
      if (comma == std::string::npos)
      {
        parts.push_back(params.substr(start));
        break;
      }
      parts.push_back(params.substr(start, comma - start));
      start = comma + 1;
    }

    std::string newParams;
    bool        changed = false;
    for (size_t p = 0; p < parts.size(); ++p)
    {
      std::string param = parts[p];
      std::string trimmed = param;
      trimmed = trimWhitespace(trimmed);

      bool        scalarConst = false;
      std::string scalarType, scalarName, scalarSuffix;
      if (parseScalarParam(trimmed, scalarConst, scalarType, scalarName, scalarSuffix))
      {
        if (scalarConst)
        {
          param = "constant " + scalarType + "& " + scalarName;
          if (!scalarSuffix.empty())
            param += " " + scalarSuffix;
          changed = true;
        }
        else
        {
          param = "constant " + scalarType + "& __cle_arg_" + scalarName;
          if (!scalarSuffix.empty())
            param += " " + scalarSuffix;
          mutableScalarLocals.push_back(scalarType + " " + scalarName + " = __cle_arg_" + scalarName + ";");
          changed = true;
        }
      }

      if (p != 0)
        newParams += ", ";
      newParams += param;
    }

    if (changed)
    {
      code.replace(parenStart + 1, parenEnd - parenStart - 1, newParams);

      size_t searchFrom = parenStart + 1 + newParams.size() + 1;
      if (!mutableScalarLocals.empty())
      {
        size_t bodyStart = code.find('{', searchFrom);
        if (bodyStart != std::string::npos)
        {
          std::string localsBlock = "\n";
          for (size_t l = 0; l < mutableScalarLocals.size(); ++l)
          {
            localsBlock += " " + mutableScalarLocals[l] + "\n";
          }
          code.insert(bodyStart + 1, localsBlock);
          searchFrom = bodyStart + 1 + localsBlock.size();
        }
      }

      pos = searchFrom;
    }
    else
    {
      pos = parenEnd + 1;
    }
  }
}

auto
OpenCLToMetalTranslator::translatePragmas(std::string & code) -> void
{
  replaceAll(code, "#pragma OPENCL", "// #pragma OPENCL");
}

auto
OpenCLToMetalTranslator::translateSamplers(std::string & code) -> void
{
  // The Metal preamble already defines sampler_t as int and CLK_* as integer
  // constants, so only the address space qualifier needs fixing here.
  replaceAll(code, "__constant sampler_t", "constant sampler_t");
  replaceAll(code, "const sampler_t", "constant sampler_t");

  // Rename any standalone variable named 'sampler' to avoid ambiguity with
  // metal::sampler (from <metal_stdlib>). replaceWord respects word boundaries
  // so sampler_t is not affected.
  replaceWord(code, "sampler", "__cle_sampler");
}

auto
OpenCLToMetalTranslator::translateQualifiers(std::string & code) -> void
{
  replaceAll(code, "__kernel void", "kernel void");
  replaceAll(code, "__attribute__((reqd_work_group_size(", "// __attribute__((reqd_work_group_size(");
}

auto
OpenCLToMetalTranslator::translateAddressSpaces(std::string & code) -> void
{
  replaceAll(code, "__global ", "device ");
  replaceAll(code, " global ", " device ");

  replaceAll(code, "__local ", "threadgroup ");
  replaceAll(code, " local ", " threadgroup ");

  replaceAll(code, "__constant ", "constant ");

  replaceAll(code, "__private ", "");

  // Helper functions that receive stack arrays via IMAGE_*_PIXEL_TYPE pointers
  // need an explicit address space in MSL.
  addThreadToPixelTypePtrs(code);
  replaceAll(code, " private ", " ");

  // Add 'device' address space to IMAGE_*_TYPE pointer macros.
  // These macros expand to kernel parameters and MSL requires an explicit
  // address space qualifier on all pointer types.
  {
    std::istringstream stream(code);
    std::ostringstream result;
    std::string        line;
    while (std::getline(stream, line))
    {
      if (line.find("#define IMAGE_") != std::string::npos
          && line.find("_TYPE") != std::string::npos
          && line.find('*') != std::string::npos
          && line.find("device ") == std::string::npos
          && line.find("threadgroup ") == std::string::npos
          && line.find("constant ") == std::string::npos)
      {
        size_t typeNameEnd = line.find("_TYPE");
        if (typeNameEnd != std::string::npos)
        {
          typeNameEnd += 5; // skip "_TYPE"
          while (typeNameEnd < line.size() && line[typeNameEnd] == ' ')
            ++typeNameEnd;
          line.insert(typeNameEnd, "device ");
        }
      }
      result << line << "\n";
    }
    code = result.str();
  }
}

auto
OpenCLToMetalTranslator::translateVectorConstructors(std::string & code) -> void
{
  // OpenCL vector literals often use `(int3)(x, y, z)` syntax. In MSL this
  // can be parsed differently; normalize to constructor form `int3(x, y, z)`.
  static const char * prefixes[] = {
    "char", "uchar", "short", "ushort", "int", "uint", "long", "ulong", "float", "half", "double"
  };
  for (size_t i = 0; i < sizeof(prefixes) / sizeof(prefixes[0]); ++i)
  {
    for (int n = 2; n <= 4; ++n)
    {
      const std::string opencl = "(" + std::string(prefixes[i]) + std::to_string(n) + ")(";
      const std::string metal = std::string(prefixes[i]) + std::to_string(n) + "(";
      replaceAll(code, opencl, metal);
    }
  }
}

auto
OpenCLToMetalTranslator::translateWorkItemFunctions(std::string & code) -> void
{
  replaceAll(code, "get_global_id(0)", "((int)__cle_gid.x)");
  replaceAll(code, "get_global_id(1)", "((int)__cle_gid.y)");
  replaceAll(code, "get_global_id(2)", "((int)__cle_gid.z)");

  replaceAll(code, "get_local_id(0)", "((int)__cle_tid.x)");
  replaceAll(code, "get_local_id(1)", "((int)__cle_tid.y)");
  replaceAll(code, "get_local_id(2)", "((int)__cle_tid.z)");

  replaceAll(code, "get_group_id(0)", "((int)__cle_tgid.x)");
  replaceAll(code, "get_group_id(1)", "((int)__cle_tgid.y)");
  replaceAll(code, "get_group_id(2)", "((int)__cle_tgid.z)");

  replaceAll(code, "get_local_size(0)", "((int)__cle_tptg.x)");
  replaceAll(code, "get_local_size(1)", "((int)__cle_tptg.y)");
  replaceAll(code, "get_local_size(2)", "((int)__cle_tptg.z)");

  replaceAll(code, "get_num_groups(0)", "((int)__cle_tgpg.x)");
  replaceAll(code, "get_num_groups(1)", "((int)__cle_tgpg.y)");
  replaceAll(code, "get_num_groups(2)", "((int)__cle_tgpg.z)");

  replaceAll(code, "get_global_size(0)", "((int)(__cle_tptg.x * __cle_tgpg.x))");
  replaceAll(code, "get_global_size(1)", "((int)(__cle_tptg.y * __cle_tgpg.y))");
  replaceAll(code, "get_global_size(2)", "((int)(__cle_tptg.z * __cle_tgpg.z))");
}

auto
OpenCLToMetalTranslator::translateSynchronization(std::string & code) -> void
{
  replaceAll(code, "barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE)", "threadgroup_barrier(mem_flags::mem_threadgroup)");
  replaceAll(code, "barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE)", "threadgroup_barrier(mem_flags::mem_threadgroup)");
  replaceAll(code, "barrier(CLK_LOCAL_MEM_FENCE)", "threadgroup_barrier(mem_flags::mem_threadgroup)");
  replaceAll(code, "barrier(CLK_GLOBAL_MEM_FENCE)", "threadgroup_barrier(mem_flags::mem_device)");

  replaceAll(code, "mem_fence(CLK_LOCAL_MEM_FENCE)", "threadgroup_barrier(mem_flags::mem_threadgroup)");
  replaceAll(code, "mem_fence(CLK_GLOBAL_MEM_FENCE)", "threadgroup_barrier(mem_flags::mem_device)");
  replaceAll(code, "read_mem_fence(CLK_LOCAL_MEM_FENCE)", "threadgroup_barrier(mem_flags::mem_threadgroup)");
  replaceAll(code, "read_mem_fence(CLK_GLOBAL_MEM_FENCE)", "threadgroup_barrier(mem_flags::mem_device)");
  replaceAll(code, "write_mem_fence(CLK_LOCAL_MEM_FENCE)", "threadgroup_barrier(mem_flags::mem_threadgroup)");
  replaceAll(code, "write_mem_fence(CLK_GLOBAL_MEM_FENCE)", "threadgroup_barrier(mem_flags::mem_device)");
}



auto
OpenCLToMetalTranslator::translateMathFunctions(std::string & code) -> void
{
  // Map cbrt/pow to preamble helpers. replaceWord respects word boundaries
  // so __cle_cbrt/__cle_pow are not re-matched (leading _ is a word char).
  replaceWord(code, "cbrt", "__cle_cbrt");
  replaceWord(code, "pow", "__cle_pow");

  // sqrt needs metal:: qualification. native_execute() bypasses the preamble,
  // so this cannot be a preamble-only macro.
  replaceWord(code, "sqrt", "metal::sqrt");

  // Affine-transform boundary check: use preamble macro with built-in tolerance.
  replaceAll(code,
             "x2 >= 0 && y2 >= 0 && z2 >= 0 && x2 < Nx && y2 < Ny && z2 < Nz",
             "AFFINE_BOUNDS_CHECK(x2, y2, z2, Nx, Ny, Nz)");

  replaceAll(code,
             "POS_src_INSTANCE(x2, y2, z2, 0)",
             "POS_src_INSTANCE(x2 + 1e-5f, y2 + 1e-5f, z2 + 1e-5f, 0)");

  replaceAll(code, "native_", "fast::");
  replaceAll(code, "half_", "fast::");
}

auto
OpenCLToMetalTranslator::cleanupCode(std::string & code) -> void
{
  replaceAll(code, "device device ", "device ");
  replaceAll(code, "threadgroup threadgroup ", "threadgroup ");
  replaceAll(code, "constant constant ", "constant ");

  collapseSpaces(code);
  collapseNewlines(code);
}
