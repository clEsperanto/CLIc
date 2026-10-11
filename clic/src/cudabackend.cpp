#include "backend.hpp"
#include "backend_common.hpp"
#include "cle_preamble_cu.h"
#include "translator.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <limits>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace cle
{

// ============================================================================
// Compile-time CUDA guard
// ============================================================================

#if USE_CUDA
/**
 * @brief Convert a CUresult error code to a human-readable string.
 *        Returns "Unknown CUDA error" if the driver provides no string.
 */
[[nodiscard]] static auto
getErrorString(CUresult error) -> std::string
{
  const char * str = nullptr;
  cuGetErrorString(error, &str);
  return (str != nullptr) ? std::string(str) : "Unknown CUDA error";
}

/**
 * @brief Convert an nvrtcResult error code to a human-readable string.
 */
[[nodiscard]] static auto
getErrorString(nvrtcResult error) -> std::string
{
  return std::string(nvrtcGetErrorString(error));
}

/// Check a CUDA driver API call; throw with context on failure.
#  define CU_CHECK(call, msg)                                                                                                    \
    do                                                                                                                           \
    {                                                                                                                            \
      CUresult err_ = (call);                                                                                                    \
      if (err_ != CUDA_SUCCESS)                                                                                                  \
      {                                                                                                                          \
        throw std::runtime_error(std::string(msg) + " CUDA error: " + getErrorString(err_) + " (" + std::to_string(err_) + ")"); \
      }                                                                                                                          \
    } while (0)

/// Check an NVRTC call; throw with context on failure.
#  define NVRTC_CHECK(call, msg)                                                                                                  \
    do                                                                                                                            \
    {                                                                                                                             \
      nvrtcResult err_ = (call);                                                                                                  \
      if (err_ != NVRTC_SUCCESS)                                                                                                  \
      {                                                                                                                           \
        throw std::runtime_error(std::string(msg) + " NVRTC error: " + getErrorString(err_) + " (" + std::to_string(err_) + ")"); \
      }                                                                                                                           \
    } while (0)

// ============================================================================
// RAII Context Guard
// ============================================================================

/**
 * @brief RAII helper that sets the CUDA context for the given device on
 *        construction. Performs dynamic_pointer_cast and validates once.
 *
 * Eliminates the repeated 3-line boilerplate (cast + validate + set context)
 * from every method.
 */
class CUDAContextGuard
{
public:
  explicit CUDAContextGuard(const Device::Pointer & device)
    : cuda_device_(detail::castDevice<CUDADevice>(device, "CUDADevice"))
  {
    // The context is usually already current on this thread: skip the driver call in that case
    const CUcontext context = cuda_device_->getCUDAContext();
    CUcontext       current = nullptr;
    if (cuCtxGetCurrent(&current) != CUDA_SUCCESS || current != context)
    {
      CU_CHECK(cuCtxSetCurrent(context), "Error: Failed to set CUDA context");
    }
  }

  [[nodiscard]] auto
  device() const -> const std::shared_ptr<const CUDADevice> &
  {
    return cuda_device_;
  }

  [[nodiscard]] auto
  context() const -> CUcontext
  {
    return cuda_device_->getCUDAContext();
  }

  [[nodiscard]] auto
  stream() const -> CUstream
  {
    return cuda_device_->getCUDAStream();
  }

private:
  std::shared_ptr<const CUDADevice> cuda_device_;
};

// ============================================================================
// Memcpy Helpers (unified 1D/2D/3D logic)
// ============================================================================

/// Direction of a memcpy operation.
enum class MemcpyDirection
{
  HostToDevice,
  DeviceToHost,
  DeviceToDevice
};

/**
 * @brief Row pitch (bytes) and slice height (rows) of a memory of the given shape (x in bytes).
 *
 * A flat {N, 1, 1} shape is a packed buffer (e.g. the buffer side of a whole buffer <-> image copy, or host
 * memory): its rows and slices follow the copied region.
 */
[[nodiscard]] static auto
memoryPitches(const std::array<size_t, 3> & shape, const std::array<size_t, 3> & region) -> std::pair<size_t, size_t>
{
  if (shape[1] == 1 && shape[2] == 1)
  {
    return { region[0], region[1] };
  }
  return { shape[0], shape[1] };
}

/**
 * @brief Perform a 1D, 2D, or 3D memory copy between host and/or device.
 *
 * Selects cuMemcpyAsync / cuMemcpy2DAsync / cuMemcpy3DAsync based on the region and origins, eliminating
 * the triplicated branching logic that was in writeBuffer/readBuffer/copyBuffer.
 *
 * The copy is queued on `stream`, after the work already submitted to it, and may still be running
 * on return: callers copying to or from host memory must synchronise the stream before returning.
 * With `blocking`, the synchronous cuMemcpy / cuMemcpy2D / cuMemcpy3D are used instead (cheaper than queuing
 * then synchronising when the device waits after each operation anyway, see CUDADevice::getWaitToFinish).
 *
 * @param src_type        Source memory type (host or device).
 * @param src_ptr         Source pointer (host void* or CUdeviceptr cast).
 * @param src_origin      Byte offset origin for the source.
 * @param src_pitches     Row pitch in bytes and slice height in rows of the source (see memoryPitches).
 * @param dst_type        Destination memory type.
 * @param dst_ptr         Destination pointer.
 * @param dst_origin      Byte offset origin for the destination.
 * @param dst_pitches     Row pitch in bytes and slice height in rows of the destination.
 * @param region          Size of the region to copy {widthBytes, height, depth}.
 * @param stream          Stream the copy is queued on.
 * @param blocking        Use the synchronous copy functions.
 */
static auto
performMemcpy(CUmemorytype                      src_type,
              const void *                      src_ptr,
              const std::array<size_t, 3> &     src_origin,
              const std::pair<size_t, size_t> & src_pitches,
              CUmemorytype                      dst_type,
              const void *                      dst_ptr,
              const std::array<size_t, 3> &     dst_origin,
              const std::pair<size_t, size_t> & dst_pitches,
              const std::array<size_t, 3> &     region,
              CUstream                          stream,
              bool                              blocking) -> void
{
  CUresult err;

  const auto [src_row_pitch, src_slice_pitch] = src_pitches;
  const auto [dst_row_pitch, dst_slice_pitch] = dst_pitches;

  // Determine the copy dimensionality from the region AND the origins: a small source pasted at a
  // non-zero z/y offset into a larger destination still needs a 3D/2D copy to land at the correct address.
  const bool need_3d = (region[2] > 1 || src_origin[2] > 0 || dst_origin[2] > 0);
  const bool need_2d = !need_3d && (region[1] > 1 || src_origin[1] > 0 || dst_origin[1] > 0);

  if (need_3d)
  {
    // ── 3D copy ──
    CUDA_MEMCPY3D params = {};

    params.srcMemoryType = src_type;
    if (src_type == CU_MEMORYTYPE_HOST)
    {
      params.srcHost = src_ptr;
    }
    else
    {
      params.srcDevice = reinterpret_cast<CUdeviceptr>(src_ptr);
    }
    params.srcXInBytes = src_origin[0];
    params.srcY = src_origin[1];
    params.srcZ = src_origin[2];
    params.srcPitch = src_row_pitch;
    params.srcHeight = src_slice_pitch;

    params.dstMemoryType = dst_type;
    if (dst_type == CU_MEMORYTYPE_HOST)
    {
      params.dstHost = const_cast<void *>(dst_ptr);
    }
    else
    {
      params.dstDevice = reinterpret_cast<CUdeviceptr>(dst_ptr);
    }
    params.dstXInBytes = dst_origin[0];
    params.dstY = dst_origin[1];
    params.dstZ = dst_origin[2];
    params.dstPitch = dst_row_pitch;
    params.dstHeight = dst_slice_pitch;

    params.WidthInBytes = region[0];
    params.Height = region[1];
    params.Depth = region[2];

    err = blocking ? cuMemcpy3D(&params) : cuMemcpy3DAsync(&params, stream);
  }
  else if (need_2d)
  {
    // ── 2D copy ──
    CUDA_MEMCPY2D params = {};

    params.srcMemoryType = src_type;
    if (src_type == CU_MEMORYTYPE_HOST)
    {
      params.srcHost = src_ptr;
    }
    else
    {
      params.srcDevice = reinterpret_cast<CUdeviceptr>(src_ptr);
    }
    params.srcXInBytes = src_origin[0];
    params.srcY = src_origin[1];
    params.srcPitch = src_row_pitch;

    params.dstMemoryType = dst_type;
    if (dst_type == CU_MEMORYTYPE_HOST)
    {
      params.dstHost = const_cast<void *>(dst_ptr);
    }
    else
    {
      params.dstDevice = reinterpret_cast<CUdeviceptr>(dst_ptr);
    }
    params.dstXInBytes = dst_origin[0];
    params.dstY = dst_origin[1];
    params.dstPitch = dst_row_pitch;

    params.WidthInBytes = region[0];
    params.Height = region[1];

    err = blocking ? cuMemcpy2D(&params) : cuMemcpy2DAsync(&params, stream);
  }
  else
  {
    // ── 1D copy ──
    auto src_addr = reinterpret_cast<CUdeviceptr>(reinterpret_cast<const char *>(src_ptr) + src_origin[0]);
    auto dst_addr = reinterpret_cast<CUdeviceptr>(reinterpret_cast<const char *>(dst_ptr) + dst_origin[0]);

    err = blocking ? cuMemcpy(dst_addr, src_addr, region[0]) : cuMemcpyAsync(dst_addr, src_addr, region[0], stream);
  }

  CU_CHECK(err, "Error: Memory copy failed");
}

// ============================================================================
// Bit-cast helper (safe type-punning without UB)
// ============================================================================

/**
 * @brief Safely reinterpret the bits of one type as another of the same size.
 *        Equivalent to std::bit_cast (C++20) but works in C++17.
 */
template <typename To, typename From>
static auto
bitCast(const From & src) -> To
{
  static_assert(sizeof(To) == sizeof(From), "bit_cast: size mismatch");
  To dst;
  std::memcpy(&dst, &src, sizeof(To));
  return dst;
}

// ============================================================================
// Kernel compilation helper
// ============================================================================

/**
 * @brief Compile an NVRTC program with the given architecture option.
 *
 * @param to_cubin true: return the CUBIN (`--gpu-architecture=sm_XY`), false: return the PTX (`compute_XY`)
 * @return the compiled module image, or an empty string if NVRTC rejects the architecture option (e.g. a GPU
 *         newer than the NVRTC library). On a compilation error, throws with the build log.
 */
[[nodiscard]] static auto
compileWithNvrtc(const std::string & kernel_source, const std::string & arch_opt, bool to_cubin) -> std::string
{
  nvrtcProgram prog;
  NVRTC_CHECK(nvrtcCreateProgram(&prog, kernel_source.c_str(), nullptr, 0, nullptr, nullptr), "Error: Failed to create NVRTC program");

  // RAII cleanup for the program handle
  struct ProgramGuard
  {
    nvrtcProgram * p;
    ~ProgramGuard() { nvrtcDestroyProgram(p); }
  } guard{ &prog };

  const std::string                 warn_opt = "--disable-warnings";
  const std::array<const char *, 2> options = { arch_opt.c_str(), warn_opt.c_str() };

  nvrtcResult res = nvrtcCompileProgram(prog, static_cast<int>(options.size()), options.data());
  if (res == NVRTC_ERROR_INVALID_OPTION)
  {
    return {};
  }
  if (res != NVRTC_SUCCESS)
  {
    // Retrieve and report the build log
    size_t log_size = 0;
    nvrtcGetProgramLogSize(prog, &log_size);
    std::string log(log_size, '\0');
    nvrtcGetProgramLog(prog, log.data());
    throw std::runtime_error("Error: Failed to compile kernel. NVRTC error: " + getErrorString(res) + " (" + std::to_string(res) +
                             ")\nBuild log:\n" + log);
  }

  size_t      size = 0;
  std::string image;
  if (to_cubin)
  {
    NVRTC_CHECK(nvrtcGetCUBINSize(prog, &size), "Error: Failed to get CUBIN size");
    image.resize(size);
    NVRTC_CHECK(nvrtcGetCUBIN(prog, image.data()), "Error: Failed to get CUBIN");
  }
  else
  {
    // NVRTC includes the null terminator in the PTX size
    NVRTC_CHECK(nvrtcGetPTXSize(prog, &size), "Error: Failed to get PTX size");
    image.resize(size);
    NVRTC_CHECK(nvrtcGetPTX(prog, image.data()), "Error: Failed to get PTX");
  }
  return image;
}

/**
 * @brief Compile a CUDA kernel source for the device architecture `arch` (e.g. "61").
 *
 * Compiles to a CUBIN for `sm_<arch>`, which the driver loads as is. PTX (`compute_<arch>`) is only produced when this
 * NVRTC does not know the architecture; the driver then JIT-compiles it at every module load.
 *
 * @return the module image (CUBIN or PTX), accepted by cuModuleLoadData
 */
[[nodiscard]] static auto
compileKernel(const std::string & kernel_source, const std::string & arch) -> std::string
{
  auto image = compileWithNvrtc(kernel_source, "--gpu-architecture=sm_" + arch, true);
  if (image.empty())
  {
    image = compileWithNvrtc(kernel_source, "--gpu-architecture=compute_" + arch, false);
  }
  if (image.empty())
  {
    throw std::runtime_error("Error: Failed to compile kernel. NVRTC does not support the device architecture " + arch);
  }
  return image;
}

/**
 * @brief A loaded kernel, as held by the kernel caches.
 *
 * `function` must stay the first member: the object is handed out as a `std::shared_ptr<void>` that callers of
 * buildKernel read as a `CUfunction *`.
 */
struct LoadedFunction
{
  CUfunction function = nullptr;
  size_t     max_block_size = 0; ///< threads per block giving the best occupancy for this kernel (registers, shared memory)
};
static_assert(std::is_standard_layout_v<LoadedFunction>, "LoadedFunction must be pointer-interconvertible with its CUfunction");

/**
 * @brief Load a module image (CUBIN or PTX) and get the kernel function from it.
 *
 * The returned LoadedFunction holds the CUfunction and its occupancy-based block size limit (computed once here), and
 * unloads the module (which must stay alive while the function is in use) when the last reference goes away.
 */
[[nodiscard]] static auto
loadFunction(const std::string & image, const std::string & kernel_name) -> std::shared_ptr<void>
{
  CUmodule cuModule = nullptr;
  CU_CHECK(cuModuleLoadData(&cuModule, image.data()), "Error: Failed to load CUDA module");

  auto       loaded = std::make_unique<LoadedFunction>();
  const auto err = cuModuleGetFunction(&loaded->function, cuModule, kernel_name.c_str());
  if (err != CUDA_SUCCESS)
  {
    cuModuleUnload(cuModule);
    CU_CHECK(err, "Error: Failed to get function '" + kernel_name + "' from module");
  }

  // Largest block size reaching the best occupancy, given the kernel's register and static shared memory usage.
  // It is also never above the kernel's own limit (CU_FUNC_ATTRIBUTE_MAX_THREADS_PER_BLOCK), which can be lower than
  // the device limit for register-heavy kernels: launching more threads would fail.
  int min_grid_size = 0;
  int block_size = 0;
  if (cuOccupancyMaxPotentialBlockSize(&min_grid_size, &block_size, loaded->function, nullptr, 0, 0) == CUDA_SUCCESS && block_size > 0)
  {
    loaded->max_block_size = static_cast<size_t>(block_size);
  }

  return std::shared_ptr<void>(loaded.release(), [cuModule](void * p) {
    cuModuleUnload(cuModule);
    delete static_cast<LoadedFunction *>(p);
  });
}

// ============================================================================
// CUDABackend Implementation
// ============================================================================

CUDABackend::CUDABackend()
{
  CU_CHECK(cuInit(0), "Error: Failed to initialize CUDA");
}

// ── Device enumeration ──────────────────────────────────────────────────────

auto
CUDABackend::getDevices(const std::string & type) const -> std::vector<Device::Pointer>
{
  if (device_list_.empty())
  {
    int deviceCount = 0;
    CU_CHECK(cuDeviceGetCount(&deviceCount), "Error: Failed to get device count");
    device_list_.reserve(static_cast<size_t>(deviceCount));
    for (int i = 0; i < deviceCount; ++i)
    {
      try
      {
        device_list_.push_back(std::make_shared<CUDADevice>(i));
      }
      catch (const std::exception & e)
      {
        // a device that cannot be initialised must not prevent the others from being used
        std::cerr << "Warning: Skipping CUDA device " << i << ": " << e.what() << std::endl;
      }
    }
  }

  if (device_list_.empty())
  {
    std::cerr << "Warning: Failed to find '" << type << "' CUDA compatible devices." << std::endl;
  }

  // CUDA devices are all GPUs: "cpu" selects none, any other type (e.g. "gpu", "all") selects all (as OpenCL / Metal)
  if (type == "cpu")
  {
    return {};
  }
  return device_list_;
}

auto
CUDABackend::getDevice(const std::string & name, const std::string & type) const -> Device::Pointer
{
  if (getDevices("all").empty()) // also builds the device list
  {
    std::cerr << "Warning: No CUDA compatible devices found." << std::endl;
    return nullptr;
  }

  // No device of the requested type: fall back to any device (as OpenCL)
  auto devices = getDevices(type);
  if (devices.empty())
  {
    devices = device_list_;
  }
  if (auto device = detail::findDeviceByName(devices, name))
  {
    return device;
  }
  return devices.back();
}

auto
CUDABackend::getDeviceFromIndex(size_t index, const std::string & type) const -> Device::Pointer
{
  if (getDevices("all").empty()) // also builds the device list
  {
    std::cerr << "Warning: No CUDA compatible devices found." << std::endl;
    return nullptr;
  }

  // No device of the requested type: fall back to any device (as OpenCL)
  auto devices = getDevices(type);
  if (devices.empty())
  {
    devices = device_list_;
  }
  return (index < devices.size()) ? devices[index] : devices.back();
}

auto
CUDABackend::getDevicesList(const std::string & type) const -> std::vector<std::string>
{
  return detail::deviceNames(getDevices(type));
}

auto
CUDABackend::getType() const -> Backend::Type
{
  return Backend::Type::CUDA;
}

// ── Memory allocation ───────────────────────────────────────────────────────

auto
CUDABackend::allocateMemory(const Device::Pointer &       device,
                            const std::array<size_t, 3> & region,
                            const dType &                 dtype,
                            const mType &                 mtype,
                            std::shared_ptr<void> &       data_ptr) const -> void
{
  // @StRigaud TODO: implement true image support for CUDA
  // For now, both BUFFER and IMAGE allocate linear memory.
  const size_t size = region[0] * region[1] * region[2] * toBytes(dtype);
  allocateBuffer(device, size, data_ptr);
}

auto
CUDABackend::allocateBuffer(const Device::Pointer & device, const size_t & size, std::shared_ptr<void> & data_ptr) -> void
{
  CUDAContextGuard ctx(device);
  const auto       pool = ctx.device()->getCUDAMemoryPool();

  CUdeviceptr mem = 0;
  if (pool == nullptr)
  {
    CU_CHECK(cuMemAlloc(&mem, size), "Error: Failed to allocate buffer memory");
  }
  else
  {
    // Stream-ordered allocation: the memory can be used by the work queued on the device stream after this call
    // (kernels, copies, fills, VkFFT and host transfers all use that stream)
    CUresult err = cuMemAllocFromPoolAsync(&mem, size, pool, ctx.stream());
    if (err == CUDA_ERROR_OUT_OF_MEMORY)
    {
      // memory freed but not yet returned to the driver: wait for the pending frees, release the cache, retry
      cuStreamSynchronize(ctx.stream());
      cuMemPoolTrimTo(pool, 0);
      err = cuMemAllocFromPoolAsync(&mem, size, pool, ctx.stream());
    }
    CU_CHECK(err, "Error: Failed to allocate buffer memory");
  }

  data_ptr = std::shared_ptr<void>(reinterpret_cast<void *>(mem), [device, pooled = (pool != nullptr)](void * ptr) {
    if (ptr && device->isInitialized())
    {
      CUDAContextGuard guard(device);
      if (pooled)
      {
        // ordered after the work queued on the device stream. Memory exported over DLPack is only released here
        // once its consumer's work is done (see the DLPack deleter in Array::toDLPack)
        cuMemFreeAsync(reinterpret_cast<CUdeviceptr>(ptr), guard.stream());
      }
      else
      {
        cuMemFree(reinterpret_cast<CUdeviceptr>(ptr));
      }
    }
    // If device already finalized, skip — process exit will reclaim GPU memory
  });
}

auto
CUDABackend::freeMemory(const Device::Pointer &, const mType &, const std::shared_ptr<void> &) const -> void
{
  // Memory is released by the custom deleter of the shared_ptr returned by allocateMemory (see
  // allocateBuffer) when its last reference goes away. Releasing it here as well would free it twice.
}

auto
CUDABackend::getRefCount(void * data_ptr) const -> int
{
  // CUDA driver API does not provide reference counting for allocations.
  return 1;
}

// ── Memory read / write ─────────────────────────────────────────────────────

auto
CUDABackend::writeMemory(const Device::Pointer &       device,
                         const std::shared_ptr<void> & buffer_ptr,
                         std::array<size_t, 3> &       buffer_shape,
                         std::array<size_t, 3> &       buffer_origin,
                         std::array<size_t, 3> &       region,
                         const dType &                 dtype,
                         const mType &                 mtype,
                         const void *                  host_ptr) const -> void
{
  // Convert element counts to byte counts for the first dimension
  detail::scaleToBytes(buffer_shape, buffer_origin, region, toBytes(dtype));

  // @StRigaud TODO: differentiate IMAGE path when image support lands
  writeBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
}

auto
CUDABackend::writeBuffer(const Device::Pointer &       device,
                         const std::shared_ptr<void> & buffer_ptr,
                         const std::array<size_t, 3> & buffer_shape,
                         const std::array<size_t, 3> & buffer_origin,
                         const std::array<size_t, 3> & region,
                         const void *                  host_ptr) -> void
{
  CUDAContextGuard ctx(device);

  const std::array<size_t, 3> host_origin = { 0, 0, 0 };

  performMemcpy(CU_MEMORYTYPE_HOST,
                host_ptr,
                host_origin,
                memoryPitches(region, region), // packed host memory
                CU_MEMORYTYPE_DEVICE,
                buffer_ptr.get(),
                buffer_origin,
                memoryPitches(buffer_shape, region),
                region,
                ctx.stream(),
                false);
  // the caller may release or modify host_ptr as soon as we return
  CU_CHECK(cuStreamSynchronize(ctx.stream()), "Error: Failed to write buffer");
}

auto
CUDABackend::readMemory(const Device::Pointer &       device,
                        const std::shared_ptr<void> & buffer_ptr,
                        std::array<size_t, 3> &       buffer_shape,
                        std::array<size_t, 3> &       buffer_origin,
                        std::array<size_t, 3> &       region,
                        const dType &                 dtype,
                        const mType &                 mtype,
                        void *                        host_ptr) const -> void
{
  detail::scaleToBytes(buffer_shape, buffer_origin, region, toBytes(dtype));

  // @StRigaud TODO: differentiate IMAGE path when image support lands
  readBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, host_ptr);
}

auto
CUDABackend::readBuffer(const Device::Pointer &       device,
                        const std::shared_ptr<void> & buffer_ptr,
                        const std::array<size_t, 3> & buffer_shape,
                        const std::array<size_t, 3> & buffer_origin,
                        const std::array<size_t, 3> & region,
                        void *                        host_ptr) -> void
{
  CUDAContextGuard ctx(device);

  const std::array<size_t, 3> host_origin = { 0, 0, 0 };

  performMemcpy(CU_MEMORYTYPE_DEVICE,
                buffer_ptr.get(),
                buffer_origin,
                memoryPitches(buffer_shape, region),
                CU_MEMORYTYPE_HOST,
                host_ptr,
                host_origin,
                memoryPitches(region, region), // packed host memory
                region,
                ctx.stream(),
                false);
  // the caller reads host_ptr as soon as we return
  CU_CHECK(cuStreamSynchronize(ctx.stream()), "Error: Failed to read buffer");
}

// ── Memory copy (device-to-device) ──────────────────────────────────────────

auto
CUDABackend::copyMemoryBufferToBuffer(const Device::Pointer &       device,
                                      const std::shared_ptr<void> & src_ptr,
                                      std::array<size_t, 3> &       src_origin,
                                      std::array<size_t, 3> &       src_shape,
                                      const std::shared_ptr<void> & dst_ptr,
                                      std::array<size_t, 3> &       dst_origin,
                                      std::array<size_t, 3> &       dst_shape,
                                      std::array<size_t, 3> &       region,
                                      const size_t &                bytes) const -> void
{
  CUDAContextGuard ctx(device);

  // Convert element counts to byte counts
  detail::scaleToBytes(src_shape, src_origin, region, bytes);
  dst_shape[0] *= bytes;
  dst_origin[0] *= bytes;

  // a flat {N, 1, 1} shape is the packed buffer side of a whole buffer <-> image copy
  performMemcpy(CU_MEMORYTYPE_DEVICE,
                src_ptr.get(),
                src_origin,
                memoryPitches(src_shape, region),
                CU_MEMORYTYPE_DEVICE,
                dst_ptr.get(),
                dst_origin,
                memoryPitches(dst_shape, region),
                region,
                ctx.stream(),
                ctx.device()->getWaitToFinish()); // asynchronous unless the device waits after each operation
}

auto
CUDABackend::copyMemoryImageToBuffer(const Device::Pointer &       device,
                                     const std::shared_ptr<void> & src_ptr,
                                     std::array<size_t, 3> &       src_origin,
                                     std::array<size_t, 3> &       src_shape,
                                     const std::shared_ptr<void> & dst_ptr,
                                     std::array<size_t, 3> &       dst_origin,
                                     std::array<size_t, 3> &       dst_shape,
                                     std::array<size_t, 3> &       region,
                                     const size_t &                bytes) const -> void
{
  // @StRigaud TODO: implement proper image-to-buffer copy when image support lands
  copyMemoryBufferToBuffer(device, src_ptr, src_origin, src_shape, dst_ptr, dst_origin, dst_shape, region, bytes);
}

auto
CUDABackend::copyMemoryBufferToImage(const Device::Pointer &       device,
                                     const std::shared_ptr<void> & src_ptr,
                                     std::array<size_t, 3> &       src_origin,
                                     std::array<size_t, 3> &       src_shape,
                                     const std::shared_ptr<void> & dst_ptr,
                                     std::array<size_t, 3> &       dst_origin,
                                     std::array<size_t, 3> &       dst_shape,
                                     std::array<size_t, 3> &       region,
                                     const size_t &                bytes) const -> void
{
  // @StRigaud TODO: implement proper buffer-to-image copy when image support lands
  copyMemoryBufferToBuffer(device, src_ptr, src_origin, src_shape, dst_ptr, dst_origin, dst_shape, region, bytes);
}

auto
CUDABackend::copyMemoryImageToImage(const Device::Pointer &       device,
                                    const std::shared_ptr<void> & src_ptr,
                                    std::array<size_t, 3> &       src_origin,
                                    std::array<size_t, 3> &       src_shape,
                                    const std::shared_ptr<void> & dst_ptr,
                                    std::array<size_t, 3> &       dst_origin,
                                    std::array<size_t, 3> &       dst_shape,
                                    std::array<size_t, 3> &       region,
                                    const size_t &                bytes) const -> void
{
  // @StRigaud TODO: implement proper image-to-image copy when image support lands
  copyMemoryBufferToBuffer(device, src_ptr, src_origin, src_shape, dst_ptr, dst_origin, dst_shape, region, bytes);
}

// ── Memory fill ─────────────────────────────────────────────────────────────

auto
CUDABackend::setBuffer(const Device::Pointer &       device,
                       const std::shared_ptr<void> & buffer_ptr,
                       const std::array<size_t, 3> & buffer_shape,
                       const std::array<size_t, 3> & buffer_origin,
                       const std::array<size_t, 3> & region,
                       const dType &                 dtype,
                       const float &                 value) -> void
{
  CUDAContextGuard ctx(device);

  // asynchronous on the device stream, unless the device waits after each operation: the synchronous memset is
  // then cheaper than queuing and synchronising
  const bool        blocking = ctx.device()->getWaitToFinish();
  const CUstream    stream = ctx.stream();
  const auto        count = region[0] * region[1] * region[2];
  const CUdeviceptr dev_ptr = reinterpret_cast<CUdeviceptr>(buffer_ptr.get());
  const CUresult    err = detail::visitDType(dtype, [&](auto tag) -> CUresult {
    using T = decltype(tag);
    const auto cval = static_cast<T>(value);
    if constexpr (sizeof(T) == 4)
    {
      const auto bits = bitCast<uint32_t>(cval);
      return blocking ? cuMemsetD32(dev_ptr, bits, count) : cuMemsetD32Async(dev_ptr, bits, count, stream);
    }
    else if constexpr (sizeof(T) == 2)
    {
      const auto bits = bitCast<uint16_t>(cval);
      return blocking ? cuMemsetD16(dev_ptr, bits, count) : cuMemsetD16Async(dev_ptr, bits, count, stream);
    }
    else
    {
      const auto bits = bitCast<uint8_t>(cval);
      return blocking ? cuMemsetD8(dev_ptr, bits, count) : cuMemsetD8Async(dev_ptr, bits, count, stream);
    }
  });

  CU_CHECK(err, "Error: Failed to fill memory");
}

auto
CUDABackend::setMemory(const Device::Pointer &       device,
                       const std::shared_ptr<void> & buffer_ptr,
                       std::array<size_t, 3> &       buffer_shape,
                       std::array<size_t, 3> &       buffer_origin,
                       std::array<size_t, 3> &       region,
                       const dType &                 dtype,
                       const mType &                 mtype,
                       const float &                 value) const -> void
{
  // @StRigaud TODO: differentiate IMAGE path when image support lands
  setBuffer(device, buffer_ptr, buffer_shape, buffer_origin, region, dtype, value);
}

// ── Kernel build and execution ──────────────────────────────────────────────


auto
CUDABackend::syncToStream(const Device::Pointer & device, int64_t consumer_stream) const -> void
{
  CUDAContextGuard ctx(device);
  const CUstream   src_stream = ctx.stream();

  if (consumer_stream <= 0)
  {
    // null/legacy stream: full sync is safest
    CU_CHECK(cuStreamSynchronize(src_stream), "Error: Failed to synchronize the CUDA stream");
    return;
  }

  const auto dst_stream = reinterpret_cast<CUstream>(consumer_stream);

  // Mark when our work is done, and make the consumer stream (e.g. CuPy) wait for it before touching the memory.
  // The device's event is reused: a wait applies to the work recorded at the time cuStreamWaitEvent is called, so
  // recording the event again later (or from another thread) does not affect waits already queued.
  CUevent    event = ctx.device()->getCUDASyncEvent();
  const bool temporary_event = (event == nullptr);
  if (temporary_event)
  {
    CU_CHECK(cuEventCreate(&event, CU_EVENT_DISABLE_TIMING), "Error: Failed to create CUDA event");
  }
  CUresult err = cuEventRecord(event, src_stream);
  if (err == CUDA_SUCCESS)
  {
    err = cuStreamWaitEvent(dst_stream, event, 0);
  }
  if (temporary_event)
  {
    cuEventDestroy(event); // valid right after the wait, which still applies to the recorded work
  }
  CU_CHECK(err, "Error: Failed to synchronize with the consumer CUDA stream");
}

auto
CUDABackend::buildKernel(const Device::Pointer & device,
                         const std::string &     kernel_source,
                         const std::string &     kernel_name,
                         std::shared_ptr<void> & kernel,
                         const std::string &     cache_key) const -> void
{
  CUDAContextGuard ctx(device);
  const auto &     cuda_device = ctx.device();

  // The function (with its module) is cached in memory; the module image (CUBIN, or PTX if NVRTC does not know the
  // architecture) on disk, which avoids the OpenCL->CUDA translation, the NVRTC compilation and, for CUBIN, the
  // driver JIT compilation. The disk entries are per device, hence per architecture.
  auto function_holder = detail::cachedBuild(
    device,
    kernel_source,
    kernel_name,
    "cubin",
    cache_key,
    [&](const std::string & image) { return loadFunction(image, kernel_name); },
    [&]() {
      // The kernel_source string is: defines + preamble_cu + opencl_kernel.
      // preamble_cu is already valid CUDA — translate only the opencl_kernel part that follows it,
      // to avoid the translator corrupting the preamble. The defines block (before the preamble) is
      // plain #define macros valid in both OpenCL and CUDA, so it is kept as-is.
      const OpenCLToCUDATranslator translator;
      const auto                   cuda_source = detail::translateAroundPreamble(
        kernel_source,
        getPreamble(),
        [](const std::string & defines) { return defines; },
        [&](const std::string & code) { return translator.translate(code); });
      auto image = compileKernel(cuda_source, cuda_device->getArch());
      return std::make_pair(loadFunction(image, kernel_name), image);
    });

  // kernel either points to caller-provided CUfunction storage, or receives the cached holder
  if (kernel != nullptr)
  {
    *reinterpret_cast<CUfunction *>(kernel.get()) = *reinterpret_cast<CUfunction *>(function_holder.get());
  }
  else
  {
    kernel = function_holder;
  }
}

auto
CUDABackend::executeKernel(const Device::Pointer &                    device,
                           const std::string &                        kernel_source,
                           const std::string &                        kernel_name,
                           const std::array<size_t, 3> &              global_size,
                           const std::array<size_t, 3> &              local_size,
                           const std::vector<std::shared_ptr<void>> & args,
                           const std::vector<size_t> &                sizes,
                           const std::string &                        cache_key) const -> void
{
  CUDAContextGuard ctx(device);

  // ── Build / retrieve the kernel function (the holder keeps its module loaded during the launch) ──
  std::shared_ptr<void> function_holder;
  buildKernel(device, kernel_source, kernel_name, function_holder, cache_key);
  const auto &     loaded = *static_cast<const LoadedFunction *>(function_holder.get());
  const CUfunction cuFunction = loaded.function;

  // ── Prepare kernel arguments ──
  // cuLaunchKernel expects an array of pointers, each one pointing to an argument value. Device memory
  // is passed by the address of its handle, scalars by the address of their value (args[i].get()).
  std::vector<CUdeviceptr> mem_handles(args.size(), 0);
  std::vector<void *>      arg_ptrs(args.size(), nullptr);
  for (size_t i = 0; i < args.size(); ++i)
  {
    if (sizes[i] == sizeof(CUdeviceptr))
    {
      mem_handles[i] = reinterpret_cast<CUdeviceptr>(args[i].get());
      arg_ptrs[i] = &mem_handles[i];
    }
    else
    {
      arg_ptrs[i] = args[i].get();
    }
  }

  // The translator gives every kernel 3 trailing parameters: the requested global size (see addGlobalSizeParameters
  // in cudaTranslator.cpp). Extra arguments are ignored by kernels without them.
  std::array<int, 3> requested_size = {};
  for (size_t i = 0; i < 3; ++i)
  {
    if (global_size[i] > static_cast<size_t>(std::numeric_limits<int>::max()))
    {
      throw std::out_of_range("Error: Global size of kernel '" + kernel_name + "' too large for CUDA");
    }
    requested_size[i] = static_cast<int>(std::max<size_t>(global_size[i], 1));
    arg_ptrs.push_back(&requested_size[i]);
  }

  // ── Compute launch configuration ──
  // Use the caller-provided local_size if it's fully specified (non-zero in all dims),
  // otherwise fall back to the auto-computed block size: the usual shape (256 threads, see computeBlockSize), reduced
  // for kernels whose register / shared memory usage gives a better occupancy (or only allows) smaller blocks.
  const bool   use_local = (local_size[0] > 0 && local_size[1] > 0 && local_size[2] > 0);
  const size_t max_threads = (loaded.max_block_size > 0) ? loaded.max_block_size : 1024;
  const auto   block_size = use_local ? local_size : detail::computeBlockSize(global_size, max_threads);
  const auto   grid_size = detail::computeGridSize(global_size, block_size);

  // ── Launch ──
  CU_CHECK(cuLaunchKernel(cuFunction,
                          static_cast<unsigned>(grid_size[0]),
                          static_cast<unsigned>(grid_size[1]),
                          static_cast<unsigned>(grid_size[2]),
                          static_cast<unsigned>(block_size[0]),
                          static_cast<unsigned>(block_size[1]),
                          static_cast<unsigned>(block_size[2]),
                          0,               // shared memory bytes
                          ctx.stream(),    // stream
                          arg_ptrs.data(), // kernel arguments
                          nullptr),        // extra
           "Error: Failed to launch kernel '" + kernel_name + "'");

  ctx.device()->finish();
}

auto
CUDABackend::getPreamble() const -> std::string
{
  return kernel::preamble_cu;
}
#endif // USE_CUDA

} // namespace cle
