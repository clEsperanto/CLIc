#include "device.hpp"
#include "utils.hpp"
#include <iomanip>  // for std::setw and std::left
#include <iterator> // Add this line
#include <map>      // for std::map
#include <vector>   // for std::vector

namespace cle
{

#if USE_OPENCL


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

OpenCLDevice::Ressources::Ressources(const cl_platform_id & platform, const cl_device_id & device, size_t device_index)
  : platform_ptr(platform)
  , device_ptr(device)
  , device_index(device_index)
{
  char _platform_name[256];
  clGetPlatformInfo(platform_ptr, CL_PLATFORM_NAME, sizeof(char) * 256, &_platform_name, nullptr);
  platform_name = std::string(_platform_name);
  platform_name.erase(platform_name.find_last_not_of(" ") + 1);
  char _platform_vendor[256];
  clGetPlatformInfo(platform_ptr, CL_PLATFORM_VENDOR, sizeof(char) * 256, &_platform_vendor, nullptr);
  platform_vendor = std::string(_platform_vendor);
  platform_vendor.erase(platform_vendor.find_last_not_of(" ") + 1);
  char _device_name[256];
  clGetDeviceInfo(device_ptr, CL_DEVICE_NAME, sizeof(char) * 256, &_device_name, nullptr);
  device_name = std::string(_device_name);
  device_name.erase(device_name.find_last_not_of(" ") + 1);
  clGetDeviceInfo(device_ptr, CL_DEVICE_TYPE, sizeof(cl_device_type), &device_type, nullptr);
  clGetDeviceInfo(device_ptr, CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_bool), &image_support, nullptr);
}

// OpenCLDevice::Ressources::~Ressources()
// {
//   if (device_ptr != nullptr)
//   {
//     cl_int err = clReleaseDevice(device_ptr);
//     if (err != CL_SUCCESS)
//     {
//       std::cerr << "Failed to release OpenCL device" << std::endl;
//     }
//   }
// }

auto
OpenCLDevice::Ressources::get_device() const -> const cl_device_id &
{
  return device_ptr;
}

auto
OpenCLDevice::Ressources::get_platform() const -> const cl_platform_id &
{
  return platform_ptr;
}


OpenCLDevice::OpenCLDevice(const cl_platform_id & platform, const cl_device_id & device)
  : clRessources(std::make_shared<Ressources>(platform, device, 0))
{
  initialize();
}

OpenCLDevice::OpenCLDevice(const std::shared_ptr<Ressources> &   ressources,
                           const std::shared_ptr<Context> &      context,
                           const std::shared_ptr<CommandQueue> & command_queue)
  : clRessources(ressources)
  , clContext(context)
  , clCommandQueue(command_queue)
{
  initialized = true;
}

OpenCLDevice::~OpenCLDevice()
{
  if (clCommandQueue != nullptr)
  {
    clCommandQueue.reset();
  }
  if (clContext != nullptr)
  {
    clContext.reset();
  }
  if (clRessources != nullptr)
  {
    clRessources.reset();
  }
}

[[nodiscard]] auto
OpenCLDevice::getPlatform() const -> std::string
{
  return clRessources->platform_name;
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
  clContext = std::make_shared<Context>(clCreateContext(nullptr, 1, &clRessources->get_device(), nullptr, nullptr, &err));
  if (err != CL_SUCCESS)
  {
    std::cerr << "Failed to create OpenCL context" << std::endl;
    return;
  }
  clCommandQueue = std::make_shared<CommandQueue>(
    clCreateCommandQueue(clContext->get(), clRessources->get_device(), 0, &err)); // clCreateCommandQueue deprecated in OpenCL 2.0+
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
  // force device to finish
  waitFinish = true;
  finish();

  if (clCommandQueue != nullptr)
  {
    clCommandQueue.reset();
  }
  if (clContext != nullptr)
  {
    clContext.reset();
  }
  if (clRessources != nullptr)
  {
    clRessources.reset();
  }

  waitFinish = false;
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
  return clRessources->get_platform();
}

auto
OpenCLDevice::getCLDevice() const -> const cl_device_id &
{
  return clRessources->get_device();
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
  return clRessources->device_name;
}

auto
OpenCLDevice::getDeviceIndex() const -> size_t
{
  return clRessources->device_index;
}

auto
OpenCLDevice::getDeviceType() const -> std::string
{
  std::map<cl_device_type, std::string> deviceTypeMap = {
    { CL_DEVICE_TYPE_CPU, "cpu" },
    { CL_DEVICE_TYPE_GPU, "gpu" },
    { CL_DEVICE_TYPE_ACCELERATOR, "accelerator" },
    { CL_DEVICE_TYPE_CUSTOM, "custom" },
  };
  return deviceTypeMap.count(clRessources->device_type) ? deviceTypeMap[clRessources->device_type] : "Unknown";
}

auto
OpenCLDevice::getNbDevicesFromContext() const -> size_t
{
  return clContext->nb_device;
}

auto
OpenCLDevice::supportImage() const -> bool
{
  return clRessources->image_support;
}

auto
OpenCLDevice::getMaximumBufferSize() const -> size_t
{
  size_t mem_size;
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(size_t), &mem_size, nullptr);
  return mem_size;
}

auto
OpenCLDevice::getLocalMemorySize() const -> size_t
{
  size_t mem_size;
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_LOCAL_MEM_SIZE, sizeof(size_t), &mem_size, nullptr);
  return mem_size;
}

auto
OpenCLDevice::getMaximumWorkGroupSize() const -> size_t
{
  size_t max_work_group_size;
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &max_work_group_size, nullptr);
  return max_work_group_size;
}

auto
OpenCLDevice::getInfo() const -> std::string
{
  std::ostringstream result;
  char               version[256], vendor[256], driver[256], extensions[1024], dev_type[256];
  cl_device_type     type;
  cl_uint            compute_units, max_work_group_size, max_clock_frequency, max_work_item_dimensions, image_support;
  size_t             global_mem_size, max_mem_size, local_mem_size;
  size_t             max_work_item_sizes[3];

  // Get device information
  const auto & name = getName();
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_VERSION, sizeof(char) * 256, &version, nullptr);
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_VENDOR, sizeof(char) * 256, &vendor, nullptr);
  clGetDeviceInfo(clRessources->get_device(), CL_DRIVER_VERSION, sizeof(char) * 256, &driver, nullptr);
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_EXTENSIONS, sizeof(char) * 1024, &extensions, nullptr);
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_TYPE, sizeof(cl_device_type), &type, nullptr);
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &compute_units, nullptr);
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(size_t), &global_mem_size, nullptr);
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_LOCAL_MEM_SIZE, sizeof(size_t), &local_mem_size, nullptr);
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(size_t), &max_mem_size, nullptr);
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(cl_uint), &max_work_group_size, nullptr);
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &max_clock_frequency, nullptr);
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_uint), &image_support, nullptr);

  std::map<cl_device_type, std::string> deviceTypeMap = {
    { CL_DEVICE_TYPE_CPU, "CPU" },
    { CL_DEVICE_TYPE_GPU, "GPU" },
    { CL_DEVICE_TYPE_ACCELERATOR, "Accelerator" },
    { CL_DEVICE_TYPE_CUSTOM, "Custom" },
  };
  std::string dev_type_str = deviceTypeMap.count(type) ? deviceTypeMap[type] : "Unknown";
  // Print device information to output string
  result << std::left << "(" << this->getType() << ") " << name << " (" + std::string(version) << ")\n";
  result << std::left << std::setw(30) << "\tVendor: " << std::string(vendor) << "\n";
  result << std::left << std::setw(30) << "\tDriver Version: " << std::string(driver) << "\n";
  result << std::left << std::setw(30) << "\tDevice Type: " << dev_type_str << "\n";
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
  char               extensions[1024];
  cl_uint            max_work_group_size, max_work_item_dimensions;
  size_t             max_work_item_sizes[3];
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_EXTENSIONS, sizeof(char) * 1024, &extensions, nullptr);
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(cl_uint), &max_work_group_size, nullptr);
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &max_work_item_dimensions, nullptr);
  clGetDeviceInfo(clRessources->get_device(), CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * 3, &max_work_item_sizes, nullptr);

  // Split extensions string into a vector
  std::istringstream       iss((std::string(extensions)));
  std::vector<std::string> extensionsVec((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

  result << this->getInfo();
  result << std::left << std::setw(30) << "\tMax Work Group Size: " << max_work_group_size << '\n';
  result << std::left << std::setw(30) << "\tMax Work Item Dimensions: " << max_work_item_dimensions << '\n';
  result << std::left << std::setw(30) << "\tMax Work Item Sizes: " << max_work_item_sizes[0] << ", " << max_work_item_sizes[1] << ", "
         << max_work_item_sizes[2] << '\n';
  result << std::left << std::setw(30) << "\tExtensions:";
  for (const auto & extension : extensionsVec)
  {
    result << "\n\t\t" << std::left << std::setw(30) << extension;
  }

  return result.str();
}

auto
OpenCLDevice::getProgramFromCache(const std::string & key) const -> std::shared_ptr<void>
{
  return cache->getCachedProgram(key);
}

auto
OpenCLDevice::addProgramToCache(const std::string & key, std::shared_ptr<void> program) -> void
{
  cache->cacheProgram(key, program);
}


#endif // USE_OPENCL

} // namespace cle
