#include "execution.hpp"
#include "backend.hpp"
#include "clic.hpp"
#include <cctype>
#include <cstring>
#include <set>

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


// Function creating common defines for constants
static auto
commonDefines(const ConstantList & constant_list) -> std::string
{
  std::ostringstream defines;
  for (const auto & item : constant_list)
  {
    std::visit([&](const auto & v) { defines << "#define " << item.first << " " << v << "\n"; }, item.second);
  }
  defines << "\n";
  defines << "\n#define GET_IMAGE_WIDTH(image_key) IMAGE_SIZE_ ## image_key ## _WIDTH";
  defines << "\n#define GET_IMAGE_HEIGHT(image_key) IMAGE_SIZE_ ## image_key ## _HEIGHT";
  defines << "\n#define GET_IMAGE_DEPTH(image_key) IMAGE_SIZE_ ## image_key ## _DEPTH";
  defines << "\n";
  return defines.str();
}


// Function creating buffer specific defines
static auto
bufferDefines(std::ostringstream &   defines,
              const std::string &    key,
              const Array::Pointer & arr,
              const size_t &         dim,
              const Device::Type &   device) -> void
{
  static constexpr std::array<const char *, 3> ndimMap = { "1", "2", "3" };
  static constexpr std::array<const char *, 3> posTypeMap = { "int", "int2", "int4" };
  static constexpr std::array<const char *, 3> posMap = { "(pos0)", "(pos0, pos1)", "(pos0, pos1, pos2, 0)" };

  const size_t        dimIndex = dim - 1;
  const std::string   ndim = ndimMap[dimIndex];
  const std::string   pos_type = posTypeMap[dimIndex];
  const std::string   pos = posMap[dimIndex];
  const std::string   stype = toShortString(arr->dtype());
  const std::string & dtype = toString(arr->dtype());

  std::string access_type = (device == Device::Type::OPENCL) ? "__global " : "";

  defines << "\n#define CONVERT_" << key << "_PIXEL_TYPE clij_convert_" << arr->dtype() << "_sat";
  defines << "\n#define IMAGE_" << key << "_PIXEL_TYPE " << arr->dtype();
  defines << "\n#define POS_" << key << "_TYPE " << pos_type;
  const std::string prefix = (device == Device::Type::OPENCL || pos_type == "int") ? "(" + pos_type + ")" : "make_" + pos_type;
  defines << "\n#define POS_" << key << "_INSTANCE(pos0,pos1,pos2,pos3) " << prefix << pos;
  defines << "\n";

  defines << "\n#define IMAGE_" << key << "_TYPE " << access_type << dtype << "*";
  defines << "\n#define READ_" << key << "_IMAGE(a,b,c) read_buffer" << ndim << "d" << stype
          << "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
  defines << "\n#define WRITE_" << key << "_IMAGE(a,b,c) write_buffer" << ndim << "d" << stype
          << "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
}


// Function creating image specific defines (OpenCL only for now)
static auto
imageDefines(std::ostringstream &   defines,
             const std::string &    key,
             const Array::Pointer & arr,
             const size_t &         dim,
             const Device::Type &   device) -> void
{
  static constexpr std::array<const char *, 3> ndimMap = { "1", "2", "3" };
  static constexpr std::array<const char *, 3> posIntTypeMap = { "int", "int2", "int4" };
  static constexpr std::array<const char *, 3> posFloatTypeMap = { "float", "float2", "float4" };
  static constexpr std::array<const char *, 3> posMap = { "(pos0)", "(pos0, pos1)", "(pos0, pos1, pos2, 0)" };

  const size_t      dimIndex = dim - 1;
  const std::string ndim = ndimMap[dimIndex];
  std::string       pos_type;
  const std::string pos = posMap[dimIndex];
  const std::string stype = toShortString(arr->dtype());

  std::string access_type;
  if (key.find("dst") != std::string::npos || key.find("destination") != std::string::npos || key.find("output") != std::string::npos)
  {
    access_type = "__write_only";
    pos_type = posIntTypeMap[dimIndex];
  }
  else
  {
    access_type = "__read_only";
    pos_type = posFloatTypeMap[dimIndex];
  }

  defines << "\n#define CONVERT_" << key << "_PIXEL_TYPE clij_convert_" << arr->dtype() << "_sat";
  defines << "\n#define IMAGE_" << key << "_PIXEL_TYPE " << arr->dtype();
  defines << "\n#define POS_" << key << "_TYPE " << pos_type;
  const std::string prefix1 = (device == Device::Type::OPENCL || pos_type == "int") ? "(" + pos_type + ")" : "make_" + pos_type;
  defines << "\n#define POS_" << key << "_INSTANCE(pos0,pos1,pos2,pos3) " << prefix1 << pos;
  defines << "\n";

  char        front_char = stype.front();
  std::string prefix2 = (front_char == 'u') ? "ui" : (front_char == 'f') ? "f" : "i";
  std::string img_type_name = access_type + " image" + ndim + "d_t";
  defines << "\n#define IMAGE_" << key << "_TYPE " << img_type_name;
  defines << "\n#define READ_" << key << "_IMAGE(a,b,c) read_image" << prefix2 << "(a,b,c)";
  defines << "\n#define WRITE_" << key << "_IMAGE(a,b,c) write_image" << prefix2 << "(a,b,c)";
}

static auto
platform_options(const Device::Pointer & device, std::string * source) -> void
{
  auto platform = device->getPlatform();
  if (platform.find("AMD") != std::string::npos)
  {
    // append at the begining of the source code the following string "#pragma OPENCL EXTENSION cl_amd_printf : enable"
    *source = "#pragma OPENCL EXTENSION cl_amd_printf : enable\n" + *source;
  }
}

// Function for creating defines for each array parameters
static auto
arrayDefines(const ParameterList & parameter_list, const Device::Type & device) -> std::string
{
  std::ostringstream defines;
  // loop over all parameters, skip if parameter is not an array
  for (const auto & param : parameter_list)
  {
    if (std::holds_alternative<float>(param.second) || std::holds_alternative<int>(param.second))
    {
      continue;
    }
    const auto & arr = std::get<Array::Pointer>(param.second);
    const auto & key = param.first;

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
    defines << "\n";
    defines << "\n#define IMAGE_SIZE_" << key << "_WIDTH " << std::to_string(arr->width());
    defines << "\n#define IMAGE_SIZE_" << key << "_HEIGHT " << std::to_string(arr->height());
    defines << "\n#define IMAGE_SIZE_" << key << "_DEPTH " << std::to_string(arr->depth());
    defines << "\n\n";
  }
  return defines.str();
}


// Top function for creating defines at runtime
auto
generateDefines(const ParameterList & parameter_list, const ConstantList & constant_list, const Device::Type & device) -> std::string
{
  std::ostringstream defines;
  defines << commonDefines(constant_list);
  defines << arrayDefines(parameter_list, device);
  return defines.str();
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
      args_ptr.push_back(std::static_pointer_cast<void>(std::make_shared<size_t>(*s)));
      args_size.push_back(sizeof(size_t));
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
  auto kernel_source = kernel_func.second;
  auto kernel_name = kernel_func.first;
  auto kernel_preamble = cle::BackendManager::getInstance().getBackend().getPreamble();
  auto defines = generateDefines(parameters, constants, device->getType());
  platform_options(device, &kernel_preamble);

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

  std::vector<std::shared_ptr<void>> args_ptr;
  std::vector<size_t>                args_size;
  marshalParameters(parameters, args_ptr, args_size);

  for (const auto & dtype : used_dtypes)
  {
    defines += "\n" + dtype_defines.at(dtype);
  }
  for (const auto & dim : used_dimensions)
  {
    defines += "\n" + dimension_defines.at(dim);
  }
  defines += "\n\n";

  const std::string program_source = defines + kernel_preamble + kernel_source;
  cle::BackendManager::getInstance().getBackend().executeKernel(
    device, program_source, kernel_name, global_range, local_range, args_ptr, args_size);
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

  auto tmp1 = Array::create(dst);
  auto tmp2 = Array::create(dst);

  auto execute_if_needed = [&](int dim, int idx, auto & input, auto & output) {
    if (dim > 1 && sigma[idx] > 0)
    {
      const ParameterList parameters = { { "src", input },     { "dst", output },   { "dim", idx },
                                         { "N", radius[idx] }, { "s", sigma[idx] }, { "order", orders[idx] } };
      execute(device, kernel, parameters, global_range);
    }
    else
    {
      input->copyTo(output);
    }
  };

  execute_if_needed(dst->width(), 0, src, tmp1);
  execute_if_needed(dst->height(), 1, tmp1, tmp2);
  execute_if_needed(dst->depth(), 2, tmp2, dst);
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
      if ((*arr)->size() != output->size())
      {
        throw std::invalid_argument("Error: array '" + name + "' size (" + std::to_string((*arr)->size()) +
                                    ") does not match output size (" + std::to_string(output->size()) + ").");
      }
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

  // --- Build ParameterList matching kernel signature order ---
  // Signature: (IMAGE_a_TYPE, IMAGE_b_TYPE, ..., IMAGE_dst_TYPE, float s1, float s2, ...)
  // IMAGE_xxx_TYPE expands to just a pointer, so ParameterList order = arrays, dst, scalars
  ParameterList param_list;

  for (const auto & a : arrays)
  {
    param_list.push_back({ "arr_" + a.name, a.arr });
  }
  param_list.push_back({ "dst", output });
  for (const auto & s : scalars)
  {
    param_list.push_back({ s.name, s.val });
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

  ks << "    const int x = get_global_id(0);\n";
  ks << "    const int y = get_global_id(1);\n";
  ks << "    const int z = get_global_id(2);\n\n";

  for (const auto & a : arrays)
  {
    ks << "    const float " << a.name << " = (float)READ_IMAGE(arr_" << a.name << ", sampler, POS_arr_" << a.name
       << "_INSTANCE(x,y,z,0)).x;\n";
  }
  ks << "\n";
  ks << "    const float value = " << float_expression << ";\n";
  ks << "    WRITE_IMAGE(dst, POS_dst_INSTANCE(x,y,z,0), CONVERT_dst_PIXEL_TYPE(value));\n";
  ks << "}\n";

  // --- Delegate to execute() ---
  const KernelInfo kernel_info = { "evaluate_kernel", ks.str() };
  const RangeArray global_range = { output->width(), output->height(), output->depth() };

  execute(device, kernel_info, param_list, global_range);
}
} // namespace cle
