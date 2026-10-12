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

/**
 * @brief Trim trailing whitespace and null characters from a string (in-place)
 * @param s The string to trim
 */
inline void
trim_string(std::string & s)
{
  const auto end = s.find_last_not_of(std::string(" \t\n\0", 4));
  s.erase(end == std::string::npos ? 0 : end + 1);
}

/**
 * @brief Query a string information of an OpenCL object (device or platform), whatever its length
 * @param get_info clGetDeviceInfo or clGetPlatformInfo
 * @return The information as a string, or an empty string on error
 */
template <typename Object, typename Param, typename GetInfo>
std::string
query_string(GetInfo get_info, Object object, Param param)
{
  size_t size = 0;
  cl_int err = get_info(object, param, 0, nullptr, &size);
  if (err != CL_SUCCESS || size == 0)
  {
    std::cerr << "Failed to retrieve OpenCL info (param: " << param << ", error: " << err << ")" << std::endl;
    return "";
  }
  std::string buffer(size, '\0');
  err = get_info(object, param, size, buffer.data(), nullptr);
  if (err != CL_SUCCESS)
  {
    std::cerr << "Failed to retrieve OpenCL info (param: " << param << ", error: " << err << ")" << std::endl;
    return "";
  }
  trim_string(buffer);
  return buffer;
}

inline std::string
device_info_string(cl_device_id device, cl_device_info param)
{
  return query_string(clGetDeviceInfo, device, param);
}

inline std::string
platform_info_string(cl_platform_id platform, cl_platform_info param)
{
  return query_string(clGetPlatformInfo, platform, param);
}

/**
 * @brief Query a fixed-size device information (e.g. cl_uint, cl_ulong, size_t)
 * @return The value, or a value-initialised T on error
 */
template <typename T>
T
device_info(cl_device_id device, cl_device_info param)
{
  T value{};
  if (clGetDeviceInfo(device, param, sizeof(T), &value, nullptr) != CL_SUCCESS)
  {
    return T{};
  }
  return value;
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
  cl_uint count = 0;
  cl_int  err = clGetContextInfo(ptr, CL_CONTEXT_NUM_DEVICES, sizeof(cl_uint), &count, nullptr);
  if (err != CL_SUCCESS)
  {
    std::cerr << "Failed to get OpenCL context number of devices" << std::endl;
  }
  nb_device = count;
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

auto
OpenCLDevice::CommandQueue::get() const -> const cl_command_queue &
{
  return ptr;
}

OpenCLDevice::Resources::Resources(const cl_platform_id & platform, const cl_device_id & device, size_t device_index)
  : device_ptr(device)
  , platform_ptr(platform)
  , device_index(device_index)
{
  // Get platform information
  platform_name = platform_info_string(platform_ptr, CL_PLATFORM_NAME);

  // Get device information
  device_name = device_info_string(device_ptr, CL_DEVICE_NAME);

  cl_device_type type_buffer = 0;
  cl_int         err = clGetDeviceInfo(device_ptr, CL_DEVICE_TYPE, sizeof(cl_device_type), &type_buffer, nullptr);
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

OpenCLDevice::OpenCLDevice(const std::shared_ptr<Resources> &    resources,
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

  // Wait for the pending work, whatever the wait setting
  clFinish(clCommandQueue->get());

  // Release the queue and the context. The cached kernels belong to that context; the resources (platform and
  // device identity) are kept, so that the device can be initialized again.
  cache = std::make_shared<ProgramCache>();
  clCommandQueue.reset();
  clContext.reset();

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
  return lowercase ? to_lower(clResources->device_name) : clResources->device_name;
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
  auto         it = type_map.find(clResources->device_type);
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
  return static_cast<size_t>(device_info<cl_ulong>(clResources->get_device(), CL_DEVICE_MAX_MEM_ALLOC_SIZE));
}

auto
OpenCLDevice::getLocalMemorySize() const -> size_t
{
  return static_cast<size_t>(device_info<cl_ulong>(clResources->get_device(), CL_DEVICE_LOCAL_MEM_SIZE));
}

auto
OpenCLDevice::getMaximumWorkGroupSize() const -> size_t
{
  return device_info<size_t>(clResources->get_device(), CL_DEVICE_MAX_WORK_GROUP_SIZE);
}

auto
OpenCLDevice::getInfo() const -> std::string
{
  std::ostringstream result;

  // Get device information safely
  const auto   device = clResources->get_device();
  std::string  version = device_info_string(device, CL_DEVICE_VERSION);
  std::string  vendor = device_info_string(device, CL_DEVICE_VENDOR);
  std::string  driver = device_info_string(device, CL_DRIVER_VERSION);
  const auto & name = getName();

  // Get numeric device information (with their exact OpenCL types)
  const auto compute_units = device_info<cl_uint>(device, CL_DEVICE_MAX_COMPUTE_UNITS);
  const auto max_clock_frequency = device_info<cl_uint>(device, CL_DEVICE_MAX_CLOCK_FREQUENCY);
  const auto image_support = device_info<cl_bool>(device, CL_DEVICE_IMAGE_SUPPORT);
  const auto global_mem_size = device_info<cl_ulong>(device, CL_DEVICE_GLOBAL_MEM_SIZE);
  const auto max_mem_size = device_info<cl_ulong>(device, CL_DEVICE_MAX_MEM_ALLOC_SIZE);
  const auto local_mem_size = device_info<cl_ulong>(device, CL_DEVICE_LOCAL_MEM_SIZE);
  const auto device_type = device_info<cl_device_type>(device, CL_DEVICE_TYPE);

  // Get device type string
  const auto & type_map = get_device_type_map(true);
  auto         type_it = type_map.find(device_type);
  std::string  device_type_str = type_it != type_map.end() ? type_it->second : "Unknown";

  // Format output
  result << std::left << "(" << this->getType() << ") " << name << " (" << version << ")\n";
  result << std::left << std::setw(30) << "\tVendor: " << vendor << "\n";
  result << std::left << std::setw(30) << "\tDriver Version: " << driver << "\n";
  result << std::left << std::setw(30) << "\tDevice Type: " << device_type_str << "\n";
  result << std::left << std::setw(30) << "\tCompute Units: " << compute_units << '\n';
  result << std::left << std::setw(30) << "\tGlobal Memory Size: " << (global_mem_size / (1024 * 1024)) << " MB\n";
  result << std::left << std::setw(30) << "\tLocal Memory Size: " << (local_mem_size / 1024) << " KB\n";
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
  const auto  device = clResources->get_device();
  std::string extensions = device_info_string(device, CL_DEVICE_EXTENSIONS);

  const auto max_work_group_size = device_info<size_t>(device, CL_DEVICE_MAX_WORK_GROUP_SIZE);
  const auto max_work_item_dimensions = device_info<cl_uint>(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS);
  size_t     max_work_item_sizes[3] = { 0, 0, 0 };
  clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(max_work_item_sizes), &max_work_item_sizes, nullptr);

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

#endif // USE_OPENCL

} // namespace cle
