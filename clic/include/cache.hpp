#pragma once

#include <cstdlib>
#include <filesystem>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>


namespace cle
{

/**
 * @brief In-memory LRU cache for compiled programs
 *
 * Stores compiled program objects (as shared_ptr<void>) keyed by a string hash.
 * Uses a least-recently-used eviction policy with a configurable maximum size.
 * Each device should own its own ProgramCache instance since compiled programs
 * are device-specific.
 */
class ProgramCache
{
public:
  static constexpr size_t MAX_CACHE_SIZE = 128;

  ProgramCache();
  ~ProgramCache() = default;

  // Non-copyable, movable
  ProgramCache(const ProgramCache &) = delete;
  ProgramCache &
  operator=(const ProgramCache &) = delete;
  ProgramCache(ProgramCache &&) = default;
  ProgramCache &
  operator=(ProgramCache &&) = default;

  /**
   * @brief Cache a compiled program with a given key
   * @param key The key to identify the program (typically device_hash + source_hash)
   * @param program The compiled program pointer to cache
   */
  auto
  put(const std::string & key, const std::shared_ptr<void> & program) -> void;

  /**
   * @brief Retrieve a cached program by key, updating its LRU position
   * @param key The key to identify the program
   * @return The program pointer if found, nullptr otherwise
   */
  auto
  get(const std::string & key) -> std::shared_ptr<void>;

  /**
   * @brief Check if a key exists in the cache
   * @param key The key to check
   * @return true if the key exists, false otherwise
   */
  [[nodiscard]] auto
  contains(const std::string & key) const -> bool;

  /**
   * @brief Get the number of cached programs
   * @return The number of entries in the cache
   */
  [[nodiscard]] auto
  size() const -> size_t;

  /**
   * @brief Clear all cached programs
   */
  auto
  clear() -> void;

private:
  struct Entry
  {
    std::shared_ptr<void>            program;
    std::list<std::string>::iterator lru_it;
  };

  std::unordered_map<std::string, Entry> cache_;
  std::list<std::string>                 lru_;
};

/**
 * @brief Disk cache manager for compiled kernel binaries
 *
 * Manages saving and loading compiled kernel binaries (OpenCL .bin, CUDA .ptx, etc.)
 * to/from disk. The cache directory is organized as:
 *   <cache_root>/<device_hash>/<source_hash>.<extension>
 *
 * The disk cache can be disabled via the CLESPERANTO_NO_CACHE environment variable
 * or programmatically via setEnabled().
 *
 * Cache directory locations:
 *   - Unix/macOS: $HOME/.cache/clesperanto/
 *   - Windows:    %LOCALAPPDATA%\clesperanto\
 *   - Fallback:   $(pwd)/.cache/clesperanto/
 */
class DiskCache
{
public:
  /**
   * @brief Get the singleton DiskCache instance
   * @return Reference to the DiskCache singleton
   */
  static auto
  instance() -> DiskCache &;

  /**
   * @brief Check if the disk cache is enabled
   * @return true if caching is enabled, false otherwise
   */
  [[nodiscard]] auto
  isEnabled() const -> bool;

  /**
   * @brief Enable or disable the disk cache
   * @param flag true to enable, false to disable
   */
  auto
  setEnabled(bool flag) -> void;

  /**
   * @brief Get the root cache directory path
   * @return The cache directory path
   */
  [[nodiscard]] auto
  getCacheDirectory() const -> const std::filesystem::path &;

  /**
   * @brief Build the full file path for a cached binary
   * @param device_hash Hash string identifying the device
   * @param source_hash Hash string identifying the kernel source
   * @param extension File extension (e.g., "bin", "ptx")
   * @return The full path to the cache file
   */
  [[nodiscard]] auto
  getFilePath(const std::string & device_hash, const std::string & source_hash, const std::string & extension) const
    -> std::filesystem::path;

  /**
   * @brief Save binary data to the disk cache
   * @param device_hash Hash string identifying the device
   * @param source_hash Hash string identifying the kernel source
   * @param extension File extension (e.g., "bin", "ptx")
   * @param data Pointer to binary data
   * @param size Size of the binary data in bytes
   */
  auto
  saveBinary(const std::string & device_hash,
             const std::string & source_hash,
             const std::string & extension,
             const void *        data,
             size_t              size) -> void;

  /**
   * @brief Load binary data from the disk cache
   * @param device_hash Hash string identifying the device
   * @param source_hash Hash string identifying the kernel source
   * @param extension File extension (e.g., "bin", "ptx")
   * @return The binary data as a vector of bytes, or empty if not found
   */
  [[nodiscard]] auto
  loadBinary(const std::string & device_hash, const std::string & source_hash, const std::string & extension) const
    -> std::vector<unsigned char>;

  /**
   * @brief Check if a cached binary exists on disk
   * @param device_hash Hash string identifying the device
   * @param source_hash Hash string identifying the kernel source
   * @param extension File extension (e.g., "bin", "ptx")
   * @return true if the file exists, false otherwise
   */
  [[nodiscard]] auto
  exists(const std::string & device_hash, const std::string & source_hash, const std::string & extension) const -> bool;

  /**
   * @brief Compute a hash string for the given input
   * @param input The string to hash
   * @return The hash as a string
   */
  [[nodiscard]] static auto
  hash(const std::string & input) -> std::string;

private:
  DiskCache();
  ~DiskCache() = default;
  DiskCache(const DiskCache &) = delete;
  DiskCache &
  operator=(const DiskCache &) = delete;

  static auto
  resolveCacheDirectory() -> std::filesystem::path;

  std::filesystem::path cacheDir_;
};

/**
 * @brief Convenience function to enable/disable the disk cache
 * @param flag true to enable, false to disable
 */
inline auto
use_cache(bool flag) -> void
{
  DiskCache::instance().setEnabled(flag);
}

/**
 * @brief Convenience function to check if the disk cache is enabled
 * @return true if caching is enabled, false otherwise
 */
inline auto
is_cache_enabled() -> bool
{
  return DiskCache::instance().isEnabled();
}

} // namespace cle

