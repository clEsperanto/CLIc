#include "execution.hpp"
#include "backend.hpp"
#include "clic.hpp"
#include <cstring>
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
  const std::vector<std::pair<std::string, std::string>> replacements = { { "(int2){", "make_int2(" },
                                                                          { "(int4){", "make_int4(" },
                                                                          { "(float4){", "make_float4(" },
                                                                          { "(float2){", "make_float2(" },
                                                                          { "__constant sampler_t", "__device__ int" },
                                                                          { "inline", "__device__ inline" },
                                                                          { "#pragma", "// #pragma" },
                                                                          { "__kernel void", "extern \"C\" __global__ void" },
                                                                          { "get_global_id(0)", "blockDim.x * blockIdx.x + threadIdx.x" },
                                                                          { "get_global_id(1)", "blockDim.y * blockIdx.y + threadIdx.y" },
                                                                          { "get_global_id(2)", "blockDim.z * blockIdx.z + threadIdx.z" } };

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


auto
execute(const Device::Pointer & device,
        const KernelInfo &      kernel_func,
        const ParameterList &   parameters,
        const RangeArray &      global_range,
        const RangeArray &      local_range,
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
  std::vector<std::shared_ptr<void>> args_ptr;
  std::vector<size_t>                args_size;

  args_ptr.reserve(parameters.size());
  args_size.reserve(parameters.size());
  for (const auto & param : parameters)
  {
    if (const auto & arr = std::get_if<Array::Pointer>(&param.second))
    {
      args_ptr.push_back(arr->get()->get_ptr());
      args_size.push_back(GPU_MEM_PTR_SIZE);
    }
    else if (const auto & f = std::get_if<float>(&param.second))
    {
      // Allocate memory for a float and copy the value
      auto float_ptr = std::make_shared<float>(*f);
      // Cast to shared_ptr<void>
      args_ptr.push_back(std::static_pointer_cast<void>(float_ptr));
      args_size.push_back(sizeof(float));
    }
    else if (const auto & i = std::get_if<int>(&param.second))
    {
      // Allocate memory for an int and copy the value
      auto int_ptr = std::make_shared<int>(*i);
      // Cast to shared_ptr<void>
      args_ptr.push_back(std::static_pointer_cast<void>(int_ptr));
      args_size.push_back(sizeof(int));
    }
    else if (const auto & i = std::get_if<unsigned int>(&param.second))
    {
      // Allocate memory for an unsigned int and copy the value
      auto uint_ptr = std::make_shared<unsigned int>(*i);
      // Cast to shared_ptr<void>
      args_ptr.push_back(std::static_pointer_cast<void>(uint_ptr));
      args_size.push_back(sizeof(unsigned int));
    }
    else if (const auto & i = std::get_if<size_t>(&param.second))
    {
      // Allocate memory for a size_t and copy the value
      auto size_ptr = std::make_shared<size_t>(*i);
      // Cast to shared_ptr<void>
      args_ptr.push_back(std::static_pointer_cast<void>(size_ptr));
      args_size.push_back(sizeof(size_t));
    }
    else
    {
      throw std::runtime_error("Error: Invalid parameter type provided.");
    }
  }

  // execute kernel
  cle::BackendManager::getInstance().getBackend().executeKernel(device, program_source, kernel_name, global_range, local_range, args_ptr, args_size);
}

size_t round_up(size_t v, size_t tile) { return ((v + tile - 1) / tile) * tile; }

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

  // Round up global size to multiple of TILE_SIZE in filtering dimension
  size_t TILE_SIZE = 1;
  size_t MAX_RADIUS = 1;
  RangeArray global_size = {dst->width(), dst->height(), dst->depth()};
  RangeArray local_size = {1, 1, 1};
  ConstantList constants = {};

  // if kernel.first contains the substring "shared", use shared memory optimization
  // throw an error if shared memory configuration fails to fallback to global memory version
  if (kernel.first.find("shared") != std::string::npos) {
    const size_t max_shared_mem = device->getLocalMemorySize();
    const size_t usable_shared_mem = (max_shared_mem * 9) / 10;  // Reserve 10% for kernel overhead
    auto max_radius = static_cast<size_t>(std::max({ radius[0], radius[1], radius[2] }) * 0.5f);

    // Lambda to calculate shared memory requirement for a given configuration
    auto calc_shared_mem = [](size_t tile_size, size_t max_radius) -> size_t {
      return (tile_size + 2 * max_radius) * sizeof(float);
    };

    const std::array<std::pair<size_t, size_t>, 7> candidates = {{
        {512, 16},   // Best for small radii
        {256, 32},   // Good for moderate radii
        {256, 64},   // Balanced approach
        {128, 64},   // Lower parallelism, moderate radii
        {128, 128},  // Lower parallelism, large radii
        {64, 128},   // Minimal parallelism, large radii
        {64, 256}    // Last resort for very large radii
      }};

    for (const auto& [tile, max_r] : candidates) {
        // Ensure MAX_RADIUS can accommodate the actual radius (rounded up to multiple of 16)
        size_t required_max_radius = std::max(max_r, round_up(max_radius, 16));
        size_t required_mem = calc_shared_mem(tile, required_max_radius);
        
        // Check if this configuration fits in available shared memory
        if (required_mem <= usable_shared_mem) {
          TILE_SIZE = tile;
          MAX_RADIUS = required_max_radius;
          break;
        }
      }

    constants = { { "TILE_SIZE", TILE_SIZE }, { "MAX_RADIUS", MAX_RADIUS } };

    if (TILE_SIZE == 1 || MAX_RADIUS == 1) {
      throw std::runtime_error("Error: Unable to configure shared memory parameters for separable filter. Falling back to global memory version.");
    }
    
  }


  
  auto tmp1 = Array::create(dst);
  auto tmp2 = Array::create(dst);

  auto execute_if_needed = [&](int dim, int idx, auto & input, auto & output) {
    if (dim > 1 && sigma[idx] > 0)
    {
      if (idx == 0) {
          // Filter along X
          global_size = {round_up(dst->width(), TILE_SIZE),dst->height(), dst->depth()};
          local_size = {TILE_SIZE, 1, 1};
      } else if (idx == 1) {
          // Filter along Y
          global_size = {dst->width(), round_up(dst->height(), TILE_SIZE), dst->depth()};
          local_size = {1, TILE_SIZE, 1};
      } else {
          // Filter along Z
          global_size = {dst->width(), dst->height(), round_up(dst->depth(), TILE_SIZE)};
          local_size = {1, 1, TILE_SIZE};
      }

      const ParameterList parameters = { { "src", input },     { "dst", output },   { "dim", idx },
                                         { "N", radius[idx] }, { "s", sigma[idx] }, { "order", orders[idx] } };
      execute(device, kernel, parameters, global_size, local_size, constants);
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
  const RangeArray local_range = { 16, 16, 1 };

  auto tmp1 = Array::create(dst);
  auto tmp2 = Array::create(dst);

  auto execute_if_needed = [&](int dim, int idx, auto & input, auto & output) {
    if (dim > 1 && sigma[idx] > 0)
    {
      const ParameterList parameters = { { "src", input }, { "dst", output }, { "dim", idx }, { "N", radius[idx] }, { "s", sigma[idx] } };
      execute(device, kernel, parameters, global_range, local_range);
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
  // this is very similar to the execute function. However, this function is used for native kernels.
  // No preamble or code conversion is done here. The kernel source is expected to be in the correct format from the
  // user. We still however need to rely on the Array class to handle arrays (float * etc.). For now, we only support 1D
  // arrays and int and float parameters.

  // get raw kernel source and name from user
  auto kernel_source = kernel_func.second;
  auto kernel_name = kernel_func.first;

  // prepare parameters to be passed to the backend
  std::vector<std::shared_ptr<void>> args_ptr;
  std::vector<size_t>                args_size;

  args_ptr.reserve(parameters.size());
  args_size.reserve(parameters.size());
  for (const auto & param : parameters)
  {
    if (const auto & arr = std::get_if<Array::Pointer>(&param.second))
    {
      args_ptr.push_back(arr->get()->get_ptr());
      args_size.push_back(GPU_MEM_PTR_SIZE);
    }
    else if (const auto & f = std::get_if<float>(&param.second))
    {
      // Allocate memory for a float and copy the value
      auto float_ptr = std::make_shared<float>(*f);
      // Cast to shared_ptr<void>
      args_ptr.push_back(std::static_pointer_cast<void>(float_ptr));
      args_size.push_back(sizeof(float));
    }
    else if (const auto & i = std::get_if<int>(&param.second))
    {
      // Allocate memory for an int and copy the value
      auto int_ptr = std::make_shared<int>(*i);
      // Cast to shared_ptr<void>
      args_ptr.push_back(std::static_pointer_cast<void>(int_ptr));
      args_size.push_back(sizeof(int));
    }
    else if (const auto & i = std::get_if<unsigned int>(&param.second))
    {
      // Allocate memory for an unsigned int and copy the value
      auto uint_ptr = std::make_shared<unsigned int>(*i);
      // Cast to shared_ptr<void>
      args_ptr.push_back(std::static_pointer_cast<void>(uint_ptr));
      args_size.push_back(sizeof(unsigned int));
    }
    else if (const auto & i = std::get_if<size_t>(&param.second))
    {
      // Allocate memory for a size_t and copy the value
      auto size_ptr = std::make_shared<size_t>(*i);
      // Cast to shared_ptr<void>
      args_ptr.push_back(std::static_pointer_cast<void>(size_ptr));
      args_size.push_back(sizeof(size_t));
    }
    else
    {
      throw std::runtime_error("Error: Invalid parameter type provided.");
    }
  }
  // execute kernel
  cle::BackendManager::getInstance().getBackend().executeKernel(device, kernel_source, kernel_name, global_range, local_range, args_ptr, args_size);
}

} // namespace cle
