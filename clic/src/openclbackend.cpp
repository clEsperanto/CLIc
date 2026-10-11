#include "backend.hpp"
#include "backend_common.hpp"
#include "cle_preamble_cl.h"

#include <memory>

namespace cle
{

#if USE_OPENCL

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
 */
[[nodiscard]] static auto
castDevice(const Device::Pointer & device) -> std::shared_ptr<const OpenCLDevice>
{
  return detail::castDevice<OpenCLDevice>(device, "OpenCLDevice");
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
// Buffer <-> image copy helper
// ============================================================================

/// A block of rows of a buffer <-> image copy: image origin and region (in pixels).
struct RectRows
{
  std::array<size_t, 3> image_origin;
  std::array<size_t, 3> region;
};

/**
 * @brief Split a buffer <-> image copy into blocks that are contiguous in the buffer.
 *
 * clEnqueueCopyBufferToImage / clEnqueueCopyImageToBuffer read or write the buffer side as a packed
 * block starting at a byte offset: they have no row or slice pitch. A region is contiguous in the
 * buffer when it spans whole rows (and whole slices if 3D); otherwise it is copied row by row.
 *
 * @param buffer_shape  shape of the buffer, in elements
 * @param buffer_origin origin of the region in the buffer, in elements
 * @param image_origin  origin of the region in the image, in pixels
 * @param region        size of the region, in elements / pixels
 * @param bytes         size of one element, in bytes
 * @param func          `(size_t buffer_offset_in_bytes, const RectRows & rows)` called for each block
 */
template <typename Func>
static auto
forEachBufferRow(const std::array<size_t, 3> & buffer_shape,
                 const std::array<size_t, 3> & buffer_origin,
                 const std::array<size_t, 3> & image_origin,
                 const std::array<size_t, 3> & region,
                 size_t                        bytes,
                 Func &&                       func) -> void
{
  const size_t row_pitch = buffer_shape[0];
  const size_t slice_pitch = buffer_shape[0] * buffer_shape[1];
  const auto   offset_of = [&](size_t y, size_t z) {
    return (buffer_origin[0] + (buffer_origin[1] + y) * row_pitch + (buffer_origin[2] + z) * slice_pitch) * bytes;
  };

  // A flat shape { N, 1, 1 } describes a packed buffer (e.g. whole-array copies): the region is contiguous
  const bool flat = buffer_shape[1] == 1 && buffer_shape[2] == 1;
  const bool whole_rows = region[0] == buffer_shape[0];
  const bool whole_slices = whole_rows && region[1] == buffer_shape[1];
  if (flat || whole_slices || (whole_rows && region[2] == 1))
  {
    func(offset_of(0, 0), RectRows{ image_origin, region });
    return;
  }
  for (size_t z = 0; z < region[2]; ++z)
  {
    for (size_t y = 0; y < region[1]; ++y)
    {
      func(offset_of(y, z), RectRows{ { image_origin[0], image_origin[1] + y, image_origin[2] + z }, { region[0], 1, 1 } });
    }
  }
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
 * @brief Extract the compiled binary of a program, to be stored in the disk cache.
 *
 * Returns the binary of the first device in the program's device list
 * (1 context = 1 device as per current architecture).
 */
[[nodiscard]] static auto
getProgramBinary(const std::shared_ptr<void> & program, const Device::Pointer & device) -> std::string
{
  // Note: only 1 device per context
  // https://github.com/clEsperanto/CLIc/pull/420/commits/e246f6d065b503b61a70b423db25738d577dda0b
  constexpr size_t device_index = 0;
  const size_t     nb_devices = std::max<size_t>(device->getNbDevicesFromContext(), 1);
  const auto       cl_prog = reinterpret_cast<cl_program>(program.get());

  std::vector<size_t> bin_sizes(nb_devices, 0);
  CL_CHECK(clGetProgramInfo(cl_prog, CL_PROGRAM_BINARY_SIZES, sizeof(size_t) * nb_devices, bin_sizes.data(), nullptr),
           "Error: Failed to query program binary sizes.");

  std::vector<std::string> binaries(nb_devices);
  std::vector<char *>      bin_ptrs(nb_devices);
  for (size_t i = 0; i < nb_devices; ++i)
  {
    binaries[i].resize(bin_sizes[i]);
    bin_ptrs[i] = binaries[i].data();
  }
  CL_CHECK(clGetProgramInfo(cl_prog, CL_PROGRAM_BINARIES, sizeof(char *) * nb_devices, bin_ptrs.data(), nullptr),
           "Error: Failed to fetch program binary.");
  return std::move(binaries[device_index]);
}

/**
 * @brief Create and build an OpenCL program from a cached binary.
 * @throw std::runtime_error if the binary is rejected (e.g. after a driver update)
 */
[[nodiscard]] static auto
createProgramFromBinary(const std::shared_ptr<const OpenCLDevice> & opencl_device, const std::string & binary) -> std::shared_ptr<void>
{
  const size_t binary_size = binary.size();
  const auto * binary_ptr = reinterpret_cast<const unsigned char *>(binary.data());
  cl_int       status = CL_SUCCESS;
  cl_int       err = CL_SUCCESS;
  auto         program =
    clCreateProgramWithBinary(opencl_device->getCLContext(), 1, &opencl_device->getCLDevice(), &binary_size, &binary_ptr, &status, &err);
  CL_CHECK(err, "Error: Failed to create program from cached binary.");
  auto program_ptr = wrapProgram(program);
  CL_CHECK(status, "Error: Cached binary rejected by the device.");
  buildProgram(opencl_device, program_ptr);
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

/**
 * @brief Create a kernel object from a built program.
 *        The kernel retains its program, so the program wrapper can be released afterwards.
 */
[[nodiscard]] static auto
createKernel(const std::shared_ptr<void> & program, const std::string & kernel_name) -> std::shared_ptr<void>
{
  cl_int err;
  auto   ocl_kernel = clCreateKernel(reinterpret_cast<cl_program>(program.get()), kernel_name.c_str(), &err);
  CL_CHECK(err, "Error: Failed to create kernel '" + kernel_name + "'.");
  return wrapKernel(ocl_kernel);
}

// ============================================================================
// OpenCLBackend Implementation
// ============================================================================

// ── Resource initialisation ─────────────────────────────────────────────────

/// Create one device (own context and queue) per OpenCL device of every platform
static auto
discoverDevices() -> std::vector<Device::Pointer>
{
  // Discover all platforms and their devices, in the order reported by the ICD loader
  // (a vector rather than a hash map, so that device order and default device are deterministic)
  std::vector<std::pair<cl_platform_id, std::vector<cl_device_id>>> resources;
  size_t                                                            device_counter = 0;

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
    resources.emplace_back(platform_id, std::move(deviceIds));
    device_counter += deviceCount;
  }

  if (resources.empty())
  {
    std::cerr << "Warning: Failed to find OpenCL compatible devices." << std::endl;
  }

  // Allocate resources: 1 context = 1 device
  // (Multi-device contexts are possible but currently unused;
  //  see https://github.com/clEsperanto/CLIc/pull/420 for context.)
  // A device whose context or queue cannot be created (e.g. broken driver) is skipped, so that it does
  // not prevent the other devices from being used.
  std::vector<Device::Pointer> devices;
  devices.reserve(device_counter);
  for (const auto & [platform_id, device_ids] : resources)
  {
    size_t device_index = 0;
    for (const auto & device_id : device_ids)
    {
      auto resources_ptr = std::make_shared<OpenCLDevice::Resources>(platform_id, device_id, device_index++);

      cl_int     context_err = CL_SUCCESS;
      cl_context context_ptr = clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, &context_err);
      if (context_err != CL_SUCCESS || context_ptr == nullptr)
      {
        std::cerr << "Warning: Skipping OpenCL device '" << resources_ptr->device_name << "': failed to create context ("
                  << getErrorString(context_err) << ")." << std::endl;
        continue;
      }
      auto context = std::make_shared<OpenCLDevice::Context>(context_ptr);

      cl_int           queue_err = CL_SUCCESS;
      cl_command_queue queue_ptr = clCreateCommandQueue(context->get(), device_id, 0, &queue_err);
      if (queue_err != CL_SUCCESS || queue_ptr == nullptr)
      {
        std::cerr << "Warning: Skipping OpenCL device '" << resources_ptr->device_name << "': failed to create command queue ("
                  << getErrorString(queue_err) << ")." << std::endl;
        continue;
      }
      auto command_queue = std::make_shared<OpenCLDevice::CommandQueue>(queue_ptr);

      devices.emplace_back(std::make_shared<OpenCLDevice>(resources_ptr, context, command_queue));
    }
  }
  return devices;
}

OpenCLBackend::OpenCLBackend()
  : device_list_(discoverDevices())
{}

OpenCLBackend::~OpenCLBackend() = default;

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

  // Search for the device by name (case-insensitive), else return last device of the specified type
  auto device = detail::findDeviceByName(devices, name);
  return (device != nullptr) ? device : devices.back();
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
  return detail::deviceNames(getDevices(type));
}

auto
OpenCLBackend::getType() const -> Backend::Type
{
  return Backend::Type::OPENCL;
}

// ── Memory allocation ───────────────────────────────────────────────────────

static auto
allocateBuffer(const Device::Pointer & device, const size_t & size, std::shared_ptr<void> & data_ptr) -> void
{
  const auto opencl_device = castDevice(device);

  cl_int err;
  auto   mem = clCreateBuffer(opencl_device->getCLContext(), CL_MEM_READ_WRITE, size, nullptr, &err);
  CL_CHECK(err, "Error: Failed to allocate buffer memory.");

  data_ptr = wrapMemObject(mem);
}

static auto
allocateImage(const Device::Pointer & device, const std::array<size_t, 3> & region, const dType & dtype, std::shared_ptr<void> & data_ptr)
  -> void
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
OpenCLBackend::freeMemory(const Device::Pointer &, const mType &, const std::shared_ptr<void> &) const -> void
{
  // Memory is released by the custom deleter of the shared_ptr returned by allocateMemory (see
  // wrapMemObject) when its last reference goes away. Releasing it here as well would free it twice.
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

static auto
writeBuffer(const Device::Pointer &       device,
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

static auto
writeImage(const Device::Pointer &       device,
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
      detail::scaleToBytes(buffer_shape, buffer_origin, region, toBytes(dtype));
      writeBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
      break;
    }
    case mType::IMAGE: {
      writeImage(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
      break;
    }
  }
}


// ── Memory read ─────────────────────────────────────────────────────────────

static auto
readBuffer(const Device::Pointer &       device,
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

static auto
readImage(const Device::Pointer &       device,
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
      detail::scaleToBytes(buffer_shape, buffer_origin, region, toBytes(dtype));
      readBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
      break;
    }
    case mType::IMAGE: {
      readImage(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
      break;
    }
  }
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
  detail::scaleToBytes(src_shape, src_origin, region, bytes);
  dst_shape[0] *= bytes;
  dst_origin[0] *= bytes;

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
                                       const size_t &          bytes) const -> void
{
  const auto opencl_device = castDevice(device);
  auto       in_ptr = static_cast<cl_mem>(src_ptr.get());
  auto       out_ptr = static_cast<cl_mem>(dst_ptr.get());

  forEachBufferRow(src_shape, src_origin, dst_origin, region, bytes, [&](size_t buffer_offset, const RectRows & rows) {
    CL_CHECK(clEnqueueCopyBufferToImage(opencl_device->getCLCommandQueue(),
                                        in_ptr,
                                        out_ptr,
                                        buffer_offset,
                                        rows.image_origin.data(),
                                        rows.region.data(),
                                        0,
                                        nullptr,
                                        nullptr),
             "Error: Failed to copy memory from buffer to image.");
  });
}

auto
OpenCLBackend::copyMemoryImageToBuffer(const Device::Pointer &       device,
                                       const std::shared_ptr<void> & src_ptr,
                                       std::array<size_t, 3> &       src_origin,
                                       std::array<size_t, 3> & /*src_shape*/,
                                       const std::shared_ptr<void> & dst_ptr,
                                       std::array<size_t, 3> &       dst_origin,
                                       std::array<size_t, 3> &       dst_shape,
                                       std::array<size_t, 3> &       region,
                                       const size_t &                bytes) const -> void
{
  const auto opencl_device = castDevice(device);
  auto       in_ptr = static_cast<cl_mem>(src_ptr.get());
  auto       out_ptr = static_cast<cl_mem>(dst_ptr.get());

  forEachBufferRow(dst_shape, dst_origin, src_origin, region, bytes, [&](size_t buffer_offset, const RectRows & rows) {
    CL_CHECK(clEnqueueCopyImageToBuffer(opencl_device->getCLCommandQueue(),
                                        in_ptr,
                                        out_ptr,
                                        rows.image_origin.data(),
                                        rows.region.data(),
                                        buffer_offset,
                                        0,
                                        nullptr,
                                        nullptr),
             "Error: Failed to copy memory from image to buffer.");
  });
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

static auto
setBuffer(const Device::Pointer &       device,
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

  const cl_int err = detail::visitDType(dtype, [&](auto tag) { return fillBufferTyped<decltype(tag)>(queue, ptr, size, value); });
  CL_CHECK(err, "Error: Failed to fill buffer memory.");
}

static auto
setImage(const Device::Pointer &       device,
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


// ── Kernel build and execution ──────────────────────────────────────────────


auto
OpenCLBackend::syncToStream(const Device::Pointer & device, int64_t /*consumer_stream*/) const -> void
{
  CL_CHECK(clFinish(castDevice(device)->getCLCommandQueue()), "Error: Failed to synchronise the command queue.");
}

auto
OpenCLBackend::buildKernel(const Device::Pointer & device,
                           const std::string &     kernel_source,
                           const std::string &     kernel_name,
                           std::shared_ptr<void> & kernel,
                           const std::string &     cache_key) const -> void
{
  const auto opencl_device = castDevice(device);

  // The kernel object (which retains its program) is cached in memory; the program binary on disk.
  kernel = detail::cachedBuild(
    device,
    kernel_source,
    kernel_name,
    "bin",
    cache_key,
    [&](const std::string & binary) { return createKernel(createProgramFromBinary(opencl_device, binary), kernel_name); },
    [&]() {
      auto program = createProgramFromSource(opencl_device, kernel_source);
      auto ocl_kernel = createKernel(program, kernel_name);
      return std::make_pair(ocl_kernel, getProgramBinary(program, device));
    });
}

auto
OpenCLBackend::executeKernel(const Device::Pointer &                    device,
                             const std::string &                        kernel_source,
                             const std::string &                        kernel_name,
                             const std::array<size_t, 3> &              global_size,
                             const std::array<size_t, 3> &              local_size,
                             const std::vector<std::shared_ptr<void>> & args,
                             const std::vector<size_t> &                sizes,
                             const std::string &                        cache_key) const -> void
{
  const auto opencl_device = castDevice(device);

  // Build or retrieve the kernel
  std::shared_ptr<void> ocl_kernel;
  buildKernel(device, kernel_source, kernel_name, ocl_kernel, cache_key);

  const auto cl_kern = reinterpret_cast<cl_kernel>(ocl_kernel.get());

  // Set kernel arguments. Memory objects are passed by the address of their handle, scalars by the
  // address of their value (both are what args[i].get() points to, resp. is).
  std::vector<cl_mem> mem_handles(args.size(), nullptr);
  for (size_t i = 0; i < args.size(); ++i)
  {
    const void * arg_ptr = args[i].get();
    if (sizes[i] == sizeof(cl_mem))
    {
      mem_handles[i] = static_cast<cl_mem>(args[i].get());
      arg_ptr = &mem_handles[i];
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
