#include "device.hpp"
#include "utils.hpp"
#include <iomanip>  // for std::setw and std::left
#include <iterator> // Add this line
#include <map>      // for std::map
#include <vector>   // for std::vector

namespace cle
{

#if USE_OPENCL

namespace
{
// Named constants for OpenCL operations
constexpr size_t OPENCL_NAME_BUFFER_SIZE = 256;
constexpr size_t OPENCL_EXTENSIONS_BUFFER_SIZE = 1024;

/**
 * @brief Trim trailing whitespace from a string (in-place)
 * @param s The string to trim
 */
inline void
trim_string(std::string & s)
{
  s.erase(s.find_last_not_of(" ") + 1);
}

/**
 * @brief Safely retrieve device information from OpenCL
 * @param device The OpenCL device
 * @param param The parameter to retrieve
 * @param buffer_size The size of the buffer
 * @return The device information as a string, or empty string on error
 */
inline std::string
safe_get_device_info(cl_device_id device, cl_device_info param, size_t buffer_size)
{
  std::string buffer(buffer_size, '\0');
  cl_int      err = clGetDeviceInfo(device, param, buffer_size, buffer.data(), nullptr);
  if (err != CL_SUCCESS)
  {
    std::cerr << "Failed to retrieve OpenCL device info (param: " << param << ", error: " << err << ")" << std::endl;
    return "";
  }
  // Find the actual end of the string (C-string might be shorter than buffer)
  size_t len = buffer.find('\0');
  if (len != std::string::npos)
  {
    buffer.resize(len);
  }
  trim_string(buffer);
  return buffer;
}

/**
 * @brief Get the device type map
 * @return A map of CL_DEVICE_TYPE to human-readable string
 */
inline const std::map<cl_device_type, std::string> &
get_device_type_map(bool uppercase = true)
{
  static const std::map<cl_device_type, std::string> lowercase_map = {
    { CL_DEVICE_TYPE_CPU, "cpu" },
    { CL_DEVICE_TYPE_GPU, "gpu" },
    { CL_DEVICE_TYPE_ACCELERATOR, "accelerator" },
    { CL_DEVICE_TYPE_CUSTOM, "custom" },
  };
  static const std::map<cl_device_type, std::string> uppercase_map = {
    { CL_DEVICE_TYPE_CPU, "CPU" },
    { CL_DEVICE_TYPE_GPU, "GPU" },
    { CL_DEVICE_TYPE_ACCELERATOR, "Accelerator" },
    { CL_DEVICE_TYPE_CUSTOM, "Custom" },
  };
  return uppercase ? uppercase_map : lowercase_map;
}

} // namespace




OpenCLDevice::Context::Context(const cl_context & ptr)
  : ptr(ptr)
{
  cl_int err = clGetContextInfo(ptr, CL_CONTEXT_NUM_DEVICES, sizeof(size_t), &nb_device, nullptr);
  if (err != CL_SUCCESS)
  {
    std::cerr << "Failed to get OpenCL context number of devices" << std::endl;
  }
}

OpenCLDevice::Context::~Context()
{
  if (ptr != nullptr)
  {
    cl_int err = clReleaseContext(ptr);
    if (err != CL_SUCCESS)
    {
      std::cerr << "Failed to release OpenCL context" << std::endl;
    }
  }
}

auto
OpenCLDevice::Context::get() const -> const cl_context &
{
  return ptr;
}

OpenCLDevice::Context::Context(Context && other) noexcept
  : ptr(other.ptr)
  , nb_device(other.nb_device)
{
  other.ptr = nullptr;
}

OpenCLDevice::Context &
OpenCLDevice::Context::operator=(Context && other) noexcept
{
  if (this != &other)
  {
    if (ptr)
    {
      clReleaseContext(ptr);
    }
    ptr = other.ptr;
    nb_device = other.nb_device;
    other.ptr = nullptr;
  }
  return *this;
}

OpenCLDevice::CommandQueue::CommandQueue(const cl_command_queue & ptr)
  : ptr(ptr)
{}

OpenCLDevice::CommandQueue::~CommandQueue()
{
  if (ptr != nullptr)
  {
    cl_int err = clReleaseCommandQueue(ptr);
    if (err != CL_SUCCESS)
    {
      std::cerr << "Failed to release OpenCL command queue" << std::endl;
    }
  }
}

OpenCLDevice::CommandQueue::CommandQueue(CommandQueue && other) noexcept
  : ptr(other.ptr)
{
  other.ptr = nullptr;
}

OpenCLDevice::CommandQueue &
OpenCLDevice::CommandQueue::operator=(CommandQueue && other) noexcept
{
  if (this != &other)
  {
    if (ptr)
    {
      clReleaseCommandQueue(ptr);
    }
    ptr = other.ptr;
    other.ptr = nullptr;
  }
  return *this;
}


auto
OpenCLDevice::CommandQueue::get() const -> const cl_command_queue &
{
  return ptr;
}

OpenCLDevice::Resources::Resources(const cl_platform_id & platform, const cl_device_id & device, size_t device_index)
  : platform_ptr(platform)
  , device_ptr(device)
  , device_index(device_index)
{
  // Get platform information
  platform_name = safe_get_device_info(device, CL_DEVICE_PLATFORM, OPENCL_NAME_BUFFER_SIZE);
  
  char platform_vendor_buffer[OPENCL_NAME_BUFFER_SIZE] = {};
  cl_int err = clGetPlatformInfo(platform_ptr, CL_PLATFORM_VENDOR, OPENCL_NAME_BUFFER_SIZE, platform_vendor_buffer, nullptr);
  if (err == CL_SUCCESS)
  {
    platform_vendor = std::string(platform_vendor_buffer);
    trim_string(platform_vendor);
  }
  else
  {
    std::cerr << "Failed to retrieve OpenCL platform vendor (error: " << err << ")" << std::endl;
    platform_vendor = "";
  }

  // Get device information
  device_name = safe_get_device_info(device, CL_DEVICE_NAME, OPENCL_NAME_BUFFER_SIZE);
  
  cl_device_type type_buffer = 0;
  err = clGetDeviceInfo(device_ptr, CL_DEVICE_TYPE, sizeof(cl_device_type), &type_buffer, nullptr);
  if (err == CL_SUCCESS)
  {
    device_type = type_buffer;
  }
  else
  {
    std::cerr << "Failed to retrieve OpenCL device type (error: " << err << ")" << std::endl;
    device_type = CL_DEVICE_TYPE_DEFAULT;
  }

  cl_bool image_support_buffer = CL_FALSE;
  err = clGetDeviceInfo(device_ptr, CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_bool), &image_support_buffer, nullptr);
  if (err == CL_SUCCESS)
  {
    image_support = image_support_buffer;
  }
  else
  {
    std::cerr << "Failed to retrieve OpenCL image support info (error: " << err << ")" << std::endl;
    image_support = CL_FALSE;
  }
}

auto
OpenCLDevice::Resources::get_device() const -> const cl_device_id &
{
  return device_ptr;
}

auto
OpenCLDevice::Resources::get_platform() const -> const cl_platform_id &
{
  return platform_ptr;
}


OpenCLDevice::OpenCLDevice(const cl_platform_id & platform, const cl_device_id & device)
  : clResources(std::make_shared<Resources>(platform, device, 0))
{
  initialize();
}

OpenCLDevice::OpenCLDevice(const std::shared_ptr<Resources> &   resources,
                           const std::shared_ptr<Context> &      context,
                           const std::shared_ptr<CommandQueue> & command_queue)
  : clResources(resources)
  , clContext(context)
  , clCommandQueue(command_queue)
{
  initialized = true;
}

OpenCLDevice::~OpenCLDevice()
{
  // shared_ptr objects automatically release resources on destruction
}

[[nodiscard]] auto
OpenCLDevice::getPlatform() const -> std::string
{
  return clResources->platform_name;
}

auto
OpenCLDevice::getType() const -> Device::Type
{
  return Device::Type::OPENCL;
}

auto
OpenCLDevice::initialize() -> void
{
  if (isInitialized())
  {
    return;
  }

  cl_int err = CL_SUCCESS;
  clContext = std::make_shared<Context>(clCreateContext(nullptr, 1, &clResources->get_device(), nullptr, nullptr, &err));
  if (err != CL_SUCCESS)
  {
    std::cerr << "Failed to create OpenCL context" << std::endl;
    return;
  }
  clCommandQueue = std::make_shared<CommandQueue>(
    clCreateCommandQueue(clContext->get(), clResources->get_device(), 0, &err)); // clCreateCommandQueue deprecated in OpenCL 2.0+
  if (err != CL_SUCCESS)
  {
    std::cerr << "Failed to create OpenCL command queue" << std::endl;
    return;
  }
  initialized = true;
}

auto
OpenCLDevice::finalize() -> void
{
  if (!isInitialized())
  {
    return;
  }

  // force device to finish
  waitFinish = true;
  finish();
  waitFinish = false;

  // Release OpenCL resources in reverse order of acquisition
  clCommandQueue.reset();
  clContext.reset();
  clResources.reset();

  initialized = false;
}

auto
OpenCLDevice::finish() const -> void
{
  if (!isInitialized())
  {
    std::cerr << "OpenCL device not initialized" << std::endl;
    return;
  }
  if (waitFinish)
  {
    clFinish(clCommandQueue->get());
  }
}

auto
OpenCLDevice::setWaitToFinish(bool flag) -> void
{
  this->waitFinish = flag;
}

auto
OpenCLDevice::isInitialized() const -> bool
{
  return initialized;
}

auto
OpenCLDevice::getCLPlatform() const -> const cl_platform_id &
{
  return clResources->get_platform();
}

auto
OpenCLDevice::getCLDevice() const -> const cl_device_id &
{
  return clResources->get_device();
}

auto
OpenCLDevice::getCLContext() const -> const cl_context &
{
  return clContext->get();
}

auto
OpenCLDevice::getCLCommandQueue() const -> const cl_command_queue &
{
  return clCommandQueue->get();
}

auto
OpenCLDevice::getName(bool lowercase) const -> std::string
{
  return clResources->device_name;
}

auto
OpenCLDevice::getDeviceIndex() const -> size_t
{
  return clResources->device_index;
}

auto
OpenCLDevice::getDeviceType() const -> std::string
{
  const auto & type_map = get_device_type_map(false);
  auto         it       = type_map.find(clResources->device_type);
  return it != type_map.end() ? it->second : "unknown";
}

auto
OpenCLDevice::getNbDevicesFromContext() const -> size_t
{
  return clContext->nb_device;
}

auto
OpenCLDevice::supportImage() const -> bool
{
  return clResources->image_support;
}

auto
OpenCLDevice::getMaximumBufferSize() const -> size_t
{
  size_t mem_size;
  clGetDeviceInfo(clResources->get_device(), CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(size_t), &mem_size, nullptr);
  return mem_size;
}

auto
OpenCLDevice::getLocalMemorySize() const -> size_t
{
  size_t mem_size;
  clGetDeviceInfo(clResources->get_device(), CL_DEVICE_LOCAL_MEM_SIZE, sizeof(size_t), &mem_size, nullptr);
  return mem_size;
}

auto
OpenCLDevice::getMaximumWorkGroupSize() const -> size_t
{
  size_t max_work_group_size;
  clGetDeviceInfo(clResources->get_device(), CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &max_work_group_size, nullptr);
  return max_work_group_size;
}

auto
OpenCLDevice::getInfo() const -> std::string
{
  std::ostringstream result;

  // Get device information safely
  std::string version   = safe_get_device_info(clResources->get_device(), CL_DEVICE_VERSION, OPENCL_NAME_BUFFER_SIZE);
  std::string vendor    = safe_get_device_info(clResources->get_device(), CL_DEVICE_VENDOR, OPENCL_NAME_BUFFER_SIZE);
  std::string driver    = safe_get_device_info(clResources->get_device(), CL_DRIVER_VERSION, OPENCL_NAME_BUFFER_SIZE);
  const auto & name     = getName();
  
  // Get numeric device information
  cl_uint       compute_units        = 0;
  cl_uint       max_clock_frequency  = 0;
  cl_uint       image_support        = 0;
  size_t        global_mem_size      = 0;
  size_t        max_mem_size         = 0;
  size_t        local_mem_size       = 0;
  cl_device_type device_type         = CL_DEVICE_TYPE_DEFAULT;

  clGetDeviceInfo(clResources->get_device(), CL_DEVICE_TYPE, sizeof(cl_device_type), &device_type, nullptr);
  clGetDeviceInfo(clResources->get_device(), CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &compute_units, nullptr);
  clGetDeviceInfo(clResources->get_device(), CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(size_t), &global_mem_size, nullptr);
  clGetDeviceInfo(clResources->get_device(), CL_DEVICE_LOCAL_MEM_SIZE, sizeof(size_t), &local_mem_size, nullptr);
  clGetDeviceInfo(clResources->get_device(), CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(size_t), &max_mem_size, nullptr);
  clGetDeviceInfo(clResources->get_device(), CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &max_clock_frequency, nullptr);
  clGetDeviceInfo(clResources->get_device(), CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_uint), &image_support, nullptr);

  // Get device type string
  const auto & type_map      = get_device_type_map(true);
  auto         type_it       = type_map.find(device_type);
  std::string  device_type_str = type_it != type_map.end() ? type_it->second : "Unknown";

  // Format output
  result << std::left << "(" << this->getType() << ") " << name << " (" << version << ")\n";
  result << std::left << std::setw(30) << "\tVendor: " << vendor << "\n";
  result << std::left << std::setw(30) << "\tDriver Version: " << driver << "\n";
  result << std::left << std::setw(30) << "\tDevice Type: " << device_type_str << "\n";
  result << std::left << std::setw(30) << "\tCompute Units: " << compute_units << '\n';
  result << std::left << std::setw(30) << "\tGlobal Memory Size: " << (global_mem_size / (1024 * 1024)) << " MB\n";
  result << std::left << std::setw(30) << "\tLocal Memory Size: " << (local_mem_size / (1024 * 1024)) << " MB\n";
  result << std::left << std::setw(30) << "\tMaximum Buffer Size: " << (max_mem_size / (1024 * 1024)) << " MB\n";
  result << std::left << std::setw(30) << "\tMax Clock Frequency: " << max_clock_frequency << " MHz\n";
  result << std::left << std::setw(30) << "\tImage Support: " << (image_support ? "Yes" : "No") << '\n';

  return result.str();
}

auto
OpenCLDevice::getInfoExtended() const -> std::string
{
  std::ostringstream result;

  // Get extended device information
  std::string extensions = safe_get_device_info(clResources->get_device(), CL_DEVICE_EXTENSIONS, OPENCL_EXTENSIONS_BUFFER_SIZE);
  
  cl_uint max_work_group_size      = 0;
  cl_uint max_work_item_dimensions = 0;
  size_t  max_work_item_sizes[3]   = { 0, 0, 0 };
  
  clGetDeviceInfo(clResources->get_device(), CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(cl_uint), &max_work_group_size, nullptr);
  clGetDeviceInfo(clResources->get_device(), CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &max_work_item_dimensions, nullptr);
  clGetDeviceInfo(clResources->get_device(), CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * 3, &max_work_item_sizes, nullptr);

  // Split extensions string into a vector
  std::istringstream       iss(extensions);
  std::vector<std::string> extensions_vec((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

  result << this->getInfo();
  result << std::left << std::setw(30) << "\tMax Work Group Size: " << max_work_group_size << '\n';
  result << std::left << std::setw(30) << "\tMax Work Item Dimensions: " << max_work_item_dimensions << '\n';
  result << std::left << std::setw(30) << "\tMax Work Item Sizes: " << max_work_item_sizes[0] << ", " << max_work_item_sizes[1] << ", "
         << max_work_item_sizes[2] << '\n';
  result << std::left << std::setw(30) << "\tExtensions:";
  
  if (extensions_vec.empty())
  {
    result << " (none)\n";
  }
  else
  {
    for (const auto & extension : extensions_vec)
    {
      result << "\n\t\t" << std::left << std::setw(30) << extension;
    }
    result << '\n';
  }

  return result.str();
}

auto
OpenCLDevice::getProgramFromCache(const std::string & key) const -> std::shared_ptr<void>
{
  return cache->get(key);
}

auto
OpenCLDevice::addProgramToCache(const std::string & key, std::shared_ptr<void> program) -> void
{
  cache->put(key, program);
}


#endif // USE_OPENCL

} // namespace cle
