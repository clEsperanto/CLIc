#pragma once

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#include "device.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#  define NOMINMAX
#  include <ShlObj.h>
#  include <windows.h>
#endif

namespace cle
{

constexpr std::string_view CACHE_FOLDER = "clesperanto";
constexpr std::string_view CACHE_DIR_WIN = "AppData\\Local";
constexpr std::string_view CACHE_DIR_UNIX = ".cache";

/**
 * @brief Get the path to the cache folder
 * @param base_path the home directory of the user
 * @return the path to the cache folder
 */
static auto
get_path_with_cache_folder(const std::filesystem::path & base_path) -> std::filesystem::path
{
  return base_path / std::filesystem::u8path(CACHE_FOLDER);
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

/**
 * @brief Get the path to the cache folder
 * @return the path to the cache folder
 */
static auto
get_cache_directory_path() -> std::filesystem::path
{
  TCHAR path[MAX_PATH];
  if (FAILED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path)))
  {
    std::cerr << "Failed to get " << CACHE_DIR_WIN << " directory\n";
    return get_path_with_cache_folder(std::filesystem::current_path());
  }
  return get_path_with_cache_folder(std::filesystem::u8path(path));
}
#else

/**
 * @brief Get the path to the cache folder
 * @return the path to the cache folder
 */
static auto
get_cache_directory_path() -> std::filesystem::path
{
  if (char * home_dir = std::getenv("HOME"); home_dir != nullptr)
  {
    return get_path_with_cache_folder(std::filesystem::u8path(home_dir) / std::filesystem::u8path(CACHE_DIR_UNIX));
  }
  std::cerr << "Failed to get user home directory\n";
  return get_path_with_cache_folder(std::filesystem::current_path() / std::filesystem::path(CACHE_DIR_UNIX));
}
#endif

/**
 * @brief Check if the cache is enabled
 *        The cache can be disabled by setting the environment variable CLESPERANTO_NO_CACHE
 * @return true if the cache is enabled, false otherwise
 */
static auto
is_cache_enabled() -> bool
{
  return std::getenv("CLESPERANTO_NO_CACHE") == nullptr;
}

/**
 * @brief Control the cache
 */
static auto
use_cache(bool flag) -> void
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
  if (flag)
  {
    // unset the environment variable CLESPERANTO_NO_CACHE
    _putenv("CLESPERANTO_NO_CACHE=");
  }
  else
  {
    // set the environment variable CLESPERANTO_NO_CACHE to 1
    _putenv("CLESPERANTO_NO_CACHE=1");
  }
#else
  if (flag)
  {
    // unset the environment variable CLESPERANTO_NO_CACHE
    unsetenv("CLESPERANTO_NO_CACHE");
  }
  else
  {
    // set the environment variable CLESPERANTO_NO_CACHE to 1
    setenv("CLESPERANTO_NO_CACHE", "1", 1);
  }
#endif
}


static const auto CACHE_FOLDER_PATH = get_cache_directory_path();

} // namespace cle

