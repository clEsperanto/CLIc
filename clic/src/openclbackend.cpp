#include "backend.hpp"
#include "cle_preamble_cl.h"

#include <memory>
#include <numeric>

namespace cle
{

// ============================================================================
// Compile-time OpenCL guard — stub implementations when disabled
// ============================================================================

#if !USE_OPENCL

OpenCLBackend::OpenCLBackend()
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

OpenCLBackend::~OpenCLBackend() = default;

auto
OpenCLBackend::initialiseResources() -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::getDevices(const std::string &) const -> std::vector<Device::Pointer>
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::getDevice(const std::string &, const std::string &) const -> Device::Pointer
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::getDeviceFromIndex(size_t, const std::string &) const -> Device::Pointer
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::getDevicesList(const std::string &) const -> std::vector<std::string>
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::getType() const -> Backend::Type
{
  return Backend::Type::OPENCL;
}

auto
OpenCLBackend::allocateMemory(const Device::Pointer &, const std::array<size_t, 3> &, const dType &, const mType &, std::shared_ptr<void> &)
  const -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::allocateBuffer(const Device::Pointer &, const size_t &, std::shared_ptr<void> &) -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::allocateImage(const Device::Pointer &, const std::array<size_t, 3> &, const dType &, std::shared_ptr<void> &) -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::freeMemory(const Device::Pointer &, const mType &, const std::shared_ptr<void> &) const -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::getRefCount(void *) const -> int
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::writeMemory(const Device::Pointer &,
                           const std::shared_ptr<void> &,
                           std::array<size_t, 3> &,
                           std::array<size_t, 3> &,
                           std::array<size_t, 3> &,
                           const dType &,
                           const mType &,
                           const void *) const -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::writeBuffer(const Device::Pointer &,
                           const std::shared_ptr<void> &,
                           const std::array<size_t, 3> &,
                           const std::array<size_t, 3> &,
                           const std::array<size_t, 3> &,
                           const void *) -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::writeImage(const Device::Pointer &,
                          const std::shared_ptr<void> &,
                          const std::array<size_t, 3> &,
                          const std::array<size_t, 3> &,
                          const std::array<size_t, 3> &,
                          const void *) -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::readMemory(const Device::Pointer &,
                          const std::shared_ptr<void> &,
                          std::array<size_t, 3> &,
                          std::array<size_t, 3> &,
                          std::array<size_t, 3> &,
                          const dType &,
                          const mType &,
                          void *) const -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::readBuffer(const Device::Pointer &,
                          const std::shared_ptr<void> &,
                          const std::array<size_t, 3> &,
                          const std::array<size_t, 3> &,
                          const std::array<size_t, 3> &,
                          void *) -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::readImage(const Device::Pointer &,
                         const std::shared_ptr<void> &,
                         const std::array<size_t, 3> &,
                         const std::array<size_t, 3> &,
                         const std::array<size_t, 3> &,
                         void *) -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::copyMemoryBufferToBuffer(const Device::Pointer &,
                                        const std::shared_ptr<void> &,
                                        std::array<size_t, 3> &,
                                        std::array<size_t, 3> &,
                                        const std::shared_ptr<void> &,
                                        std::array<size_t, 3> &,
                                        std::array<size_t, 3> &,
                                        std::array<size_t, 3> &,
                                        const size_t &) const -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::copyMemoryImageToBuffer(const Device::Pointer &,
                                       const std::shared_ptr<void> &,
                                       std::array<size_t, 3> &,
                                       std::array<size_t, 3> &,
                                       const std::shared_ptr<void> &,
                                       std::array<size_t, 3> &,
                                       std::array<size_t, 3> &,
                                       std::array<size_t, 3> &,
                                       const size_t &) const -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::copyMemoryBufferToImage(const Device::Pointer &,
                                       const std::shared_ptr<void> &,
                                       std::array<size_t, 3> &,
                                       std::array<size_t, 3> &,
                                       const std::shared_ptr<void> &,
                                       std::array<size_t, 3> &,
                                       std::array<size_t, 3> &,
                                       std::array<size_t, 3> &,
                                       const size_t &) const -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::copyMemoryImageToImage(const Device::Pointer &,
                                      const std::shared_ptr<void> &,
                                      std::array<size_t, 3> &,
                                      std::array<size_t, 3> &,
                                      const std::shared_ptr<void> &,
                                      std::array<size_t, 3> &,
                                      std::array<size_t, 3> &,
                                      std::array<size_t, 3> &,
                                      const size_t &) const -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::setMemory(const Device::Pointer &,
                         const std::shared_ptr<void> &,
                         std::array<size_t, 3> &,
                         std::array<size_t, 3> &,
                         std::array<size_t, 3> &,
                         const dType &,
                         const mType &,
                         const float &) const -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::setBuffer(const Device::Pointer &,
                         const std::shared_ptr<void> &,
                         const std::array<size_t, 3> &,
                         const std::array<size_t, 3> &,
                         const std::array<size_t, 3> &,
                         const dType &,
                         const float &) -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::setImage(const Device::Pointer &,
                        const std::shared_ptr<void> &,
                        const std::array<size_t, 3> &,
                        const std::array<size_t, 3> &,
                        const std::array<size_t, 3> &,
                        const dType &,
                        const float &) -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::buildKernel(const Device::Pointer &, const std::string &, const std::string &, std::shared_ptr<void> &) const -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::executeKernel(const Device::Pointer &,
                             const std::string &,
                             const std::string &,
                             const std::array<size_t, 3> &,
                             const std::array<size_t, 3> &,
                             const std::vector<std::shared_ptr<void>> &,
                             const std::vector<size_t> &) const -> void
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

auto
OpenCLBackend::getPreamble() const -> std::string
{
  throw std::runtime_error("Error: OpenCL is not enabled");
}

#else // USE_OPENCL ═══════════════════════════════════════════════════════════

// ============================================================================
// Error Handling
// ============================================================================

/**
 * @brief Convert an OpenCL error code to a human-readable string.
 */
[[nodiscard]] static auto
getErrorString(cl_int error) -> std::string
{
  static const std::unordered_map<cl_int, std::string> errorMap = {
    { CL_SUCCESS, "CL_SUCCESS" },
    { CL_DEVICE_NOT_FOUND, "CL_DEVICE_NOT_FOUND" },
    { CL_DEVICE_NOT_AVAILABLE, "CL_DEVICE_NOT_AVAILABLE" },
    { CL_COMPILER_NOT_AVAILABLE, "CL_COMPILER_NOT_AVAILABLE" },
    { CL_MEM_OBJECT_ALLOCATION_FAILURE, "CL_MEM_OBJECT_ALLOCATION_FAILURE" },
    { CL_OUT_OF_RESOURCES, "CL_OUT_OF_RESOURCES" },
    { CL_OUT_OF_HOST_MEMORY, "CL_OUT_OF_HOST_MEMORY" },
    { CL_PROFILING_INFO_NOT_AVAILABLE, "CL_PROFILING_INFO_NOT_AVAILABLE" },
    { CL_MEM_COPY_OVERLAP, "CL_MEM_COPY_OVERLAP" },
    { CL_IMAGE_FORMAT_MISMATCH, "CL_IMAGE_FORMAT_MISMATCH" },
    { CL_IMAGE_FORMAT_NOT_SUPPORTED, "CL_IMAGE_FORMAT_NOT_SUPPORTED" },
    { CL_BUILD_PROGRAM_FAILURE, "CL_BUILD_PROGRAM_FAILURE" },
    { CL_MAP_FAILURE, "CL_MAP_FAILURE" },
    { CL_INVALID_VALUE, "CL_INVALID_VALUE" },
    { CL_INVALID_DEVICE_TYPE, "CL_INVALID_DEVICE_TYPE" },
    { CL_INVALID_PLATFORM, "CL_INVALID_PLATFORM" },
    { CL_INVALID_DEVICE, "CL_INVALID_DEVICE" },
    { CL_INVALID_CONTEXT, "CL_INVALID_CONTEXT" },
    { CL_INVALID_QUEUE_PROPERTIES, "CL_INVALID_QUEUE_PROPERTIES" },
    { CL_INVALID_COMMAND_QUEUE, "CL_INVALID_COMMAND_QUEUE" },
    { CL_INVALID_HOST_PTR, "CL_INVALID_HOST_PTR" },
    { CL_INVALID_MEM_OBJECT, "CL_INVALID_MEM_OBJECT" },
    { CL_INVALID_IMAGE_FORMAT_DESCRIPTOR, "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR" },
    { CL_INVALID_IMAGE_SIZE, "CL_INVALID_IMAGE_SIZE" },
    { CL_INVALID_SAMPLER, "CL_INVALID_SAMPLER" },
    { CL_INVALID_BINARY, "CL_INVALID_BINARY" },
    { CL_INVALID_BUILD_OPTIONS, "CL_INVALID_BUILD_OPTIONS" },
    { CL_INVALID_PROGRAM, "CL_INVALID_PROGRAM" },
    { CL_INVALID_PROGRAM_EXECUTABLE, "CL_INVALID_PROGRAM_EXECUTABLE" },
    { CL_INVALID_KERNEL_NAME, "CL_INVALID_KERNEL_NAME" },
    { CL_INVALID_KERNEL_DEFINITION, "CL_INVALID_KERNEL_DEFINITION" },
    { CL_INVALID_KERNEL, "CL_INVALID_KERNEL" },
    { CL_INVALID_ARG_INDEX, "CL_INVALID_ARG_INDEX" },
    { CL_INVALID_ARG_VALUE, "CL_INVALID_ARG_VALUE" },
    { CL_INVALID_ARG_SIZE, "CL_INVALID_ARG_SIZE" },
    { CL_INVALID_KERNEL_ARGS, "CL_INVALID_KERNEL_ARGS" },
    { CL_INVALID_WORK_DIMENSION, "CL_INVALID_WORK_DIMENSION" },
    { CL_INVALID_WORK_GROUP_SIZE, "CL_INVALID_WORK_GROUP_SIZE" },
    { CL_INVALID_WORK_ITEM_SIZE, "CL_INVALID_WORK_ITEM_SIZE" },
    { CL_INVALID_GLOBAL_OFFSET, "CL_INVALID_GLOBAL_OFFSET" },
    { CL_INVALID_EVENT_WAIT_LIST, "CL_INVALID_EVENT_WAIT_LIST" },
    { CL_INVALID_EVENT, "CL_INVALID_EVENT" },
    { CL_INVALID_OPERATION, "CL_INVALID_OPERATION" },
    { CL_INVALID_GL_OBJECT, "CL_INVALID_GL_OBJECT" },
    { CL_INVALID_BUFFER_SIZE, "CL_INVALID_BUFFER_SIZE" },
    { CL_INVALID_MIP_LEVEL, "CL_INVALID_MIP_LEVEL" },
    { CL_INVALID_GLOBAL_WORK_SIZE, "CL_INVALID_GLOBAL_WORK_SIZE" },
  };
  const auto it = errorMap.find(error);
  return (it != errorMap.end()) ? it->second : "CL_UNKNOWN_ERROR";
}

/// Check an OpenCL API call; throw with context on failure.
#  define CL_CHECK(call, msg)                                                                                                      \
    do                                                                                                                             \
    {                                                                                                                              \
      cl_int err_ = (call);                                                                                                        \
      if (err_ != CL_SUCCESS)                                                                                                      \
      {                                                                                                                            \
        throw std::runtime_error(std::string(msg) + " OpenCL error: " + getErrorString(err_) + " (" + std::to_string(err_) + ")"); \
      }                                                                                                                            \
    } while (0)

// ============================================================================
// Device cast helper
// ============================================================================

/**
 * @brief Cast a Device::Pointer to OpenCLDevice, throwing on failure.
 *
 * Eliminates the repeated dynamic_pointer_cast + validate pattern from
 * every method that needs the underlying OpenCL device.
 */
[[nodiscard]] static auto
castDevice(const Device::Pointer & device) -> std::shared_ptr<const OpenCLDevice>
{
  auto opencl_device = std::dynamic_pointer_cast<const OpenCLDevice>(device);
  if (!opencl_device)
  {
    throw std::runtime_error("Error: Failed to cast device to OpenCLDevice");
  }
  return opencl_device;
}

// ============================================================================
// Buffer pitch helpers
// ============================================================================

/**
 * @brief Compute row and slice pitches for buffer rect operations.
 *
 * OpenCL rect operations require:
 *   - row_pitch = 0 for 1D, shape[0] (in bytes) for 2D+
 *   - slice_pitch = 0 for 1D/2D, shape[0]*shape[1] (in bytes) for 3D
 */
struct BufferPitches
{
  size_t row;
  size_t slice;
};

[[nodiscard]] static auto
computeBufferPitches(const std::array<size_t, 3> & shape) -> BufferPitches
{
  return { (shape[1] > 1) ? shape[0] : 0, (shape[2] > 1) ? shape[0] * shape[1] : 0 };
}

// ============================================================================
// Image channel type helper
// ============================================================================

/**
 * @brief Map a dType to the corresponding OpenCL image channel data type.
 *
 * Logs a warning and defaults to CL_FLOAT for unsupported types.
 */
[[nodiscard]] static auto
toImageChannelType(const dType & dtype) -> cl_channel_type
{
  switch (dtype)
  {
    case dType::FLOAT:
      return CL_FLOAT;
    case dType::INT32:
      return CL_SIGNED_INT32;
    case dType::UINT32:
      return CL_UNSIGNED_INT32;
    case dType::INT16:
      return CL_SIGNED_INT16;
    case dType::UINT16:
      return CL_UNSIGNED_INT16;
    case dType::INT8:
      return CL_SIGNED_INT8;
    case dType::UINT8:
      return CL_UNSIGNED_INT8;
    default:
      std::cerr << "Warning: Unsupported data type for image allocation. Defaulting to float." << std::endl;
      return CL_FLOAT;
  }
}

// ============================================================================
// Fill helpers (reduce switch/case repetition in setBuffer/setImage)
// ============================================================================

/**
 * @brief Type-safe buffer fill: cast value to T and enqueue fill.
 */
template <typename T>
static auto
fillBufferTyped(cl_command_queue queue, cl_mem mem, size_t size_bytes, float value) -> cl_int
{
  const auto cval = static_cast<T>(value);
  return clEnqueueFillBuffer(queue, mem, &cval, sizeof(cval), 0, size_bytes, 0, nullptr, nullptr);
}

/**
 * @brief Type-safe image fill: create a 4-element fill colour and enqueue.
 */
template <typename T>
static auto
fillImageTyped(cl_command_queue queue, cl_mem mem, const std::array<size_t, 3> & origin, const std::array<size_t, 3> & region, float value)
  -> cl_int
{
  const T cval[4] = { static_cast<T>(value), static_cast<T>(value), static_cast<T>(value), static_cast<T>(value) };
  return clEnqueueFillImage(queue, mem, cval, origin.data(), region.data(), 0, nullptr, nullptr);
}

// ============================================================================
// Shared pointer wrapper for OpenCL objects with custom deleters
// ============================================================================

/**
 * @brief Wrap a cl_mem in a shared_ptr with automatic clReleaseMemObject.
 */
[[nodiscard]] static auto
wrapMemObject(cl_mem mem) -> std::shared_ptr<void>
{
  return std::shared_ptr<void>(mem, [](void * p) {
    if (p)
    {
      clReleaseMemObject(static_cast<cl_mem>(p));
    }
  });
}

/**
 * @brief Wrap a cl_program in a shared_ptr with automatic clReleaseProgram.
 */
[[nodiscard]] static auto
wrapProgram(cl_program program) -> std::shared_ptr<void>
{
  return std::shared_ptr<void>(program, [](void * p) {
    if (p)
    {
      clReleaseProgram(static_cast<cl_program>(p));
    }
  });
}

/**
 * @brief Wrap a cl_kernel in a shared_ptr with automatic clReleaseKernel.
 */
[[nodiscard]] static auto
wrapKernel(cl_kernel kernel) -> std::shared_ptr<void>
{
  return std::shared_ptr<void>(kernel, [](void * p) {
    if (p)
    {
      clReleaseKernel(static_cast<cl_kernel>(p));
    }
  });
}

// ============================================================================
// Program build helpers
// ============================================================================

/**
 * @brief Build an OpenCL program for the given device.
 *
 * On failure, retrieves and includes the build log in the exception message.
 */
static auto
buildProgram(const std::shared_ptr<const OpenCLDevice> & opencl_device, const std::shared_ptr<void> & program) -> void
{
  if (!program)
  {
    throw std::runtime_error("Error: Program pointer is null.");
  }

  const auto   cl_prog = reinterpret_cast<cl_program>(program.get());
  const cl_int buildStatus = clBuildProgram(cl_prog, 1, &opencl_device->getCLDevice(), "-w", nullptr, nullptr);

  if (buildStatus != CL_SUCCESS)
  {
    // Retrieve build log for debugging
    size_t len = 0;
    clGetProgramBuildInfo(cl_prog, opencl_device->getCLDevice(), CL_PROGRAM_BUILD_LOG, 0, nullptr, &len);

    std::string log;
    if (len > 0)
    {
      log.resize(len);
      clGetProgramBuildInfo(cl_prog, opencl_device->getCLDevice(), CL_PROGRAM_BUILD_LOG, len, log.data(), &len);
    }

    std::string error_msg = "Error: Failed to build OpenCL program.\n";
    error_msg += "OpenCL error: " + getErrorString(buildStatus) + " (" + std::to_string(buildStatus) + ")\n";
    if (!log.empty())
    {
      error_msg += "Build log:\n" + log;
    }
    throw std::runtime_error(error_msg);
  }
}

/**
 * @brief Save a compiled program binary to the disk cache.
 *
 * Extracts the binary for the first device in the program's device list
 * (assumes 1 context = 1 device as per current architecture).
 */
static auto
saveBinaryToCache(const std::string &           device_hash,
                  const std::string &           source_hash,
                  const std::shared_ptr<void> & program,
                  const Device::Pointer &       device) -> void
{
  // Note: only 1 device per context
  // https://github.com/clEsperanto/CLIc/pull/420/commits/e246f6d065b503b61a70b423db25738d577dda0b
  constexpr size_t device_index = 0;
  const size_t     nb_devices = device->getNbDevicesFromContext();
  const auto       cl_prog = reinterpret_cast<cl_program>(program.get());

  // Query binary sizes
  auto bin_size_list = std::make_unique<size_t[]>(nb_devices);
  CL_CHECK(clGetProgramInfo(cl_prog, CL_PROGRAM_BINARY_SIZES, sizeof(size_t) * nb_devices, bin_size_list.get(), nullptr),
           "Error: Failed to query program binary sizes.");

  // Allocate storage for each device's binary
  auto bin_value_list = std::make_unique<std::unique_ptr<char[]>[]>(nb_devices);
  auto bin_ptrs = std::make_unique<char *[]>(nb_devices);
  for (size_t i = 0; i < nb_devices; ++i)
  {
    bin_value_list[i] = std::make_unique<char[]>(bin_size_list[i]);
    bin_ptrs[i] = bin_value_list[i].get();
  }

  CL_CHECK(clGetProgramInfo(cl_prog, CL_PROGRAM_BINARIES, sizeof(unsigned char *) * nb_devices, bin_ptrs.get(), nullptr),
           "Error: Failed to fetch program binary.");

  DiskCache::instance().saveBinary(device_hash, source_hash, "bin", bin_value_list[device_index].get(), bin_size_list[device_index]);
}

/**
 * @brief Attempt to load a cached program binary from disk and rebuild it.
 *
 * @return A shared_ptr to the rebuilt program, or nullptr if not found / failed.
 */
[[nodiscard]] static auto
loadProgramFromCache(const std::shared_ptr<const OpenCLDevice> & opencl_device,
                     const std::string &                         device_hash,
                     const std::string &                         source_hash) -> std::shared_ptr<void>
{
  const auto binary = DiskCache::instance().loadBinary(device_hash, source_hash, "bin");
  if (binary.empty())
  {
    return nullptr;
  }

  auto         binary_size = binary.size();
  const auto * binary_code_ptr = binary.data();
  cl_int       status;
  cl_int       err;
  auto         program = clCreateProgramWithBinary(
    opencl_device->getCLContext(), 1, &opencl_device->getCLDevice(), &binary_size, &binary_code_ptr, &status, &err);

  if (status != CL_SUCCESS || program == nullptr)
  {
    std::cerr << "Warning: Failed to create program from cached binary. Status: " << getErrorString(status) << " (" << status
              << "), Error: " << getErrorString(err) << " (" << err << ")" << std::endl;
    return nullptr;
  }

  auto program_ptr = wrapProgram(program);

  try
  {
    buildProgram(opencl_device, program_ptr);
  }
  catch (const std::exception & e)
  {
    std::cerr << "Warning: Failed to build program from cached binary: " << e.what() << std::endl;
    return nullptr;
  }

  return program_ptr;
}

/**
 * @brief Create and build an OpenCL program from source code.
 */
[[nodiscard]] static auto
createProgramFromSource(const std::shared_ptr<const OpenCLDevice> & opencl_device, const std::string & kernel_source)
  -> std::shared_ptr<void>
{
  cl_int       err;
  const char * source = kernel_source.c_str();
  auto         program = clCreateProgramWithSource(opencl_device->getCLContext(), 1, &source, nullptr, &err);
  CL_CHECK(err, "Error: Failed to create program from source.");

  auto program_ptr = wrapProgram(program);
  buildProgram(opencl_device, program_ptr);
  return program_ptr;
}

// ============================================================================
// OpenCLBackend Implementation
// ============================================================================

OpenCLBackend::OpenCLBackend()
{
  initialiseResources();
}

OpenCLBackend::~OpenCLBackend() = default;

// ── Resource initialisation ─────────────────────────────────────────────────

auto
OpenCLBackend::initialiseResources() -> void
{
  // Discover all platforms and their devices
  std::unordered_map<cl_platform_id, std::vector<cl_device_id>> resources;
  size_t                                                        device_counter = 0;

  cl_uint platformCount = 0;
  cl_int  err = clGetPlatformIDs(0, nullptr, &platformCount);
  if (err != CL_SUCCESS || platformCount == 0)
  {
    throw std::runtime_error("Error: Failed to find any OpenCL compatible platforms.");
  }

  std::vector<cl_platform_id> platformIds(platformCount);
  err = clGetPlatformIDs(platformCount, platformIds.data(), nullptr);
  if (err != CL_SUCCESS)
  {
    throw std::runtime_error("Error: Failed to get OpenCL platform IDs.");
  }

  for (const auto & platform_id : platformIds)
  {
    cl_uint deviceCount = 0;
    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceCount);
    if (err != CL_SUCCESS || deviceCount == 0)
    {
      continue;
    }

    std::vector<cl_device_id> deviceIds(deviceCount);
    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, deviceCount, deviceIds.data(), nullptr);
    if (err != CL_SUCCESS)
    {
      continue;
    }
    resources[platform_id] = deviceIds;
    device_counter += deviceCount;
  }

  if (resources.empty())
  {
    std::cerr << "Warning: Failed to find OpenCL compatible devices." << std::endl;
  }

  // Allocate resources: 1 context = 1 device
  // (Multi-device contexts are possible but currently unused;
  //  see https://github.com/clEsperanto/CLIc/pull/420 for context.)
  device_list_.reserve(device_counter);
  for (const auto & [platform_id, device_ids] : resources)
  {
    size_t device_index = 0;
    for (const auto & device_id : device_ids)
    {
      auto context = std::make_shared<OpenCLDevice::Context>(clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, nullptr));
      if (context == nullptr)
      {
        throw std::runtime_error("Error: Failed to create OpenCL context.");
      }

      auto command_queue = std::make_shared<OpenCLDevice::CommandQueue>(clCreateCommandQueue(context->get(), device_id, 0, nullptr));
      if (command_queue == nullptr)
      {
        throw std::runtime_error("Error: Failed to create OpenCL command queue.");
      }

      device_list_.emplace_back(std::make_shared<OpenCLDevice>(
        std::make_shared<OpenCLDevice::Resources>(platform_id, device_id, device_index++), context, command_queue));
    }
  }
}

// ── Device enumeration ──────────────────────────────────────────────────────

auto
OpenCLBackend::getDevices(const std::string & type) const -> std::vector<Device::Pointer>
{
  if (type != "gpu" && type != "cpu")
  {
    return device_list_;
  }

  std::vector<Device::Pointer> filtered;
  filtered.reserve(device_list_.size());
  std::copy_if(device_list_.begin(), device_list_.end(), std::back_inserter(filtered), [&type](const Device::Pointer & device) {
    return device->getDeviceType() == type;
  });
  return filtered;
}

auto
OpenCLBackend::getDevice(const std::string & name, const std::string & type) const -> Device::Pointer
{
  // If no devices at all, return nullptr
  if (device_list_.empty())
  {
    std::cerr << "Warning: Fail to find any OpenCL compatible devices." << std::endl;
    return nullptr;
  }

  // If no device of the specified type, fall back to last device from system
  const auto devices = getDevices(type);
  if (devices.empty())
  {
    return device_list_.back();
  }

  // If no specific name requested, return last device of the specified type
  if (name.empty())
  {
    return devices.back();
  }

  // Search for the device by name (case-insensitive)
  const auto lower_name = to_lower(name);
  const auto it = std::find_if(devices.begin(), devices.end(), [&lower_name](const Device::Pointer & dev) {
    return to_lower(dev->getName()).find(lower_name) != std::string::npos;
  });

  if (it != devices.end())
  {
    return *it;
  }

  // If no match found, return last device of the specified type
  return devices.back();
}

auto
OpenCLBackend::getDeviceFromIndex(size_t index, const std::string & type) const -> Device::Pointer
{
  if (device_list_.empty())
  {
    std::cerr << "Warning: Fail to find any OpenCL compatible devices." << std::endl;
    return nullptr;
  }

  const auto devices = getDevices(type);
  if (devices.empty())
  {
    return device_list_.back();
  }

  if (index < devices.size())
  {
    return devices[index];
  }

  return devices.back();
}

auto
OpenCLBackend::getDevicesList(const std::string & type) const -> std::vector<std::string>
{
  const auto               devices = getDevices(type);
  std::vector<std::string> deviceList;
  deviceList.reserve(devices.size());
  std::transform(
    devices.begin(), devices.end(), std::back_inserter(deviceList), [](const Device::Pointer & device) { return device->getName(); });
  return deviceList;
}

auto
OpenCLBackend::getType() const -> Backend::Type
{
  return Backend::Type::OPENCL;
}

// ── Memory allocation ───────────────────────────────────────────────────────

auto
OpenCLBackend::allocateMemory(const Device::Pointer &       device,
                              const std::array<size_t, 3> & region,
                              const dType &                 dtype,
                              const mType &                 mtype,
                              std::shared_ptr<void> &       data_ptr) const -> void
{
  switch (mtype)
  {
    case mType::BUFFER: {
      const size_t size = region[0] * region[1] * region[2] * toBytes(dtype);
      allocateBuffer(device, size, data_ptr);
      break;
    }
    case mType::IMAGE: {
      allocateImage(device, region, dtype, data_ptr);
      break;
    }
  }
}

auto
OpenCLBackend::allocateBuffer(const Device::Pointer & device, const size_t & size, std::shared_ptr<void> & data_ptr) -> void
{
  const auto opencl_device = castDevice(device);

  cl_int err;
  auto   mem = clCreateBuffer(opencl_device->getCLContext(), CL_MEM_READ_WRITE, size, nullptr, &err);
  CL_CHECK(err, "Error: Failed to allocate buffer memory.");

  data_ptr = wrapMemObject(mem);
}

auto
OpenCLBackend::allocateImage(const Device::Pointer &       device,
                             const std::array<size_t, 3> & region,
                             const dType &                 dtype,
                             std::shared_ptr<void> &       data_ptr) -> void
{
  const auto opencl_device = castDevice(device);

  // Configure image format
  cl_image_format image_format = {};
  image_format.image_channel_order = CL_R; // WARNING: see https://github.com/clEsperanto/CLIc/issues/317
  image_format.image_channel_data_type = toImageChannelType(dtype);

  // Configure image descriptor — type determined by dimensionality
  cl_image_desc image_desc = {};
  image_desc.image_width = region[0];
  image_desc.image_height = region[1];
  image_desc.image_depth = region[2];

  if (region[2] > 1)
  {
    image_desc.image_type = CL_MEM_OBJECT_IMAGE3D;
  }
  else if (region[1] > 1)
  {
    image_desc.image_type = CL_MEM_OBJECT_IMAGE2D;
  }
  else
  {
    image_desc.image_type = CL_MEM_OBJECT_IMAGE1D;
  }

  cl_int err;
  auto   image = clCreateImage(opencl_device->getCLContext(), CL_MEM_READ_WRITE, &image_format, &image_desc, nullptr, &err);
  CL_CHECK(err, "Error: Failed to allocate image memory.");

  data_ptr = wrapMemObject(image);
}

auto
OpenCLBackend::freeMemory(const Device::Pointer &, const mType &, const std::shared_ptr<void> & data_ptr) const -> void
{
  // Note: device and mtype parameters are unused; OpenCL handles cleanup
  // through the shared_ptr custom deleter. This explicit release decrements
  // the OpenCL reference count immediately.
  CL_CHECK(clReleaseMemObject(static_cast<cl_mem>(data_ptr.get())), "Error: Failed to free memory.");
}

auto
OpenCLBackend::getRefCount(void * data_ptr) const -> int
{
  cl_uint ref_count = 0;
  CL_CHECK(clGetMemObjectInfo(static_cast<cl_mem>(data_ptr), CL_MEM_REFERENCE_COUNT, sizeof(cl_uint), &ref_count, nullptr),
           "Error: Failed to get reference count.");
  return static_cast<int>(ref_count);
}

// ── Memory write ────────────────────────────────────────────────────────────

auto
OpenCLBackend::writeMemory(const Device::Pointer &       device,
                           const std::shared_ptr<void> & buffer_ptr,
                           std::array<size_t, 3> &       buffer_shape,
                           std::array<size_t, 3> &       buffer_origin,
                           std::array<size_t, 3> &       region,
                           const dType &                 dtype,
                           const mType &                 mtype,
                           const void *                  host_ptr) const -> void
{
  switch (mtype)
  {
    case mType::BUFFER: {
      const size_t elem_size = toBytes(dtype);
      buffer_shape[0] *= elem_size;
      buffer_origin[0] *= elem_size;
      region[0] *= elem_size;
      writeBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
      break;
    }
    case mType::IMAGE: {
      writeImage(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
      break;
    }
  }
}

auto
OpenCLBackend::writeBuffer(const Device::Pointer &       device,
                           const std::shared_ptr<void> & buffer_ptr,
                           const std::array<size_t, 3> & buffer_shape,
                           const std::array<size_t, 3> & buffer_origin,
                           const std::array<size_t, 3> & region,
                           const void *                  host_ptr) -> void
{
  const auto opencl_device = castDevice(device);
  const auto [row_pitch, slice_pitch] = computeBufferPitches(buffer_shape);

  constexpr cl_bool           blocking = CL_TRUE;
  const std::array<size_t, 3> host_origin = { 0, 0, 0 };
  auto                        ptr = static_cast<cl_mem>(buffer_ptr.get());
  cl_int                      err;

  if (buffer_shape[2] > 1 || buffer_shape[1] > 1)
  {
    err = clEnqueueWriteBufferRect(opencl_device->getCLCommandQueue(),
                                   ptr,
                                   blocking,
                                   buffer_origin.data(),
                                   host_origin.data(),
                                   region.data(),
                                   row_pitch,
                                   slice_pitch,
                                   0,
                                   0,
                                   host_ptr,
                                   0,
                                   nullptr,
                                   nullptr);
  }
  else
  {
    err =
      clEnqueueWriteBuffer(opencl_device->getCLCommandQueue(), ptr, blocking, buffer_origin[0], region[0], host_ptr, 0, nullptr, nullptr);
  }

  CL_CHECK(err, "Error: Failed to write buffer memory.");
}

auto
OpenCLBackend::writeImage(const Device::Pointer &       device,
                          const std::shared_ptr<void> & buffer_ptr,
                          const std::array<size_t, 3> & /*buffer_shape*/,
                          const std::array<size_t, 3> & buffer_origin,
                          const std::array<size_t, 3> & region,
                          const void *                  host_ptr) -> void
{
  const auto        opencl_device = castDevice(device);
  constexpr cl_bool blocking = CL_TRUE;
  auto              ptr = static_cast<cl_mem>(buffer_ptr.get());

  const auto err = clEnqueueWriteImage(
    opencl_device->getCLCommandQueue(), ptr, blocking, buffer_origin.data(), region.data(), 0, 0, host_ptr, 0, nullptr, nullptr);

  CL_CHECK(err, "Error: Failed to write image memory.");
}

// ── Memory read ─────────────────────────────────────────────────────────────

auto
OpenCLBackend::readMemory(const Device::Pointer &       device,
                          const std::shared_ptr<void> & buffer_ptr,
                          std::array<size_t, 3> &       buffer_shape,
                          std::array<size_t, 3> &       buffer_origin,
                          std::array<size_t, 3> &       region,
                          const dType &                 dtype,
                          const mType &                 mtype,
                          void *                        host_ptr) const -> void
{
  switch (mtype)
  {
    case mType::BUFFER: {
      const size_t elem_size = toBytes(dtype);
      buffer_shape[0] *= elem_size;
      buffer_origin[0] *= elem_size;
      region[0] *= elem_size;
      readBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
      break;
    }
    case mType::IMAGE: {
      readImage(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
      break;
    }
  }
}

auto
OpenCLBackend::readBuffer(const Device::Pointer &       device,
                          const std::shared_ptr<void> & buffer_ptr,
                          const std::array<size_t, 3> & buffer_shape,
                          const std::array<size_t, 3> & buffer_origin,
                          const std::array<size_t, 3> & region,
                          void *                        host_ptr) -> void
{
  const auto opencl_device = castDevice(device);
  const auto [row_pitch, slice_pitch] = computeBufferPitches(buffer_shape);

  constexpr cl_bool           blocking = CL_TRUE;
  const std::array<size_t, 3> host_origin = { 0, 0, 0 };
  auto                        ptr = static_cast<cl_mem>(buffer_ptr.get());
  cl_int                      err;

  if (buffer_shape[2] > 1 || buffer_shape[1] > 1)
  {
    err = clEnqueueReadBufferRect(opencl_device->getCLCommandQueue(),
                                  ptr,
                                  blocking,
                                  buffer_origin.data(),
                                  host_origin.data(),
                                  region.data(),
                                  row_pitch,
                                  slice_pitch,
                                  0,
                                  0,
                                  host_ptr,
                                  0,
                                  nullptr,
                                  nullptr);
  }
  else
  {
    err =
      clEnqueueReadBuffer(opencl_device->getCLCommandQueue(), ptr, blocking, buffer_origin[0], region[0], host_ptr, 0, nullptr, nullptr);
  }

  CL_CHECK(err, "Error: Failed to read buffer memory.");
}

auto
OpenCLBackend::readImage(const Device::Pointer &       device,
                         const std::shared_ptr<void> & buffer_ptr,
                         const std::array<size_t, 3> & /*buffer_shape*/,
                         const std::array<size_t, 3> & buffer_origin,
                         const std::array<size_t, 3> & region,
                         void *                        host_ptr) -> void
{
  const auto        opencl_device = castDevice(device);
  constexpr cl_bool blocking = CL_TRUE;
  auto              ptr = static_cast<cl_mem>(buffer_ptr.get());

  const auto err = clEnqueueReadImage(
    opencl_device->getCLCommandQueue(), ptr, blocking, buffer_origin.data(), region.data(), 0, 0, host_ptr, 0, nullptr, nullptr);

  CL_CHECK(err, "Error: Failed to read image memory.");
}

// ── Memory copy (device-to-device) ──────────────────────────────────────────

auto
OpenCLBackend::copyMemoryBufferToBuffer(const Device::Pointer &       device,
                                        const std::shared_ptr<void> & src_ptr,
                                        std::array<size_t, 3> &       src_origin,
                                        std::array<size_t, 3> &       src_shape,
                                        const std::shared_ptr<void> & dst_ptr,
                                        std::array<size_t, 3> &       dst_origin,
                                        std::array<size_t, 3> &       dst_shape,
                                        std::array<size_t, 3> &       region,
                                        const size_t &                bytes) const -> void
{
  const auto opencl_device = castDevice(device);

  // Adjust sizes and offsets for byte alignment
  region[0] *= bytes;
  src_origin[0] *= bytes;
  src_shape[0] *= bytes;
  dst_origin[0] *= bytes;
  dst_shape[0] *= bytes;

  const auto [src_row_pitch, src_slice_pitch] = computeBufferPitches(src_shape);
  const auto [dst_row_pitch, dst_slice_pitch] = computeBufferPitches(dst_shape);

  auto   in_ptr = static_cast<cl_mem>(src_ptr.get());
  auto   out_ptr = static_cast<cl_mem>(dst_ptr.get());
  cl_int err;

  if (dst_shape[2] > 1 || dst_shape[1] > 1 || src_shape[2] > 1 || src_shape[1] > 1)
  {
    err = clEnqueueCopyBufferRect(opencl_device->getCLCommandQueue(),
                                  in_ptr,
                                  out_ptr,
                                  src_origin.data(),
                                  dst_origin.data(),
                                  region.data(),
                                  src_row_pitch,
                                  src_slice_pitch,
                                  dst_row_pitch,
                                  dst_slice_pitch,
                                  0,
                                  nullptr,
                                  nullptr);
  }
  else
  {
    err = clEnqueueCopyBuffer(
      opencl_device->getCLCommandQueue(), in_ptr, out_ptr, src_origin[0], dst_origin[0], region[0], 0, nullptr, nullptr);
  }

  CL_CHECK(err, "Error: Failed to copy memory from buffer to buffer.");
}

auto
OpenCLBackend::copyMemoryBufferToImage(const Device::Pointer &       device,
                                       const std::shared_ptr<void> & src_ptr,
                                       std::array<size_t, 3> &       src_origin,
                                       std::array<size_t, 3> &       src_shape,
                                       const std::shared_ptr<void> & dst_ptr,
                                       std::array<size_t, 3> &       dst_origin,
                                       std::array<size_t, 3> & /*dst_shape*/,
                                       std::array<size_t, 3> & region,
                                       const size_t & /*bytes*/) const -> void
{
  const auto opencl_device = castDevice(device);

  // Note: Byte scaling is not needed when copying from buffer to image
  const auto [src_row_pitch, src_slice_pitch] = computeBufferPitches(src_shape);
  const size_t bufferOffset = src_origin[0] + src_origin[1] * src_row_pitch + src_origin[2] * src_slice_pitch;

  auto in_ptr = static_cast<cl_mem>(src_ptr.get());
  auto out_ptr = static_cast<cl_mem>(dst_ptr.get());

  const auto err = clEnqueueCopyBufferToImage(
    opencl_device->getCLCommandQueue(), in_ptr, out_ptr, bufferOffset, dst_origin.data(), region.data(), 0, nullptr, nullptr);

  CL_CHECK(err, "Error: Failed to copy memory from buffer to image.");
}

auto
OpenCLBackend::copyMemoryImageToBuffer(const Device::Pointer &       device,
                                       const std::shared_ptr<void> & src_ptr,
                                       std::array<size_t, 3> &       src_origin,
                                       std::array<size_t, 3> &       src_shape,
                                       const std::shared_ptr<void> & dst_ptr,
                                       std::array<size_t, 3> &       dst_origin,
                                       std::array<size_t, 3> &       dst_shape,
                                       std::array<size_t, 3> &       region,
                                       const size_t & /*bytes*/) const -> void
{
  const auto opencl_device = castDevice(device);

  // Note: Byte scaling is not applied when copying from image to buffer
  const auto [dst_row_pitch, dst_slice_pitch] = computeBufferPitches(dst_shape);
  const size_t bufferOffset = src_origin[0] + src_origin[1] * dst_row_pitch + src_origin[2] * dst_slice_pitch;

  auto in_ptr = static_cast<cl_mem>(src_ptr.get());
  auto out_ptr = static_cast<cl_mem>(dst_ptr.get());

  const auto err = clEnqueueCopyImageToBuffer(
    opencl_device->getCLCommandQueue(), in_ptr, out_ptr, src_origin.data(), region.data(), bufferOffset, 0, nullptr, nullptr);

  CL_CHECK(err, "Error: Failed to copy memory from image to buffer.");
}

auto
OpenCLBackend::copyMemoryImageToImage(const Device::Pointer &       device,
                                      const std::shared_ptr<void> & src_ptr,
                                      std::array<size_t, 3> &       src_origin,
                                      std::array<size_t, 3> & /*src_shape*/,
                                      const std::shared_ptr<void> & dst_ptr,
                                      std::array<size_t, 3> &       dst_origin,
                                      std::array<size_t, 3> & /*dst_shape*/,
                                      std::array<size_t, 3> & region,
                                      const size_t & /*bytes*/) const -> void
{
  const auto opencl_device = castDevice(device);

  auto in_ptr = static_cast<cl_mem>(src_ptr.get());
  auto out_ptr = static_cast<cl_mem>(dst_ptr.get());

  const auto err = clEnqueueCopyImage(
    opencl_device->getCLCommandQueue(), in_ptr, out_ptr, src_origin.data(), dst_origin.data(), region.data(), 0, nullptr, nullptr);

  CL_CHECK(err, "Error: Failed to copy memory from image to image.");
}

// ── Memory fill ─────────────────────────────────────────────────────────────

auto
OpenCLBackend::setMemory(const Device::Pointer &       device,
                         const std::shared_ptr<void> & buffer_ptr,
                         std::array<size_t, 3> &       buffer_shape,
                         std::array<size_t, 3> &       buffer_origin,
                         std::array<size_t, 3> &       region,
                         const dType &                 dtype,
                         const mType &                 mtype,
                         const float &                 value) const -> void
{
  switch (mtype)
  {
    case mType::BUFFER: {
      setBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, dtype, value);
      break;
    }
    case mType::IMAGE: {
      setImage(device, buffer_ptr, buffer_shape, buffer_origin, region, dtype, value);
      break;
    }
  }
}

auto
OpenCLBackend::setBuffer(const Device::Pointer &       device,
                         const std::shared_ptr<void> & buffer_ptr,
                         const std::array<size_t, 3> & /*buffer_shape*/,
                         const std::array<size_t, 3> & /*buffer_origin*/,
                         const std::array<size_t, 3> & region,
                         const dType &                 dtype,
                         const float &                 value) -> void
{
  const auto   opencl_device = castDevice(device);
  const size_t size = region[0] * region[1] * region[2] * toBytes(dtype);
  auto         ptr = static_cast<cl_mem>(buffer_ptr.get());
  const auto   queue = opencl_device->getCLCommandQueue();

  cl_int err;
  switch (dtype)
  {
    case dType::FLOAT:
      err = fillBufferTyped<float>(queue, ptr, size, value);
      break;
    case dType::INT32:
      err = fillBufferTyped<int32_t>(queue, ptr, size, value);
      break;
    case dType::UINT32:
      err = fillBufferTyped<uint32_t>(queue, ptr, size, value);
      break;
    case dType::INT16:
      err = fillBufferTyped<int16_t>(queue, ptr, size, value);
      break;
    case dType::UINT16:
      err = fillBufferTyped<uint16_t>(queue, ptr, size, value);
      break;
    case dType::INT8:
      err = fillBufferTyped<int8_t>(queue, ptr, size, value);
      break;
    case dType::UINT8:
      err = fillBufferTyped<uint8_t>(queue, ptr, size, value);
      break;
    default:
      throw std::invalid_argument("Error: Invalid data type for buffer fill operation");
  }

  CL_CHECK(err, "Error: Failed to fill buffer memory.");
}

auto
OpenCLBackend::setImage(const Device::Pointer &       device,
                        const std::shared_ptr<void> & buffer_ptr,
                        const std::array<size_t, 3> & /*buffer_shape*/,
                        const std::array<size_t, 3> & buffer_origin,
                        const std::array<size_t, 3> & region,
                        const dType &                 dtype,
                        const float &                 value) -> void
{
  const auto opencl_device = castDevice(device);
  auto       ptr = static_cast<cl_mem>(buffer_ptr.get());
  const auto queue = opencl_device->getCLCommandQueue();

  cl_int err;
  switch (dtype)
  {
    case dType::FLOAT:
      err = fillImageTyped<cl_float>(queue, ptr, buffer_origin, region, value);
      break;
    case dType::INT32:
    case dType::INT16:
    case dType::INT8:
      err = fillImageTyped<cl_int>(queue, ptr, buffer_origin, region, value);
      break;
    case dType::UINT32:
    case dType::UINT16:
    case dType::UINT8:
      err = fillImageTyped<cl_uint>(queue, ptr, buffer_origin, region, value);
      break;
    default:
      throw std::invalid_argument("Error: Invalid data type for image fill operation");
  }

  CL_CHECK(err, "Error: Failed to fill image memory.");
}

// ── Kernel build and execution ──────────────────────────────────────────────

auto
OpenCLBackend::buildKernel(const Device::Pointer & device,
                           const std::string &     kernel_source,
                           const std::string &     kernel_name,
                           std::shared_ptr<void> & kernel) const -> void
{
  const auto opencl_device = castDevice(device);
  auto &     disk_cache = DiskCache::instance();
  const auto source_hash = DiskCache::hash(kernel_source);
  const auto device_hash = DiskCache::hash(opencl_device->getInfo());

  // ── Level 1: in-memory cache (fastest) ──
  const auto cache_key = device_hash + "_" + source_hash;
  auto       program = device->getProgramFromCache(cache_key);

  // ── Level 2: disk cache (avoids recompilation) ──
  if (program == nullptr && disk_cache.isEnabled())
  {
    program = loadProgramFromCache(opencl_device, device_hash, source_hash);
  }

  // ── Level 3: compile from source ──
  if (program == nullptr)
  {
    program = createProgramFromSource(opencl_device, kernel_source);
    if (disk_cache.isEnabled())
    {
      saveBinaryToCache(device_hash, source_hash, program, device);
    }
  }

  device->addProgramToCache(cache_key, program);

  // Create kernel from program
  cl_int err;
  auto   ocl_kernel = clCreateKernel(reinterpret_cast<cl_program>(program.get()), kernel_name.c_str(), &err);
  CL_CHECK(err, "Error: Failed to create kernel '" + kernel_name + "'.");

  kernel = wrapKernel(ocl_kernel);
}

auto
OpenCLBackend::executeKernel(const Device::Pointer &                    device,
                             const std::string &                        kernel_source,
                             const std::string &                        kernel_name,
                             const std::array<size_t, 3> &              global_size,
                             const std::array<size_t, 3> &              local_size,
                             const std::vector<std::shared_ptr<void>> & args,
                             const std::vector<size_t> &                sizes) const -> void
{
  const auto opencl_device = castDevice(device);

  // Build or retrieve the kernel
  std::shared_ptr<void> ocl_kernel;
  buildKernel(device, kernel_source, kernel_name, ocl_kernel);

  const auto cl_kern = reinterpret_cast<cl_kernel>(ocl_kernel.get());

  // Set kernel arguments
  for (size_t i = 0; i < args.size(); ++i)
  {
    void * arg_ptr = args[i].get();

    // Check if the argument is a cl_mem (device memory object)
    if (sizes[i] == sizeof(cl_mem))
    {
      arg_ptr = (void *)&args[i];
    }

    const cl_int err = clSetKernelArg(cl_kern, static_cast<cl_uint>(i), sizes[i], arg_ptr);
    if (err != CL_SUCCESS)
    {
      throw std::runtime_error("Error: Failed to set kernel argument " + std::to_string(i) + " for kernel '" + kernel_name +
                               "'. OpenCL error: " + getErrorString(err) + " (" + std::to_string(err) + ")");
    }
  }

  // Launch kernel
  const cl_int err = clEnqueueNDRangeKernel(opencl_device->getCLCommandQueue(),
                                            cl_kern,
                                            3,
                                            nullptr,
                                            global_size.data(),
                                            (local_size[0] > 0) ? local_size.data() : nullptr,
                                            0,
                                            nullptr,
                                            nullptr);
  if (err != CL_SUCCESS)
  {
    std::string error_msg =
      "Error: Failed to launch kernel '" + kernel_name + "'. OpenCL error: " + getErrorString(err) + " (" + std::to_string(err) + ")";
    error_msg += "\nGlobal work size: [" + std::to_string(global_size[0]) + ", " + std::to_string(global_size[1]) + ", " +
                 std::to_string(global_size[2]) + "]";
    if (local_size[0] > 0)
    {
      error_msg += "\nLocal work size: [" + std::to_string(local_size[0]) + ", " + std::to_string(local_size[1]) + ", " +
                   std::to_string(local_size[2]) + "]";
    }
    throw std::runtime_error(error_msg);
  }

  // Wait for kernel to complete
  opencl_device->finish();
}

auto
OpenCLBackend::getPreamble() const -> std::string
{
  return kernel::preamble_cl;
}

#endif // USE_OPENCL

} // namespace cle
