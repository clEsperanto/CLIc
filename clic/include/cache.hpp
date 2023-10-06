#ifndef __INCLUDE_CACHE_HPP
#define __INCLUDE_CACHE_HPP

#include <cstdlib> // for std::getenv
#include <filesystem>
#include <iostream>
#include <string>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#  include <userenv.h>
#  include <windows.h>
#  pragma comment(lib, "userenv.lib")
#endif

namespace cle
{

const constexpr char * HIDDEN_FOLDER = ".cle";
const constexpr char * OCL_CACHE_FOLDER = "ocl_cache";
const constexpr char * CU_CACHE_FOLDER = "cuda_cache";

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
static auto
get_home_directory_path() -> std::filesystem::path
{
  HANDLE hToken;
  DWORD  bufSize = MAX_PATH;
  if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
  {
    std::cerr << "Failed to open process token\n";
    return std::filesystem::current_path();
  }
  TCHAR path[MAX_PATH];
  if (!GetUserProfileDirectory(hToken, path, &bufSize))
  {
    std::cerr << "Failed to get user profile directory\n";
    CloseHandle(hToken);
    return std::filesystem::current_path();
  }
  CloseHandle(hToken);
  return std::filesystem::path(path);
}
#else
static auto
get_home_directory_path() -> std::filesystem::path
{
  char * home_dir = std::getenv("HOME");
  if (home_dir != nullptr)
  {
    return { home_dir };
  }
  std::cerr << "Failed to get user home directory\n";
  return std::filesystem::current_path();
}
#endif

static auto
get_home_directory(const std::string & cache) -> std::filesystem::path
{
  const auto hidden_folder = std::filesystem::path(cle::HIDDEN_FOLDER);
  const auto cache_folder = std::filesystem::path(cache);
  auto       cache_folder_path = get_home_directory_path() / hidden_folder / cache_folder;
  try
  {
    std::filesystem::create_directories(cache_folder_path);
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << '\n';
  }
  return cache_folder_path;
}

static const auto OLC_CACHE_FOLDER_PATH = get_home_directory("opencl_cache");
static const auto CU_CACHE_FOLDER_PATH = get_home_directory("cuda_cache");

} // namespace cle


#endif // __INCLUDE_CACHE_HPP
