#include "execution.hpp"
#include "backend.hpp"

#include <fstream>
#include <regex>
#include <string_view>
#include <vector>

namespace cle
{

auto
translateOpenclToCuda(std::string & code) -> void
{
  // nested lambda function to find and replace all occurrences of a string
  auto findAndReplace = [](std::string & str, const std::string & to_find, const std::string & to_replace) -> void {
    size_t pos = 0;
    while ((pos = str.find(to_find, pos)) != std::string::npos)
    {
      str.replace(pos, to_find.length(), to_replace);
      pos += to_replace.length();

      if (to_find.find("int") != std::string::npos || to_find.find("float") != std::string::npos)
      {
        size_t pos2 = str.find("};", pos);
        str.replace(pos2, 2, ");");
      }
    }
  };

  // list of replacements to be performed (not exhaustive)
  const std::vector<std::pair<std::string, std::string>> replacements = {
    { "(int2){", "make_int2(" },     // special case - need to followed by ');' replacement
    { "(int4){", "make_int4(" },     // special case - need to followed by ');' replacement
    { "(float4){", "make_float4(" }, // special case - need to followed by ');' replacement
    { "(float2){", "make_float2(" }, // special case - need to followed by ');' replacement
    { "__constant sampler_t", "__device__ int" },
    { "inline", "__device__ inline" },
    { "#pragma", "// #pragma" },
    { "__kernel void", "extern \"C\" __global__ void" },
    { "get_global_id(0)", "blockDim.x * blockIdx.x + threadIdx.x" },
    { "get_global_id(1)", "blockDim.y * blockIdx.y + threadIdx.y" },
    { "get_global_id(2)", "blockDim.z * blockIdx.z + threadIdx.z" }
  };

  // perform replacements
  for (const auto & [to_replace, replace_with] : replacements)
  {
    findAndReplace(code, to_replace, replace_with);
  }
}

auto
cudaDefines(const ParameterList & parameter_list, const ConstantList & constant_list) -> std::string
{
  std::ostringstream defines;
  for (const auto & [key, value] : constant_list)
  {
    defines << "#define " << key << " " << value << "\n";
  }
  defines << "\n";
  for (const auto & param : parameter_list)
  {
    if (std::holds_alternative<const float>(param.second) || std::holds_alternative<const int>(param.second))
    {
      continue;
    }
    const auto & arr = std::get<Array::Pointer>(param.second);

    // Function to format and append the define string
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
    defines << "\n\n";
    defines << "\n#define IMAGE_SIZE_" << param.first << "_WIDTH " << std::to_string(arr->width());
    defines << "\n#define IMAGE_SIZE_" << param.first << "_HEIGHT " << std::to_string(arr->height());
    defines << "\n#define IMAGE_SIZE_" << param.first << "_DEPTH " << std::to_string(arr->depth());
    defines << "\n\n";
    defines << "\n#define IMAGE_" << param.first << "_TYPE " << arr->dtype() << "*";
    defines << "\n#define READ_" << param.first << "_IMAGE(a,b,c) read_buffer" << ndim << "d" << arr->shortType()
            << "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
    defines << "\n#define WRITE_" << param.first << "_IMAGE(a,b,c) write_buffer" << ndim << "d" << arr->shortType()
            << "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
    defines << "\n";
  }
  defines << "\n";
  return defines.str();
}

auto
oclDefines(const ParameterList & parameter_list, const ConstantList & constant_list) -> std::string
{
  std::ostringstream defines;
  for (const auto & [key, value] : constant_list)
  {
    defines << "#define " << key << " " << value << "\n";
  }
  defines << "\n";
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

    const int         dimIndex = arr->dim() - 1;
    const std::string ndim = ndimMap[dimIndex];
    const std::string pos_type = posTypeMap[dimIndex];
    const std::string pos = posMap[dimIndex];

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
      char        front_char = arr->shortType().front();
      std::string prefix = (front_char == 'u') ? "ui" : (front_char == 'f') ? "f" : "i";

      std::string access_type;
      if (param.first.find("dst") != std::string::npos || param.first.find("destination") != std::string::npos ||
          param.first.find("output") != std::string::npos)
      {
        access_type = "__write_only";
      }
      else
      {
        access_type = "__read_only";
      }
      std::string img_type_name = access_type + " image" + ndim + "d_t";

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
    case Device::Type::CUDA: {
      defines = cle::cudaDefines(parameters, constants);
      cle::translateOpenclToCuda(kernel_source);
      break;
    }
    case Device::Type::OPENCL: {
      defines = cle::oclDefines(parameters, constants);
      break;
    }
  }
  const std::string program_source = defines + kernel_preamble + kernel_source;

  // prepare parameters to be passed to the backend
  std::vector<void *> args_ptr;
  std::vector<size_t> args_size;
  args_ptr.reserve(parameters.size());
  args_size.reserve(parameters.size());

#if USE_OPENCL
  const constexpr size_t size_of_pointer = sizeof(cl_mem);
#else
  const constexpr size_t size_of_pointer = sizeof(void *);
#endif
  for (const auto & param : parameters)
  {
    if (const auto & arr = std::get_if<Array::Pointer>(&param.second))
    {
      args_ptr.push_back(device->getType() == Device::Type::CUDA ? (*arr)->get() : *(*arr)->get());
      args_size.push_back(size_of_pointer);
    }
    else if (const auto & f = std::get_if<const float>(&param.second))
    {
      args_ptr.push_back(const_cast<float *>(f));
      args_size.push_back(sizeof(float));
    }
    else if (const auto & i = std::get_if<const int>(&param.second))
    {
      args_ptr.push_back(const_cast<int *>(i));
      args_size.push_back(sizeof(int));
    }
    else
    {
      throw std::runtime_error("Error: Invalid parameter type provided.");
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
