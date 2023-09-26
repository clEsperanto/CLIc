#include "execution.hpp"
#include "backend.hpp"

#include <fstream>
#include <regex>
#include <string_view>
#include <vector>

#include <chrono>
namespace cle
{


auto
replace_string(std::string & code, const std::vector<std::pair<std::string, std::string>> & replacements) -> void
{
  for (const auto & replacement : replacements)
  {
    std::string::size_type pos = 0;
    while ((pos = code.find(replacement.first, pos)) != std::string::npos)
    {
      code.replace(pos, replacement.first.length(), replacement.second);
      pos += replacement.first.length();
    }
  }
}

auto
translateOpenclToCuda(std::string & code) -> void
{
  replace_string(code,
                 { { "get_global_id(2)", "blockDim.z * blockIdx.z + threadIdx.z" },
                   { "get_global_id(1)", "blockDim.y * blockIdx.y + threadIdx.y" },
                   { "get_global_id(0)", "blockDim.x * blockIdx.x + threadIdx.x" },
                   { "#pragma", "// #pragma" },
                   { "inline", "__device__ inline" },
                   { "__kernel void", "extern \"C\" __global__ void" },
                   { "__constant sampler_t", "__device__ int" },
                   { "float4", "make_float4" },
                   { "float2", "make_float2" },
                   { "int4", "make_int4" },
                   { "int2", "make_int2" } });
}

auto
cudaDefines(const ParameterList & parameter_list, const ConstantList & constant_list) -> std::string
{
  std::ostringstream defines;
  if (!constant_list.empty())
  {
    for (const auto & [key, value] : constant_list)
    {
      defines << "#define " << key << " " << value << "\n";
    }
    defines << "\n";
  }
  std::string size_params = "";
  for (const auto & param : parameter_list)
  {
    if (std::holds_alternative<const float>(param.second) || std::holds_alternative<const int>(param.second))
    {
      continue;
    }
    const auto & arr = std::get<Array::Pointer>(param.second);

    // Function to format and append the define string
    auto appendToString = [](std::ostringstream & os,
                             const std::string &  paramFirst,
                             const std::string &  posType,
                             const std::string &  pos) {
      if (posType == "int")
      {
        os << "\n#define POS_" << paramFirst << "_INSTANCE(pos0,pos1,pos2,pos3) " << pos;
      }
      else
      {
        os << "\n#define POS_" << paramFirst << "_INSTANCE(pos0,pos1,pos2,pos3) make_" << posType << "" << pos;
      }
    };
    static constexpr std::array<const char *, 3> ndimMap = { "1", "2", "3" };
    static constexpr std::array<const char *, 3> posTypeMap = { "int", "int2", "int4" };
    static constexpr std::array<const char *, 3> posMap = { "(pos0)", "(pos0, pos1)", "(pos0, pos1, pos2, 0)" };

    int         dim = arr->dim();
    std::string ndim = ndimMap[dim - 1];
    std::string pos_type = posTypeMap[dim - 1];
    std::string pos = posMap[dim - 1];
    if (pos_type == "int")
    {
      defines << "\n#define POS_" << param.first << "_INSTANCE(pos0,pos1,pos2,pos3) " << pos;
    }
    else
    {
      defines << "\n#define POS_" << param.first << "_INSTANCE(pos0,pos1,pos2,pos3) make_" << pos_type << "" << pos;
    }
    defines << "\n";
    defines << "\n#define CONVERT_" << param.first << "_PIXEL_TYPE clij_convert_" << arr->dtype() << "_sat";
    defines << "\n#define IMAGE_" << param.first << "_PIXEL_TYPE " << arr->dtype() << "";
    defines << "\n#define POS_" << param.first << "_TYPE " << pos_type;
    defines << "\n";
    defines << "\n";
    defines << "\n#define IMAGE_SIZE_" << param.first << "_WIDTH " << std::to_string(arr->width());
    defines << "\n#define IMAGE_SIZE_" << param.first << "_HEIGHT " << std::to_string(arr->height());
    defines << "\n#define IMAGE_SIZE_" << param.first << "_DEPTH " << std::to_string(arr->depth());
    defines << "\n";
    defines << "\n";
    defines << "\n#define IMAGE_" << param.first << "_TYPE " << size_params << "" << arr->dtype() << "*";
    defines << "\n#define READ_" << param.first << "_IMAGE(a,b,c) read_buffer" << ndim << "d" << arr->shortType()
            << "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
    defines << "\n#define WRITE_" << param.first << "_IMAGE(a,b,c) write_buffer" << ndim << "d" << arr->shortType()
            << "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
    defines << "\n";
    size_params = "";
  }
  defines << "\n";
  return defines.str();
}

auto
oclDefines(const ParameterList & parameter_list, const ConstantList & constant_list) -> std::string
{
  std::ostringstream defines;
  if (!constant_list.empty())
  {
    for (const auto & [key, value] : constant_list)
    {
      defines << "#define " << key << " " << value << "\n";
    }
    defines << "\n";
  }
  defines << "\n#define GET_IMAGE_WIDTH(image_key) IMAGE_SIZE_ ## image_key ## _WIDTH";
  defines << "\n#define GET_IMAGE_HEIGHT(image_key) IMAGE_SIZE_ ## image_key ## _HEIGHT";
  defines << "\n#define GET_IMAGE_DEPTH(image_key) IMAGE_SIZE_ ## image_key ## _DEPTH";
  defines << "\n";
  for (const auto & param : parameter_list)
  {
    if (std::holds_alternative<const float>(param.second) || std::holds_alternative<const int>(param.second))
    {
      continue;
    }
    const auto & arr = std::get<Array::Pointer>(param.second);

    static constexpr std::array<const char *, 3> ndimMap = { "1", "2", "3" };
    static constexpr std::array<const char *, 3> posTypeMap = { "int", "int2", "int4" };
    static constexpr std::array<const char *, 3> posMap = { "(pos0)", "(pos0, pos1)", "(pos0, pos1, pos2, 0)" };

    int         dim = arr->dim();
    std::string ndim = ndimMap[dim - 1];
    std::string pos_type = posTypeMap[dim - 1];
    std::string pos = posMap[dim - 1];

    defines << "\n";
    defines << "\n#define CONVERT_" << param.first << "_PIXEL_TYPE clij_convert_" << arr->dtype() << "_sat";
    defines << "\n#define IMAGE_" << param.first << "_PIXEL_TYPE " << arr->dtype() << "";
    defines << "\n#define POS_" << param.first << "_TYPE " << pos_type;
    defines << "\n#define POS_" << param.first << "_INSTANCE(pos0,pos1,pos2,pos3) (" << pos_type << ")" << pos;
    defines << "\n";
    if (arr->mtype() == mType::BUFFER)
    {
      defines << "\n#define IMAGE_" << param.first << "_TYPE __global " << arr->dtype() << "*";
      defines << "\n#define READ_" << param.first << "_IMAGE(a,b,c) read_buffer" << ndim << "d" << arr->shortType()
              << "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
      defines << "\n#define WRITE_" << param.first << "_IMAGE(a,b,c) write_buffer" << ndim << "d" << arr->shortType()
              << "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
    }
    else
    {
      std::string img_type_name;
      if (param.first.find("dst") != std::string::npos || param.first.find("destination") != std::string::npos ||
          param.first.find("output") != std::string::npos)
      {
        img_type_name = "__write_only image" + ndim + "d_t";
      }
      else
      {
        img_type_name = "__read_only image" + ndim + "d_t";
      }
      std::string prefix;
      switch (arr->shortType().front())
      {
        case 'u':
          prefix = "ui";
          break;
        case 'f':
          prefix = "f";
          break;
        default:
          prefix = "i";
          break;
      }
      defines << "\n#define IMAGE_" << param.first << "_TYPE " << img_type_name;
      defines << "\n#define READ_" << param.first << "_IMAGE(a,b,c) read_image" << prefix << "(a,b,c)";
      defines << "\n#define WRITE_" << param.first << "_IMAGE(a,b,c) write_image" << prefix << "(a,b,c)";
    }
    defines << "\n";
    defines << "\n#define IMAGE_SIZE_" << param.first << "_WIDTH " << std::to_string(arr->width());
    defines << "\n#define IMAGE_SIZE_" << param.first << "_HEIGHT " << std::to_string(arr->height());
    defines << "\n#define IMAGE_SIZE_" << param.first << "_DEPTH " << std::to_string(arr->depth());
    defines << "\n";
  }
  defines << "\n";
  return defines.str();
}

auto
execute(const Device::Pointer & device,
        const KernelInfo &      kernel_func,
        const ParameterList &   parameters,
        const RangeArray &      global_range,
        const ConstantList &    constants) -> void
{
  // prepare kernel source for compilation and execution
  auto        kernel_source = kernel_func.second;
  const auto  kernel_name = kernel_func.first;
  const auto  kernel_preamble = cle::BackendManager::getInstance().getBackend().getPreamble();
  std::string defines;
  switch (device->getType())
  {
    case Device::Type::CUDA:
      defines = cle::cudaDefines(parameters, constants);
      cle::translateOpenclToCuda(kernel_source);
      break;
    case Device::Type::OPENCL:
      defines = cle::oclDefines(parameters, constants);
      break;
  }
  std::string program_source;
  program_source.reserve(kernel_preamble.size() + defines.size() + kernel_source.size());
  program_source += defines;
  program_source += kernel_preamble;
  program_source += kernel_source;

  // prepare parameters to be passed to the backend
  std::vector<void *> args_ptr;
  std::vector<size_t> args_size;
  args_ptr.reserve(parameters.size());
  args_size.reserve(parameters.size());
  for (const auto & param : parameters)
  {
    if (std::holds_alternative<Array::Pointer>(param.second))
    {
      const auto & arr = std::get<Array::Pointer>(param.second);
      switch (device->getType())
      {
        case Device::Type::CUDA:
          args_ptr.push_back(arr->get());
          break;
        case Device::Type::OPENCL:
          args_ptr.push_back(*arr->get());
          args_size.push_back(sizeof(cl_mem));
          break;
      }
    }
    else if (std::holds_alternative<const float>(param.second))
    {
      const auto & f = std::get<const float>(param.second);
      args_ptr.push_back(const_cast<float *>(&f));
      args_size.push_back(sizeof(float));
    }
    else if (std::holds_alternative<const int>(param.second))
    {
      const auto & i = std::get<const int>(param.second);
      args_ptr.push_back(const_cast<int *>(&i));
      args_size.push_back(sizeof(int));
    }
  }

  // execute kernel in backend
  try
  {
    cle::BackendManager::getInstance().getBackend().executeKernel(
      device, program_source, kernel_name, global_range, args_ptr, args_size);
  }
  catch (const std::exception & e)
  {
    throw std::runtime_error("Error: Failed to execute the kernel. \n\t > " + std::string(e.what()));
  }
}

auto
native_execute(const Device::Pointer & device,
               const KernelInfo &      kernel_func,
               const ParameterList &   parameters,
               const RangeArray &      global_range,
               const RangeArray &      local_range) -> void
{
  // TODO @StRigaud: Implement native execution for OpenCL and CUDA
  // allows execution of pure CUDA or OpenCL code without CLIJ syntax
  throw std::runtime_error("Error: Native execution is not implemented yet.");
}

} // namespace cle
