#include "device.hpp"
#include "utils.hpp"
#include <iomanip>  // for std::setw and std::left
#include <iterator> // Add this line
#include <map>      // for std::map
#include <vector>   // for std::vector

namespace cle
{

#if USE_OPENCL

OpenCLDevice::OpenCLDevice(const cl_platform_id & platform, const cl_device_id & device)
  : clDevice(device)
  , clPlatform(platform)
{
  initialize();
}

OpenCLDevice::~OpenCLDevice()
{
  if (isInitialized())
  {
    finalize();
  }
}

[[nodiscard]] auto
OpenCLDevice::getPlatform() const -> const std::string
{
  // from cl_platform_id to std::string
  char platform_name[256];
  clGetPlatformInfo(clPlatform, CL_PLATFORM_NAME, sizeof(char) * 256, &platform_name, nullptr);
  return std::string(platform_name);
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
  clContext = clCreateContext(nullptr, 1, &clDevice, nullptr, nullptr, &err);
  if (err != CL_SUCCESS)
  {
    std::cerr << "Failed to create OpenCL context" << std::endl;
    return;
  }
  clCommandQueue = clCreateCommandQueue(clContext, clDevice, 0, &err); // clCreateCommandQueue deprecated in OpenCL 2.0+
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
    std::cerr << "OpenCL device not initialized" << std::endl;
    return;
  }
  this->waitFinish = true;
  this->finish();
  clReleaseContext(clContext);
  clReleaseCommandQueue(clCommandQueue);
  clReleaseDevice(clDevice);
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
    clFinish(clCommandQueue);
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
  return clPlatform;
}

auto
OpenCLDevice::getCLDevice() const -> const cl_device_id &
{
  return clDevice;
}

auto
OpenCLDevice::getCLContext() const -> const cl_context &
{
  return clContext;
}

auto
OpenCLDevice::getCLCommandQueue() const -> const cl_command_queue &
{
  return clCommandQueue;
}

auto
OpenCLDevice::getName(bool lowercase) const -> std::string
{
  char device_name[256];
  clGetDeviceInfo(clDevice, CL_DEVICE_NAME, sizeof(char) * 256, &device_name, nullptr);
  if (lowercase)
  {
    return to_lower(std::string(device_name));
  }
  return std::string(device_name);
}

auto
OpenCLDevice::supportImage() const -> const bool
{
  cl_bool image_support;
  clGetDeviceInfo(clDevice, CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_bool), &image_support, nullptr);
  return image_support == CL_TRUE;
}

auto
OpenCLDevice::getInfo() const -> std::string
{
  std::ostringstream result;
  char               version[256], vendor[256], driver[256], extensions[1024], dev_type[256];
  cl_device_type     type;
  cl_uint            compute_units, max_work_group_size, max_clock_frequency, max_work_item_dimensions, image_support;
  size_t             global_mem_size, max_mem_size;
  size_t             max_work_item_sizes[3];

  // Get device information
  const auto & name = getName();
  clGetDeviceInfo(clDevice, CL_DEVICE_VERSION, sizeof(char) * 256, &version, nullptr);
  clGetDeviceInfo(clDevice, CL_DEVICE_VENDOR, sizeof(char) * 256, &vendor, nullptr);
  clGetDeviceInfo(clDevice, CL_DRIVER_VERSION, sizeof(char) * 256, &driver, nullptr);
  clGetDeviceInfo(clDevice, CL_DEVICE_EXTENSIONS, sizeof(char) * 1024, &extensions, nullptr);
  clGetDeviceInfo(clDevice, CL_DEVICE_TYPE, sizeof(cl_device_type), &type, nullptr);
  clGetDeviceInfo(clDevice, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &compute_units, nullptr);
  clGetDeviceInfo(clDevice, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(size_t), &global_mem_size, nullptr);
  clGetDeviceInfo(clDevice, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(size_t), &max_mem_size, nullptr);
  clGetDeviceInfo(clDevice, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(cl_uint), &max_work_group_size, nullptr);
  clGetDeviceInfo(clDevice, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &max_clock_frequency, nullptr);
  clGetDeviceInfo(clDevice, CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_uint), &image_support, nullptr);

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
  result << std::left << std::setw(30) << "\tMaximum Object Size: " << (max_mem_size / (1024 * 1024)) << " MB\n";
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
  clGetDeviceInfo(clDevice, CL_DEVICE_EXTENSIONS, sizeof(char) * 1024, &extensions, nullptr);
  clGetDeviceInfo(clDevice, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(cl_uint), &max_work_group_size, nullptr);
  clGetDeviceInfo(clDevice, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &max_work_item_dimensions, nullptr);
  clGetDeviceInfo(clDevice, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * 3, &max_work_item_sizes, nullptr);

  // Split extensions string into a vector
  std::istringstream       iss((std::string(extensions)));
  std::vector<std::string> extensionsVec((std::istream_iterator<std::string>(iss)),
                                         std::istream_iterator<std::string>());

  result << this->getInfo();
  result << std::left << std::setw(30) << "\tMax Work Group Size: " << max_work_group_size << '\n';
  result << std::left << std::setw(30) << "\tMax Work Item Dimensions: " << max_work_item_dimensions << '\n';
  result << std::left << std::setw(30) << "\tMax Work Item Sizes: " << max_work_item_sizes[0] << ", "
         << max_work_item_sizes[1] << ", " << max_work_item_sizes[2] << '\n';
  result << std::left << std::setw(30) << "\tExtensions:";
  for (const auto & extension : extensionsVec)
  {
    result << "\n\t\t" << std::left << std::setw(30) << extension;
  }

  return result.str();
}


#endif // USE_OPENCL

} // namespace cle
