#include "cache.hpp"

#include <fstream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#  define NOMINMAX
#  include <ShlObj.h>
#  include <windows.h>
#endif

namespace cle
{

// =============================================================================
// ProgramCache implementation
// =============================================================================

ProgramCache::ProgramCache()
{
  cache_.reserve(MAX_CACHE_SIZE);
}

auto
ProgramCache::put(const std::string & key, const std::shared_ptr<void> & program) -> void
{
  auto it = cache_.find(key);
  if (it != cache_.end())
  {
    // Key already exists: update program and move to most-recently-used
    lru_.erase(it->second.lru_it);
    lru_.push_back(key);
    it->second.lru_it = std::prev(lru_.end());
    it->second.program = program;
    return;
  }
  if (cache_.size() >= MAX_CACHE_SIZE)
  {
    // Evict least recently used entry
    auto oldest = lru_.front();
    lru_.pop_front();
    cache_.erase(oldest);
  }
  lru_.push_back(key);
  cache_[key] = { program, std::prev(lru_.end()) };
}

auto
ProgramCache::get(const std::string & key) -> std::shared_ptr<void>
{
  auto it = cache_.find(key);
  if (it != cache_.end())
  {
    // Move to most-recently-used
    lru_.erase(it->second.lru_it);
    lru_.push_back(key);
    it->second.lru_it = std::prev(lru_.end());
    return it->second.program;
  }
  return nullptr;
}

auto
ProgramCache::contains(const std::string & key) const -> bool
{
  return cache_.find(key) != cache_.end();
}

auto
ProgramCache::size() const -> size_t
{
  return cache_.size();
}

auto
ProgramCache::clear() -> void
{
  cache_.clear();
  lru_.clear();
}

// =============================================================================
// DiskCache implementation
// =============================================================================

static constexpr std::string_view CACHE_FOLDER_NAME = "clesperanto";
static constexpr std::string_view CACHE_DIR_UNIX = ".cache";

auto
DiskCache::resolveCacheDirectory() -> std::filesystem::path
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
  TCHAR path[MAX_PATH];
  if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path)))
  {
    return std::filesystem::u8path(path) / std::filesystem::u8path(CACHE_FOLDER_NAME);
  }
  std::cerr << "Failed to get AppData\\Local directory\n";
  return std::filesystem::current_path() / std::filesystem::u8path(CACHE_FOLDER_NAME);
#else
  if (const char * home_dir = std::getenv("HOME"); home_dir != nullptr)
  {
    return std::filesystem::u8path(home_dir) / std::filesystem::u8path(CACHE_DIR_UNIX) / std::filesystem::u8path(CACHE_FOLDER_NAME);
  }
  std::cerr << "Failed to get user home directory\n";
  return std::filesystem::current_path() / std::filesystem::u8path(CACHE_DIR_UNIX) / std::filesystem::u8path(CACHE_FOLDER_NAME);
#endif
}

DiskCache::DiskCache()
  : cacheDir_(resolveCacheDirectory())
{}

auto
DiskCache::instance() -> DiskCache &
{
  static DiskCache inst;
  return inst;
}

auto
DiskCache::isEnabled() const -> bool
{
  return std::getenv("CLESPERANTO_NO_CACHE") == nullptr;
}

auto
DiskCache::setEnabled(bool flag) -> void
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
  if (flag)
  {
    _putenv("CLESPERANTO_NO_CACHE=");
  }
  else
  {
    _putenv("CLESPERANTO_NO_CACHE=1");
  }
#else
  if (flag)
  {
    unsetenv("CLESPERANTO_NO_CACHE");
  }
  else
  {
    setenv("CLESPERANTO_NO_CACHE", "1", 1);
  }
#endif
}

auto
DiskCache::getCacheDirectory() const -> const std::filesystem::path &
{
  return cacheDir_;
}

auto
DiskCache::getFilePath(const std::string & device_hash, const std::string & source_hash, const std::string & extension) const
  -> std::filesystem::path
{
  return cacheDir_ / std::filesystem::path(device_hash) / std::filesystem::path(source_hash + "." + extension);
}

auto
DiskCache::saveBinary(const std::string & device_hash,
                      const std::string & source_hash,
                      const std::string & extension,
                      const void *        data,
                      size_t              size) -> void
{
  auto binary_path = getFilePath(device_hash, source_hash, extension);
  std::filesystem::create_directories(binary_path.parent_path());
  std::ofstream outfile(binary_path, std::ios::binary);
  if (!outfile)
  {
    throw std::runtime_error("Error: Failed to open cache file for writing: " + binary_path.string());
  }
  outfile.write(reinterpret_cast<const char *>(data), size);
  if (!outfile.good())
  {
    throw std::runtime_error("Error: Failed to write cache file: " + binary_path.string());
  }
}

auto
DiskCache::loadBinary(const std::string & device_hash, const std::string & source_hash, const std::string & extension) const
  -> std::vector<unsigned char>
{
  auto binary_path = getFilePath(device_hash, source_hash, extension);
  if (!std::filesystem::exists(binary_path))
  {
    return {};
  }
  std::ifstream infile(binary_path, std::ios::binary | std::ios::ate);
  if (!infile.is_open())
  {
    std::cerr << "Error: Failed to open cache file: " << binary_path << std::endl;
    return {};
  }
  auto file_size = static_cast<size_t>(infile.tellg());
  if (file_size == 0)
  {
    std::cerr << "Error: Cache file is empty: " << binary_path << std::endl;
    return {};
  }
  infile.seekg(0, std::ios::beg);
  std::vector<unsigned char> data(file_size);
  if (!infile.read(reinterpret_cast<char *>(data.data()), file_size))
  {
    std::cerr << "Error: Failed to read cache file: " << binary_path << std::endl;
    return {};
  }
  return data;
}

auto
DiskCache::exists(const std::string & device_hash, const std::string & source_hash, const std::string & extension) const -> bool
{
  return std::filesystem::exists(getFilePath(device_hash, source_hash, extension));
}

auto
DiskCache::hash(const std::string & input) -> std::string
{
  return std::to_string(std::hash<std::string>{}(input));
}

} // namespace cle
