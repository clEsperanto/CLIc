#include "execution.hpp"
#include "backend.hpp"
#include "clic.hpp"

namespace cle
{

// Function for translating OpenCL code to CUDA code
// @StRigaud TODO: function is not exhaustive and needs to be improved to support more features
static auto
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
  // special case: 'make_' need to followed by ');' replacement, e.g. (int2){1,2}; -> make_int2(1,2);
  const std::vector<std::pair<std::string, std::string>> replacements = {
    { "(int2){", "make_int2(" },
    { "(int4){", "make_int4(" },
    { "(float4){", "make_float4(" },
    { "(float2){", "make_float2(" },
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


// Function creating common defines for constants
static auto
commonDefines(const ConstantList & constant_list) -> std::string
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
  return defines.str();
}


// Function creating buffer specific defines
static auto
bufferDefines(std::ostringstream & defines,
              const std::string &  key,
              const std::string &  ndim,
              const std::string &  dtype,
              const std::string &  stype,
              const std::string &  ocl) -> void
{
  defines << "\n#define IMAGE_" << key << "_TYPE " << ocl << dtype << "*";
  defines << "\n#define READ_" << key << "_IMAGE(a,b,c) read_buffer" << ndim << "d" << stype
          << "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
  defines << "\n#define WRITE_" << key << "_IMAGE(a,b,c) write_buffer" << ndim << "d" << stype
          << "(GET_IMAGE_WIDTH(a),GET_IMAGE_HEIGHT(a),GET_IMAGE_DEPTH(a),a,b,c)";
}


// Function creating image specific defines (OpenCL only for now)
static auto
imageDefines(std::ostringstream & defines,
             const std::string &  key,
             const std::string &  ndim,
             const std::string &  stype,
             const std::string &  access_type) -> void
{
  char        front_char = stype.front();
  std::string prefix = (front_char == 'u') ? "ui" : (front_char == 'f') ? "f" : "i";
  std::string img_type_name = access_type + " image" + ndim + "d_t";
  defines << "\n#define IMAGE_" << key << "_TYPE " << img_type_name;
  defines << "\n#define READ_" << key << "_IMAGE(a,b,c) read_image" << prefix << "(a,b,c)";
  defines << "\n#define WRITE_" << key << "_IMAGE(a,b,c) write_image" << prefix << "(a,b,c)";
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
  std::ostringstream                           defines;
  static constexpr std::array<const char *, 3> ndimMap = { "1", "2", "3" };
  static constexpr std::array<const char *, 3> posTypeMap = { "int", "int2", "int4" };
  static constexpr std::array<const char *, 3> posMap = { "(pos0)", "(pos0, pos1)", "(pos0, pos1, pos2, 0)" };

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
    auto              dim = shape_to_dimension(arr->width(), arr->height(), arr->depth());
    const size_t      dimIndex = dim - 1;
    const std::string ndim = ndimMap[dimIndex];
    const std::string pos_type = posTypeMap[dimIndex];
    const std::string pos = posMap[dimIndex];
    defines << "\n#define CONVERT_" << key << "_PIXEL_TYPE clij_convert_" << arr->dtype() << "_sat";
    defines << "\n#define IMAGE_" << key << "_PIXEL_TYPE " << arr->dtype();
    defines << "\n#define POS_" << key << "_TYPE " << pos_type;
    const std::string prefix =
      (device == Device::Type::OPENCL || pos_type == "int") ? "(" + pos_type + ")" : "make_" + pos_type;
    defines << "\n#define POS_" << param.first << "_INSTANCE(pos0,pos1,pos2,pos3) " << prefix << pos;
    defines << "\n";

    // manage array type (buffer or image), and read/write macros
    if (arr->mtype() == mType::BUFFER || device == Device::Type::CUDA)
    {
      std::string ocl_keyword = (device == Device::Type::OPENCL) ? "__global " : "";
      bufferDefines(defines, key, ndim, toString(arr->dtype()), arr->shortType(), ocl_keyword);
    }
    else
    {
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
      imageDefines(defines, key, ndim, arr->shortType(), access_type);
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
generateDefines(const ParameterList & parameter_list, const ConstantList & constant_list, const Device::Type & device)
  -> std::string
{
  std::ostringstream defines;
  defines << commonDefines(constant_list);
  defines << arrayDefines(parameter_list, device);
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
  auto kernel_source = kernel_func.second;
  auto kernel_name = kernel_func.first;
  auto kernel_preamble = cle::BackendManager::getInstance().getBackend().getPreamble();
  auto defines = generateDefines(parameters, constants, device->getType());
  if (device->getType() == Device::Type::CUDA)
  {
    cle::translateOpenclToCuda(kernel_source);
  }
  platform_options(device, &kernel_preamble);
  const std::string program_source = defines + kernel_preamble + kernel_source;

  // prepare parameters to be passed to the backend
  std::vector<void *> args_ptr;
  std::vector<size_t> args_size;
  args_ptr.reserve(parameters.size());
  args_size.reserve(parameters.size());
  for (const auto & param : parameters)
  {
    if (const auto & arr = std::get_if<Array::Pointer>(&param.second))
    {
      args_ptr.push_back(device->getType() == Device::Type::CUDA ? (*arr)->get() : *(*arr)->get());
      args_size.push_back(GPU_MEM_PTR_SIZE);
    }
    else if (const auto & f = std::get_if<float>(&param.second))
    {
      args_ptr.push_back(const_cast<float *>(f));
      args_size.push_back(sizeof(float));
    }
    else if (const auto & i = std::get_if<int>(&param.second))
    {
      args_ptr.push_back(const_cast<int *>(i));
      args_size.push_back(sizeof(int));
    }
    else
    {
      throw std::runtime_error("Error: Invalid parameter type provided.");
    }
  }

  // execute kernel
  cle::BackendManager::getInstance().getBackend().executeKernel(
    device, program_source, kernel_name, global_range, args_ptr, args_size);
}


auto
execute_separable(const Device::Pointer &      device,
                  const KernelInfo &           kernel,
                  const Array::Pointer &       src,
                  const Array::Pointer &       dst,
                  const std::array<float, 3> & sigma,
                  const std::array<int, 3> &   radius) -> void
{
  Array::check_ptr(src, "Error: 'src' is null. Please ensure the provided parameters before calling this function.");
  Array::check_ptr(dst, "Error: 'dst' is null. Please ensure the provided parameters before calling this function.");

  const RangeArray global_range = { dst->width(), dst->height(), dst->depth() };

  auto tmp1 = Array::create(dst);
  auto tmp2 = Array::create(dst);

  if (dst->width() > 1 && sigma[0] > 0)
  {
    const ParameterList parameters = {
      { "src", src }, { "dst", tmp1 }, { "dim", 0 }, { "N", radius[0] }, { "s", sigma[0] }
    };
    execute(device, kernel, parameters, global_range);
  }
  else
  {
    src->copy(tmp1);
  }
  if (dst->height() > 1 && sigma[1] > 0)
  {
    const ParameterList parameters = {
      { "src", tmp1 }, { "dst", tmp2 }, { "dim", 1 }, { "N", radius[1] }, { "s", sigma[1] }
    };
    execute(device, kernel, parameters, global_range);
  }
  else
  {
    tmp1->copy(tmp2);
  }
  if (dst->depth() > 1 && sigma[2] > 0)
  {
    const ParameterList parameters = {
      { "src", tmp2 }, { "dst", dst }, { "dim", 2 }, { "N", radius[2] }, { "s", sigma[2] }
    };
    execute(device, kernel, parameters, global_range);
  }
  else
  {
    tmp2->copy(dst);
  }
}


auto
native_execute(const Device::Pointer & device,
               const KernelInfo &      kernel_func,
               const ParameterList &   parameters,
               const RangeArray &      global_range,
               const RangeArray &      local_range) -> void
{
  // this is very similar to the execute function. However, this function is used for native kernels.
  // No preamble or code conversion is done here. The kernel source is expected to be in the correct format from the
  // user. We still however need to rely on the Array class to handle arrays (float * etc.). For now, we only support 1D
  // arrays and int and float parameters.

  // get raw kernel source and name from user
  auto kernel_source = kernel_func.second;
  auto kernel_name = kernel_func.first;

  // prepare parameters to be passed to the backend (CUDA or OpenCL)
  std::vector<void *> args_ptr;
  std::vector<size_t> args_size;
  args_ptr.reserve(parameters.size());
  args_size.reserve(parameters.size());
  for (const auto & param : parameters)
  {
    if (const auto & arr = std::get_if<Array::Pointer>(&param.second))
    {
      args_ptr.push_back(device->getType() == Device::Type::CUDA ? (*arr)->get() : *(*arr)->get());
      args_size.push_back(GPU_MEM_PTR_SIZE);
    }
    else if (const auto & f = std::get_if<float>(&param.second))
    {
      args_ptr.push_back(const_cast<float *>(f));
      args_size.push_back(sizeof(float));
    }
    else if (const auto & i = std::get_if<int>(&param.second))
    {
      args_ptr.push_back(const_cast<int *>(i));
      args_size.push_back(sizeof(int));
    }
    else
    {
      throw std::runtime_error("Error: Invalid parameter type provided.");
    }
  }

  // execute kernel
  cle::BackendManager::getInstance().getBackend().executeKernel(
    device, kernel_source, kernel_name, global_range, args_ptr, args_size);
}

} // namespace cle
