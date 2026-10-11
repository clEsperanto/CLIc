#pragma once

// Internal helpers shared by the backend implementations (OpenCL, CUDA, Metal).
// This header is private to the library: it is not installed and not part of the public API.

#include "backend.hpp"
#include "cache.hpp"
#include "device.hpp"
#include "utils.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace cle::detail
{

// ============================================================================
// Device selection
// ============================================================================

/**
 * @brief Cast a device to the backend-specific device type
 * @param type_name name of the device type, used in the error message
 * @throw std::invalid_argument if the device is null, std::runtime_error if it belongs to another backend
 */
template <typename DeviceType>
[[nodiscard]] auto
castDevice(const Device::Pointer & device, const char * type_name) -> std::shared_ptr<const DeviceType>
{
  if (device == nullptr)
  {
    throw std::invalid_argument("Error: Device is null");
  }
  auto typed_device = std::dynamic_pointer_cast<const DeviceType>(device);
  if (typed_device == nullptr)
  {
    throw std::runtime_error(std::string("Error: Failed to cast device to ") + type_name +
                             " (device belongs to another backend: " + device->getName() + ")");
  }
  return typed_device;
}

/**
 * @brief Find the first device whose name contains `name` (case-insensitive)
 * @return the matching device, or nullptr if none matches
 */
[[nodiscard]] inline auto
findDeviceByName(const std::vector<Device::Pointer> & devices, const std::string & name) -> Device::Pointer
{
  const auto lower_name = to_lower(name);
  const auto it = std::find_if(devices.begin(), devices.end(), [&lower_name](const Device::Pointer & device) {
    return to_lower(device->getName()).find(lower_name) != std::string::npos;
  });
  return (it != devices.end()) ? *it : nullptr;
}

/**
 * @brief List the names of the devices
 */
[[nodiscard]] inline auto
deviceNames(const std::vector<Device::Pointer> & devices) -> std::vector<std::string>
{
  std::vector<std::string> names;
  names.reserve(devices.size());
  std::transform(
    devices.begin(), devices.end(), std::back_inserter(names), [](const Device::Pointer & device) { return device->getName(); });
  return names;
}

// ============================================================================
// Memory region helpers
// ============================================================================

/**
 * @brief Convert the x component of a shape, origin and region from elements to bytes
 */
inline auto
scaleToBytes(std::array<size_t, 3> & shape, std::array<size_t, 3> & origin, std::array<size_t, 3> & region, size_t elem_size) -> void
{
  shape[0] *= elem_size;
  origin[0] *= elem_size;
  region[0] *= elem_size;
}

/**
 * @brief Call `func` with a value-initialised object of the C++ type matching `dtype`
 *        (e.g. `visitDType(dType::UINT8, [](auto tag) { using T = decltype(tag); ... })`)
 * @throw std::invalid_argument for types without a scalar equivalent (COMPLEX, UNKNOWN)
 */
template <typename Func>
auto
visitDType(const dType & dtype, Func && func) -> decltype(func(float{}))
{
  switch (dtype)
  {
    case dType::FLOAT:
      return func(float{});
    case dType::INT32:
      return func(int32_t{});
    case dType::UINT32:
      return func(uint32_t{});
    case dType::INT16:
      return func(int16_t{});
    case dType::UINT16:
      return func(uint16_t{});
    case dType::INT8:
      return func(int8_t{});
    case dType::UINT8:
      return func(uint8_t{});
    default:
      throw std::invalid_argument("Error: Unsupported data type '" + toString(dtype) + "'");
  }
}

// ============================================================================
// Kernel launch configuration
// ============================================================================

/**
 * @brief Compute a work-group (block / threadgroup) size for the given global size.
 *
 * Strategy:
 *   - Determine the active dimensions (global_size[i] > 1).
 *   - 1D: up to 256 threads along the active dimension.
 *   - 2D: 32 x 8 (wider along the first active dimension for coalescing).
 *   - 3D: 32 x 8 x 1 (z goes to the grid).
 *   - z is capped at 64 and the total is halved until it fits within `max_threads`.
 */
[[nodiscard]] inline auto
computeBlockSize(const std::array<size_t, 3> & global_size, size_t max_threads = 1024) -> std::array<size_t, 3>
{
  const auto dim = std::count_if(global_size.begin(), global_size.end(), [](size_t size) { return size > 1; });

  std::array<size_t, 3> block = { 1, 1, 1 };
  switch (dim)
  {
    case 1: {
      constexpr size_t kMaxThreads1D = 256;
      for (size_t i = 0; i < 3; ++i)
      {
        if (global_size[i] > 1)
        {
          block[i] = std::min(global_size[i], kMaxThreads1D);
        }
      }
      break;
    }
    case 2: {
      constexpr size_t kBlockDimFirst2D = 32;
      constexpr size_t kBlockDimOther2D = 8;
      bool             first_active = true;
      for (size_t i = 0; i < 3; ++i)
      {
        if (global_size[i] > 1)
        {
          block[i] = std::min(global_size[i], first_active ? kBlockDimFirst2D : kBlockDimOther2D);
          first_active = false;
        }
      }
      break;
    }
    case 3: {
      constexpr std::array<size_t, 3> kBlockDim3D = { 32, 8, 1 };
      for (size_t i = 0; i < 3; ++i)
      {
        block[i] = std::min(global_size[i], kBlockDim3D[i]);
      }
      break;
    }
    default:
      break;
  }

  block[2] = std::min(block[2], size_t{ 64 });

  max_threads = std::max(max_threads, size_t{ 1 });
  while (block[0] * block[1] * block[2] > max_threads)
  {
    auto max_it = std::max_element(block.begin(), block.end());
    *max_it = (*max_it + 1) / 2;
  }
  return block;
}

/**
 * @brief Compute the grid size from global size and block size (ceiling division)
 */
[[nodiscard]] inline auto
computeGridSize(const std::array<size_t, 3> & global_size, const std::array<size_t, 3> & block_size) -> std::array<size_t, 3>
{
  return { (global_size[0] + block_size[0] - 1) / block_size[0],
           (global_size[1] + block_size[1] - 1) / block_size[1],
           (global_size[2] + block_size[2] - 1) / block_size[2] };
}

// ============================================================================
// Kernel source helpers
// ============================================================================

/**
 * @brief Translate an OpenCL kernel source whose preamble is already written in the target language.
 *
 * The source passed to the backends is `defines + preamble + opencl_kernel`. Only the parts around the
 * preamble are given to `translate_defines` and `translate_kernel`, so the already-valid preamble is
 * kept untouched. If the preamble is not found, the whole source is translated with `translate_kernel`.
 */
template <typename TranslateDefines, typename TranslateKernel>
[[nodiscard]] auto
translateAroundPreamble(const std::string & source,
                        const std::string & preamble,
                        TranslateDefines && translate_defines,
                        TranslateKernel &&  translate_kernel) -> std::string
{
  const auto preamble_pos = preamble.empty() ? std::string::npos : source.find(preamble);
  if (preamble_pos == std::string::npos)
  {
    return translate_kernel(source);
  }
  return translate_defines(source.substr(0, preamble_pos)) + preamble + translate_kernel(source.substr(preamble_pos + preamble.size()));
}

// ============================================================================
// Kernel cache
// ============================================================================

/**
 * @brief Get a compiled kernel from the 3-level cache, building it if needed.
 *
 *   1. in-memory cache of the device (key: the caller's cache_key, or std::hash + length of the source; kernel name)
 *   2. disk cache (key: device hash + source hash), loaded with `from_disk`
 *   3. compilation from source with `from_source`, whose cacheable artefact is saved to disk
 *
 * @param cache_key   identifier of kernel_source for the in-memory cache (see Backend::buildKernel); empty: the
 *                    source is hashed
 * @param from_disk   `(const std::string & artefact) -> std::shared_ptr<void>`, builds the kernel from a
 *                    disk-cached artefact. Returning nullptr or throwing discards the entry (e.g. corrupted
 *                    file, driver update) and falls back to compiling from source.
 * @param from_source `() -> std::pair<std::shared_ptr<void>, std::string>`, compiles the kernel and returns
 *                    it with the artefact to store on disk (empty string: nothing is stored).
 * @return the backend-specific kernel object, never nullptr
 */
template <typename FromDisk, typename FromSource>
[[nodiscard]] auto
cachedBuild(const Device::Pointer & device,
            const std::string &     kernel_source,
            const std::string &     kernel_name,
            const std::string &     extension,
            const std::string &     cache_key,
            FromDisk &&             from_disk,
            FromSource &&           from_source) -> std::shared_ptr<void>
{
  // ── Level 1: in-memory cache ──
  // Keyed with the caller's cache_key when provided (no per-launch cost), else with the fast std::hash of the source
  // (process-local). The stable disk-cache hash is only computed on a miss.
  const auto memory_key =
    (cache_key.empty() ? "H" + std::to_string(std::hash<std::string>{}(kernel_source)) + "-" + std::to_string(kernel_source.size())
                       : "K" + cache_key) +
    "_" + kernel_name;
  if (auto cached = device->getProgramFromCache(memory_key))
  {
    return cached;
  }

  const auto & device_hash = deviceHash(*device);
  const auto   source_hash = DiskCache::hash(kernel_source);

  auto &                disk_cache = DiskCache::instance();
  const bool            use_disk = disk_cache.isEnabled();
  std::shared_ptr<void> kernel = nullptr;

  // ── Level 2: disk cache ──
  if (use_disk)
  {
    const auto artefact = disk_cache.loadBinary(device_hash, source_hash, extension);
    if (!artefact.empty())
    {
      try
      {
        kernel = from_disk(std::string(artefact.begin(), artefact.end()));
      }
      catch (const std::exception & e)
      {
        std::cerr << "Warning: Ignoring unusable cached kernel '" << kernel_name << "', rebuilding it. " << e.what() << std::endl;
        kernel = nullptr;
      }
    }
  }

  // ── Level 3: build from source ──
  if (kernel == nullptr)
  {
    auto built = from_source();
    kernel = std::move(built.first);
    if (use_disk && !built.second.empty())
    {
      try
      {
        disk_cache.saveBinary(device_hash, source_hash, extension, built.second.data(), built.second.size());
      }
      catch (const std::exception & e)
      {
        // a read-only or full cache location must not prevent the kernel from running
        std::cerr << "Warning: Failed to save kernel '" << kernel_name << "' to the disk cache. " << e.what() << std::endl;
      }
    }
  }

  if (kernel == nullptr)
  {
    throw std::runtime_error("Error: Failed to build kernel '" + kernel_name + "'");
  }
  device->addProgramToCache(memory_key, kernel);
  return kernel;
}

} // namespace cle::detail
