#include "execution.hpp"
#include "backend.hpp"
#include "clic.hpp"
#include "tier0.hpp"
#include <cctype>
#include <cstdio>
#include <cstring>
#include <limits>
#include <mutex>
#include <set>
#include <type_traits>

namespace cle
{
namespace
{
static const std::unordered_map<int, std::string> dimension_defines = { { 1, "#define USE_1D" },
                                                                        { 2, "#define USE_2D" },
                                                                        { 3, "#define USE_3D" } };

static const std::unordered_map<dType, std::string> dtype_defines = {
  { dType::INT8, "#define USE_CHAR" },     { dType::UINT8, "#define USE_UCHAR" },  { dType::INT16, "#define USE_SHORT" },
  { dType::UINT16, "#define USE_USHORT" }, { dType::INT32, "#define USE_INT" },    { dType::UINT32, "#define USE_UINT" },
  { dType::FLOAT, "#define USE_FLOAT" },   { dType::COMPLEX, "#define USE_FLOAT" }
};
} // namespace


// The defines are built with plain string appends: std::ostringstream formatting was measured to be ~50% of the
// host time of a kernel launch. The generated text must stay byte-identical (it is part of the cache keys).

// Append several strings (std::string or const char *) to `out`
template <typename... Parts>
static auto
append(std::string & out, const Parts &... parts) -> void
{
  (out.append(parts), ...);
}

// Append a constant value, formatted like std::ostream would (floats: "%g", i.e. 6 significant digits)
static auto
appendConstant(std::string & out, const ConstantType & value) -> void
{
  std::visit(
    [&out](const auto & v) {
      using T = std::decay_t<decltype(v)>;
      if constexpr (std::is_same_v<T, std::string>)
      {
        out += v;
      }
      else if constexpr (std::is_same_v<T, float>)
      {
        std::array<char, 32> buffer{};
        const int            length = std::snprintf(buffer.data(), buffer.size(), "%g", static_cast<double>(v));
        out.append(buffer.data(), static_cast<size_t>(length));
      }
      else
      {
        out += std::to_string(v);
      }
    },
    value);
}

// Function creating common defines for constants
static auto
commonDefines(std::string & defines, const ConstantList & constant_list) -> void
{
  for (const auto & item : constant_list)
  {
    append(defines, "#define ", item.first, " ");
    appendConstant(defines, item.second);
    defines += '\n';
  }
  defines += "\n";
  defines += "\n#define GET_IMAGE_WIDTH(image_key) IMAGE_SIZE_ ## image_key ## _WIDTH";
  defines += "\n#define GET_IMAGE_HEIGHT(image_key) IMAGE_SIZE_ ## image_key ## _HEIGHT";
  defines += "\n#define GET_IMAGE_DEPTH(image_key) IMAGE_SIZE_ ## image_key ## _DEPTH";
  defines += "\n";
}


// Function creating buffer specific defines
static auto
bufferDefines(std::string & defines, const std::string & key, const Array::Pointer & arr, const size_t & dim, const Device::Type & device)
  -> void
{
  static constexpr std::array<const char *, 3> ndimMap = { "1", "2", "3" };
  static constexpr std::array<const char *, 3> posTypeMap = { "int", "int2", "int4" };
  static constexpr std::array<const char *, 3> posMap = { "(pos0)", "(pos0, pos1)", "(pos0, pos1, pos2, 0)" };

  const size_t      dimIndex = dim - 1;
  const char *      ndim = ndimMap[dimIndex];
  const std::string pos_type = posTypeMap[dimIndex];
  const char *      pos = posMap[dimIndex];
  const std::string stype = toShortString(arr->dtype());
  const std::string dtype = toString(arr->dtype());
  const char *      access_type = (device == Device::Type::OPENCL) ? "__global " : "";
  const std::string prefix = (device == Device::Type::OPENCL || pos_type == "int") ? "(" + pos_type + ")" : "make_" + pos_type;

  append(defines, "\n#define CONVERT_", key, "_PIXEL_TYPE clij_convert_", dtype, "_sat");
  append(defines, "\n#define IMAGE_", key, "_PIXEL_TYPE ", dtype);
  append(defines, "\n#define POS_", key, "_TYPE ", pos_type);
  append(defines, "\n#define POS_", key, "_INSTANCE(pos0,pos1,pos2,pos3) ", prefix, pos);
  defines += "\n";

  append(defines, "\n#define IMAGE_", key, "_TYPE ", access_type, dtype, "*");
  append(defines,
         "\n#define READ_",
         key,
         "_IMAGE(a,b,c) read_buffer",
         ndim,
         "d",
         stype,
         "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)");
  append(defines,
         "\n#define WRITE_",
         key,
         "_IMAGE(a,b,c) write_buffer",
         ndim,
         "d",
         stype,
         "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)");
}


// Function creating image specific defines (OpenCL only for now)
static auto
imageDefines(std::string & defines, const std::string & key, const Array::Pointer & arr, const size_t & dim, const Device::Type & device)
  -> void
{
  static constexpr std::array<const char *, 3> ndimMap = { "1", "2", "3" };
  static constexpr std::array<const char *, 3> posIntTypeMap = { "int", "int2", "int4" };
  static constexpr std::array<const char *, 3> posFloatTypeMap = { "float", "float2", "float4" };
  static constexpr std::array<const char *, 3> posMap = { "(pos0)", "(pos0, pos1)", "(pos0, pos1, pos2, 0)" };

  const size_t      dimIndex = dim - 1;
  const char *      ndim = ndimMap[dimIndex];
  const char *      pos = posMap[dimIndex];
  const std::string stype = toShortString(arr->dtype());
  const std::string dtype = toString(arr->dtype());

  const bool write_only =
    key.find("dst") != std::string::npos || key.find("destination") != std::string::npos || key.find("output") != std::string::npos;
  const char *      access_type = write_only ? "__write_only" : "__read_only";
  const std::string pos_type = write_only ? posIntTypeMap[dimIndex] : posFloatTypeMap[dimIndex];
  const std::string prefix1 = (device == Device::Type::OPENCL || pos_type == "int") ? "(" + pos_type + ")" : "make_" + pos_type;

  append(defines, "\n#define CONVERT_", key, "_PIXEL_TYPE clij_convert_", dtype, "_sat");
  append(defines, "\n#define IMAGE_", key, "_PIXEL_TYPE ", dtype);
  append(defines, "\n#define POS_", key, "_TYPE ", pos_type);
  append(defines, "\n#define POS_", key, "_INSTANCE(pos0,pos1,pos2,pos3) ", prefix1, pos);
  defines += "\n";

  const char   front_char = stype.front();
  const char * prefix2 = (front_char == 'u') ? "ui" : (front_char == 'f') ? "f" : "i";
  append(defines, "\n#define IMAGE_", key, "_TYPE ", access_type, " image", ndim, "d_t");
  append(defines, "\n#define READ_", key, "_IMAGE(a,b,c) read_image", prefix2, "(a,b,c)");
  append(defines, "\n#define WRITE_", key, "_IMAGE(a,b,c) write_image", prefix2, "(a,b,c)");
}

static auto
amdPlatform(const Device::Pointer & device) -> bool
{
  return device->getPlatform().find("AMD") != std::string::npos;
}

// Function for creating defines for each array parameters
static auto
arrayDefines(std::string & defines, const ParameterList & parameter_list, const Device::Type & device) -> void
{
  // loop over all parameters, skip if parameter is not an array
  for (const auto & param : parameter_list)
  {
    const auto * arr_ptr = std::get_if<Array::Pointer>(&param.second);
    if (arr_ptr == nullptr)
    {
      continue;
    }
    const auto & arr = *arr_ptr;
    const auto & key = param.first;
    if (arr == nullptr)
    {
      throw std::invalid_argument("Error: Array parameter '" + key + "' is null");
    }

    // manage array dimension
    auto dim = shape_to_dimension(arr->width(), arr->height(), arr->depth());
    // manage array type (buffer or image), and read/write macros
    if (arr->mtype() == mType::BUFFER || device == Device::Type::CUDA)
    {
      bufferDefines(defines, key, arr, dim, device);
    }
    else
    {
      imageDefines(defines, key, arr, dim, device);
    }

    // manage array size
    defines += "\n";
    append(defines, "\n#define IMAGE_SIZE_", key, "_WIDTH ", std::to_string(arr->width()));
    append(defines, "\n#define IMAGE_SIZE_", key, "_HEIGHT ", std::to_string(arr->height()));
    append(defines, "\n#define IMAGE_SIZE_", key, "_DEPTH ", std::to_string(arr->depth()));
    defines += "\n\n";
  }
}


// Top function for creating defines at runtime
auto
generateDefines(const ParameterList & parameter_list, const ConstantList & constant_list, const Device::Type & device) -> std::string
{
  std::string defines;
  defines.reserve(1024 + 1024 * parameter_list.size());
  commonDefines(defines, constant_list);
  arrayDefines(defines, parameter_list, device);
  return defines;
}


// ============================================================================
// Program source cache
// ============================================================================

// The program source passed to the backend (defines + preamble + kernel) only depends on the "launch signature":
// kernel, backend, and the name, type, memory type and shape of each array parameter (sizes are baked into the
// defines), plus the constants. Scalar values are kernel arguments and do not change the source. Caching the source
// per signature skips the defines generation and the source assembly on every launch with the same signature.
// The cache is shared by all devices (the source does not depend on the device beyond the backend and platform).
// The signature is also given to the backend as cache_key, so that it does not hash the whole source again.

namespace
{
struct SourceCache
{
  std::mutex   mutex;
  ProgramCache cache; // LRU, values are std::shared_ptr<const std::string>
};

auto
sourceCache() -> SourceCache &
{
  static SourceCache instance;
  return instance;
}
} // namespace

// Build the launch signature. Every input of the source generation must be part of it.
static auto
launchSignature(const Device::Pointer & device,
                const KernelInfo &      kernel_func,
                const ParameterList &   parameters,
                const ConstantList &    constants) -> std::string
{
  constexpr char field = '\x1f';
  constexpr char record = '\x1e';

  std::string key;
  key.reserve(128 + 64 * parameters.size());
  append(key, kernel_func.first, std::string(1, field));
  append(key, std::to_string(std::hash<std::string>{}(kernel_func.second)), "-", std::to_string(kernel_func.second.size()));
  key += field;
  key += std::to_string(static_cast<int>(device->getType()));
  key += amdPlatform(device) ? "A" : "-";

  for (const auto & [name, value] : parameters)
  {
    key += record;
    key += name;
    key += field;
    key += std::to_string(value.index());
    if (const auto * arr = std::get_if<Array::Pointer>(&value))
    {
      if (*arr == nullptr)
      {
        throw std::invalid_argument("Error: Array parameter '" + name + "' is null");
      }
      const auto & a = *arr;
      for (const size_t v : { static_cast<size_t>(a->dtype()),
                              static_cast<size_t>(a->mtype()),
                              a->width(),
                              a->height(),
                              a->depth(),
                              static_cast<size_t>(a->dim()) })
      {
        key += field;
        key += std::to_string(v);
      }
    }
  }
  for (const auto & [name, value] : constants)
  {
    key += record;
    append(key, name, std::string(1, field));
    appendConstant(key, value);
  }
  return key;
}

// Generate the full program source: defines + backend preamble + kernel
static auto
generateProgramSource(const Device::Pointer & device,
                      const KernelInfo &      kernel_func,
                      const ParameterList &   parameters,
                      const ConstantList &    constants) -> std::string
{
  auto kernel_preamble = cle::BackendManager::getInstance().getBackend().getPreamble();
  auto defines = generateDefines(parameters, constants, device->getType());
  if (amdPlatform(device))
  {
    kernel_preamble = "#pragma OPENCL EXTENSION cl_amd_printf : enable\n" + kernel_preamble;
  }

  // Collect unique array dtypes and dimensions for CLIJ defines
  std::set<dType> used_dtypes;
  std::set<int>   used_dimensions;
  for (const auto & [key, value] : parameters)
  {
    if (const auto * arr = std::get_if<Array::Pointer>(&value))
    {
      used_dtypes.insert((*arr)->dtype());
      used_dimensions.insert((*arr)->dim());
    }
  }
  for (const auto & dtype : used_dtypes)
  {
    append(defines, "\n", dtype_defines.at(dtype));
  }
  for (const auto & dim : used_dimensions)
  {
    append(defines, "\n", dimension_defines.at(dim));
  }
  defines += "\n\n";

  std::string program_source;
  program_source.reserve(defines.size() + kernel_preamble.size() + kernel_func.second.size());
  append(program_source, defines, kernel_preamble, kernel_func.second);
  return program_source;
}


// Marshal ParameterList into flat argument arrays for backend execution
static auto
marshalParameters(const ParameterList & parameters, std::vector<std::shared_ptr<void>> & args_ptr, std::vector<size_t> & args_size) -> void
{
  args_ptr.reserve(parameters.size());
  args_size.reserve(parameters.size());
  for (const auto & [key, value] : parameters)
  {
    if (const auto * arr = std::get_if<Array::Pointer>(&value))
    {
      if (*arr == nullptr)
      {
        throw std::invalid_argument("Error: Array parameter '" + key + "' is null");
      }
      if (!(*arr)->isContiguous() || (*arr)->offset() != 0)
      {
        throw std::runtime_error("Error: Array views are not yet supported in kernel execution (parameter '" + key + "')");
      }
      args_ptr.push_back((*arr)->get_ptr());
      args_size.push_back(GPU_MEM_PTR_SIZE);
    }
    else if (const auto * f = std::get_if<float>(&value))
    {
      args_ptr.push_back(std::static_pointer_cast<void>(std::make_shared<float>(*f)));
      args_size.push_back(sizeof(float));
    }
    else if (const auto * i = std::get_if<int>(&value))
    {
      args_ptr.push_back(std::static_pointer_cast<void>(std::make_shared<int>(*i)));
      args_size.push_back(sizeof(int));
    }
    else if (const auto * u = std::get_if<unsigned int>(&value))
    {
      args_ptr.push_back(std::static_pointer_cast<void>(std::make_shared<unsigned int>(*u)));
      args_size.push_back(sizeof(unsigned int));
    }
    else if (const auto * s = std::get_if<size_t>(&value))
    {
      // size_t is not a valid kernel argument type in OpenCL C, and its size equals the size of a device
      // pointer, which the backends use to identify memory arguments: pass it as a 32-bit unsigned int.
      if (*s > std::numeric_limits<unsigned int>::max())
      {
        throw std::out_of_range("Error: Parameter '" + key + "' (" + std::to_string(*s) + ") exceeds the unsigned int range");
      }
      args_ptr.push_back(std::static_pointer_cast<void>(std::make_shared<unsigned int>(static_cast<unsigned int>(*s))));
      args_size.push_back(sizeof(unsigned int));
    }
    else
    {
      throw std::runtime_error("Error: Invalid parameter type.");
    }
  }
}


auto
execute(const Device::Pointer & device,
        const KernelInfo &      kernel_func,
        const ParameterList &   parameters,
        const RangeArray &      global_range,
        const RangeArray &      local_range,
        const ConstantList &    constants) -> void
{
  // Program source from the cache when this launch signature was already seen, else generated and cached
  const auto                         signature = launchSignature(device, kernel_func, parameters, constants);
  std::shared_ptr<const std::string> program_source;
  auto &                             source_cache = sourceCache();
  {
    std::lock_guard<std::mutex> lock(source_cache.mutex);
    program_source = std::static_pointer_cast<const std::string>(source_cache.cache.get(signature));
  }
  if (program_source == nullptr)
  {
    program_source = std::make_shared<const std::string>(generateProgramSource(device, kernel_func, parameters, constants));
    std::lock_guard<std::mutex> lock(source_cache.mutex);
    source_cache.cache.put(signature, std::const_pointer_cast<std::string>(program_source));
  }

  std::vector<std::shared_ptr<void>> args_ptr;
  std::vector<size_t>                args_size;
  marshalParameters(parameters, args_ptr, args_size);

  cle::BackendManager::getInstance().getBackend().executeKernel(
    device, *program_source, kernel_func.first, global_range, local_range, args_ptr, args_size, signature);
}


auto
execute_separable(const Device::Pointer &      device,
                  const KernelInfo &           kernel,
                  const Array::Pointer &       src,
                  const Array::Pointer &       dst,
                  const std::array<float, 3> & sigma,
                  const std::array<int, 3> &   radius,
                  const std::array<int, 3> &   orders) -> void
{
  Array::check_ptr(src, "Error: 'src' is null. Please ensure the provided parameters before calling this function.");
  Array::check_ptr(dst, "Error: 'dst' is null. Please ensure the provided parameters before calling this function.");

  const RangeArray global_range = { dst->width(), dst->height(), dst->depth() };

  // 1D passes to run, along x, y, z: a dimension is processed if it is not flat and its sigma is positive
  const std::array<size_t, 3> extent = { dst->width(), dst->height(), dst->depth() };
  std::vector<int>            passes;
  for (int axis = 0; axis < 3; ++axis)
  {
    if (extent[axis] > 1 && sigma[axis] > 0)
    {
      passes.push_back(axis);
    }
  }
  if (passes.empty())
  {
    src->copyTo(dst);
    return;
  }

  // Ping-pong between dst and a single temporary, ordered so that the last pass writes into dst:
  // 1 pass: src -> dst; 2 passes: src -> tmp -> dst; 3 passes: src -> dst -> tmp -> dst.
  // A pass never reads and writes the same buffer: when src and dst share their memory, src is copied first.
  Array::Pointer input = src;
  if (src->get() == dst->get())
  {
    input = Array::create(dst);
    src->copyTo(input);
  }
  Array::Pointer tmp = (passes.size() > 1) ? Array::create(dst) : nullptr;
  if (passes.size() == 3 && input != src)
  {
    tmp = input; // the copy of src is free after the first pass: reuse it as the temporary
  }

  for (size_t i = 0; i < passes.size(); ++i)
  {
    const int  axis = passes[i];
    const bool to_dst = ((passes.size() - 1 - i) % 2) == 0;
    const auto output = to_dst ? dst : tmp;

    ParameterList parameters;
    parameters.emplace_back("src", input);
    parameters.emplace_back("dst", output);
    parameters.emplace_back("dim", axis);
    parameters.emplace_back("N", radius[axis]);
    parameters.emplace_back("s", sigma[axis]);
    parameters.emplace_back("order", orders[axis]);
    execute(device, kernel, parameters, global_range);
    input = output;
  }
}


auto
native_execute(const Device::Pointer & device,
               const KernelInfo &      kernel_func,
               const ParameterList &   parameters,
               const RangeArray &      global_range,
               const RangeArray &      local_range) -> void
{

  auto kernel_source = kernel_func.second;
  auto kernel_name = kernel_func.first;

  std::vector<std::shared_ptr<void>> args_ptr;
  std::vector<size_t>                args_size;
  marshalParameters(parameters, args_ptr, args_size);

  cle::BackendManager::getInstance().getBackend().executeKernel(
    device, kernel_source, kernel_name, global_range, local_range, args_ptr, args_size);
}


// Extract unique variable names from an expression, in order of first appearance.
// Skips numeric literals (including float suffixes like 2.0f) and known OpenCL builtins.
static auto
extractVariableNames(const std::string & expr) -> std::vector<std::string>
{
  static const std::set<std::string> builtins = {
    // Math functions
    "sin",
    "cos",
    "tan",
    "asin",
    "acos",
    "atan",
    "atan2",
    "sinh",
    "cosh",
    "tanh",
    "asinh",
    "acosh",
    "atanh",
    "exp",
    "exp2",
    "exp10",
    "log",
    "log2",
    "log10",
    "pow",
    "power",
    "pown",
    "powr",
    "sqrt",
    "rsqrt",
    "cbrt",
    "fabs",
    "abs",
    "fmin",
    "fmax",
    "fmod",
    "remainder",
    "ceil",
    "floor",
    "round",
    "trunc",
    "rint",
    "clamp",
    "mix",
    "step",
    "smoothstep",
    "sign",
    "min",
    "max",
    "mad",
    "fma",
    "copysign",
    "fdim",
    "hypot",
    "ldexp",
    "frexp",
    "native_sin",
    "native_cos",
    "native_exp",
    "native_log",
    "native_sqrt",
    "native_tan",
    "native_recip",
    "native_rsqrt",
    "native_powr",
    "half_sin",
    "half_cos",
    "half_exp",
    "half_log",
    "half_sqrt",
    "half_tan",
    "half_recip",
    "half_rsqrt",
    "half_powr",
    "isnan",
    "isinf",
    "isfinite",
    "isnormal",
    "signbit",
    "select",
    "bitselect",
    "convert_float",
    "convert_int",
    "convert_uint",
    "convert_char",
    "convert_uchar",
    "convert_short",
    "convert_ushort",
    "as_float",
    "as_int",
    "as_uint",
    // Types and keywords
    "float",
    "double",
    "half",
    "int",
    "uint",
    "char",
    "uchar",
    "short",
    "ushort",
    "long",
    "ulong",
    "bool",
    "void",
    "const",
    "unsigned",
    "return",
    "if",
    "else",
    "for",
    "while",
    "do",
    "break",
    "continue",
    "true",
    "false",
  };

  std::vector<std::string> vars;
  std::set<std::string>    seen;
  size_t                   i = 0;
  const size_t             len = expr.size();

  while (i < len)
  {
    // Skip numeric literals (e.g. 42, 3.14, 2.0f, 1e-3f, 0xFF)
    if (std::isdigit(static_cast<unsigned char>(expr[i])) ||
        (expr[i] == '.' && i + 1 < len && std::isdigit(static_cast<unsigned char>(expr[i + 1]))))
    {
      if (expr[i] == '0' && i + 1 < len && (expr[i + 1] == 'x' || expr[i + 1] == 'X'))
      {
        i += 2;
        while (i < len && std::isxdigit(static_cast<unsigned char>(expr[i])))
          ++i;
      }
      else
      {
        while (i < len && (std::isdigit(static_cast<unsigned char>(expr[i])) || expr[i] == '.'))
          ++i;
        if (i < len && (expr[i] == 'e' || expr[i] == 'E'))
        {
          ++i;
          if (i < len && (expr[i] == '+' || expr[i] == '-'))
            ++i;
          while (i < len && std::isdigit(static_cast<unsigned char>(expr[i])))
            ++i;
        }
      }
      // Consume type suffix (f, F, l, L, u, U)
      while (i < len && (expr[i] == 'f' || expr[i] == 'F' || expr[i] == 'l' || expr[i] == 'L' || expr[i] == 'u' || expr[i] == 'U'))
        ++i;
      continue;
    }

    // Extract identifiers
    if (std::isalpha(static_cast<unsigned char>(expr[i])) || expr[i] == '_')
    {
      size_t start = i;
      while (i < len && (std::isalnum(static_cast<unsigned char>(expr[i])) || expr[i] == '_'))
        ++i;
      std::string id = expr.substr(start, i - start);
      if (builtins.find(id) == builtins.end() && seen.find(id) == seen.end())
      {
        vars.push_back(id);
        seen.insert(id);
      }
      continue;
    }

    ++i;
  }
  return vars;
}


// Replace integer-oriented math builtins with their float equivalents.
// Since all values are cast to float, we need float versions of these functions.
// Only standalone identifiers followed by '(' are replaced (not substrings).
static auto
promoteBuiltinsToFloat(const std::string & expr) -> std::string
{
  // Mappings from integer/generic builtins to float-specific equivalents
  static const std::vector<std::pair<std::string, std::string>> replacements = {
    { "abs", "fabs" },
    { "min", "fmin" },
    { "max", "fmax" },
    { "power", "pow" },
  };

  std::string result = expr;

  for (const auto & [from, to] : replacements)
  {
    size_t pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos)
    {
      const size_t end = pos + from.size();

      // Check it's a standalone identifier (not part of a longer word)
      const bool preceded_by_id = pos > 0 && (std::isalnum(static_cast<unsigned char>(result[pos - 1])) || result[pos - 1] == '_');
      const bool followed_by_id = end < result.size() && (std::isalnum(static_cast<unsigned char>(result[end])) || result[end] == '_');

      if (!preceded_by_id && !followed_by_id)
      {
        result.replace(pos, from.size(), to);
        pos += to.size();
      }
      else
      {
        pos += from.size();
      }
    }
  }

  return result;
}


auto
evaluate(const Device::Pointer &            device,
         const std::string &                expression,
         const std::vector<ParameterType> & parameters,
         const Array::Pointer &             output) -> void
{
  // Validate
  Array::check_ptr(output, "Error: 'output' is null in evaluate().");
  if (parameters.empty())
  {
    throw std::invalid_argument("Error: 'parameters' list is empty in evaluate().");
  }
  if (expression.empty())
  {
    throw std::invalid_argument("Error: 'expression' is empty in evaluate().");
  }

  // Extract variable names from expression, in order of first appearance
  auto var_names = extractVariableNames(expression);
  if (var_names.size() != parameters.size())
  {
    throw std::invalid_argument("Error: expression has " + std::to_string(var_names.size()) + " variable(s) but " +
                                std::to_string(parameters.size()) + " parameter(s) were provided.");
  }

  // Promote integer math builtins to float equivalents
  const std::string float_expression = promoteBuiltinsToFloat(expression);

  // Separate arrays and scalars, preserving names
  struct ArrayParam
  {
    std::string    name;
    Array::Pointer arr;
  };
  struct ScalarParam
  {
    std::string name;
    float       val;
  };

  std::vector<ArrayParam>  arrays;
  std::vector<ScalarParam> scalars;

  for (size_t idx = 0; idx < parameters.size(); ++idx)
  {
    const auto & p = parameters[idx];
    const auto & name = var_names[idx];

    if (const auto * arr = std::get_if<Array::Pointer>(&p))
    {
      Array::check_ptr(*arr, ("Error: array '" + name + "' is null in evaluate().").c_str());
      arrays.push_back({ name, *arr });
    }
    else if (const auto * f = std::get_if<float>(&p))
    {
      scalars.push_back({ name, *f });
    }
    else if (const auto * i = std::get_if<int>(&p))
    {
      scalars.push_back({ name, static_cast<float>(*i) });
    }
    else if (const auto * u = std::get_if<unsigned int>(&p))
    {
      scalars.push_back({ name, static_cast<float>(*u) });
    }
    else if (const auto * s = std::get_if<size_t>(&p))
    {
      scalars.push_back({ name, static_cast<float>(*s) });
    }
    else
    {
      throw std::runtime_error("Error: unsupported parameter type at position " + std::to_string(idx) + ".");
    }
  }

  if (arrays.empty())
  {
    throw std::invalid_argument("Error: at least one Array parameter is required in evaluate().");
  }

  std::vector<Array::Pointer> array_inputs;
  array_inputs.reserve(arrays.size());
  for (const auto & array_param : arrays)
  {
    array_inputs.push_back(array_param.arr);
  }

  const auto broadcast_shape = tier0::infer_broadcast_shape(array_inputs);
  if (output->width() != broadcast_shape[0] || output->height() != broadcast_shape[1] || output->depth() != broadcast_shape[2])
  {
    throw std::invalid_argument("Error: output shape (" + std::to_string(output->width()) + "," + std::to_string(output->height()) + "," +
                                std::to_string(output->depth()) + ") does not match broadcasted shape (" +
                                std::to_string(broadcast_shape[0]) + "," + std::to_string(broadcast_shape[1]) + "," +
                                std::to_string(broadcast_shape[2]) + ") in evaluate().");
  }

  // --- Build ParameterList matching kernel signature order ---
  // Signature: (IMAGE_a_TYPE, IMAGE_b_TYPE, ..., IMAGE_dst_TYPE, float s1, float s2, ...)
  // IMAGE_xxx_TYPE expands to just a pointer, so ParameterList order = arrays, dst, scalars
  ParameterList param_list;

  for (const auto & a : arrays)
  {
    param_list.emplace_back("arr_" + a.name, a.arr);
  }
  param_list.emplace_back("dst", output);
  for (const auto & s : scalars)
  {
    param_list.emplace_back(s.name, s.val);
  }

  // --- Generate macro-compatible kernel source ---
  std::ostringstream ks;
  ks << "__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;\n\n";
  ks << "__kernel void evaluate_kernel(\n";

  for (const auto & a : arrays)
  {
    ks << "    IMAGE_arr_" << a.name << "_TYPE arr_" << a.name << ",\n";
  }
  ks << "    IMAGE_dst_TYPE dst";
  for (const auto & s : scalars)
  {
    ks << ",\n    const float " << s.name;
  }
  ks << "\n) {\n";

  ks << "    const int id0 = get_global_id(0);\n";
  ks << "    const int id1 = get_global_id(1);\n";
  ks << "    const int id2 = get_global_id(2);\n\n";

  for (const auto & a : arrays)
  {
    ks << "    const float " << a.name << " = (float)READ_IMAGE(arr_" << a.name << ", sampler, POS_arr_" << a.name
       << "_INSTANCE(id0,id1,id2,0)).x;\n";
  }
  ks << "\n";
  ks << "    const float value = " << float_expression << ";\n";
  ks << "    WRITE_IMAGE(dst, POS_dst_INSTANCE(id0,id1,id2,0), CONVERT_dst_PIXEL_TYPE(value));\n";
  ks << "}\n";

  // --- Delegate to execute() ---
  const KernelInfo kernel_info = { "evaluate_kernel", ks.str() };
  const RangeArray global_range = { output->width(), output->height(), output->depth() };

  execute(device, kernel_info, param_list, global_range);
}
} // namespace cle
