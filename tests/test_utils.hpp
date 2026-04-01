#pragma once

#include <string>
#include <vector>

inline std::vector<std::string>
getParameters()
{
  std::vector<std::string> parameters;
#if USE_OPENCL
  parameters.push_back("opencl");
#endif
#if USE_CUDA
  parameters.push_back("cuda");
#endif
#if USE_METAL
  parameters.push_back("metal");
#endif
  return parameters;
}