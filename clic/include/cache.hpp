#ifndef __INCLUDE_CACHE_HPP
#define __INCLUDE_CACHE_HPP

#include <cstdlib> // for std::getenv
#include <filesystem>
#include <iostream>
#include <string>

#include "device.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#  include <ShlObj.h>
#  include <windows.h>
#endif

namespace cle
{

// const constexpr char * HIDDEN_FOLDER = ".cache";
const constexpr char * CACHE_FOLDER = ".cache/clesperanto";

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
static auto
get_cache_directory_path() -> std::filesystem::path
{
  // cache directory is %LOCALAPPDATA%/clesperanto
  TCHAR path[MAX_PATH];
  if (FAILED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path)))
  {
    std::cerr << "Failed to get AppData\\Local directory\n";
    return std::filesystem::current_path() / std::filesystem::path(CACHE_FOLDER);
  }
  return std::filesystem::path(path) / std::filesystem::path(CACHE_FOLDER);
}
#else
static auto
get_cache_directory_path() -> std::filesystem::path
{
  // cache directory is $HOME/.cache/clesperanto
  char * home_dir = std::getenv("HOME");
  if (home_dir != nullptr)
  {
    return std::filesystem::path(home_dir) / std::filesystem::path(CACHE_FOLDER);
  }
  std::cerr << "Failed to get user home directory\n";
  return std::filesystem::current_path() / std::filesystem::path(CACHE_FOLDER);
}
#endif

static const auto CACHE_FOLDER_PATH = get_cache_directory_path();

} // namespace cle


#endif // __INCLUDE_CACHE_HPP
