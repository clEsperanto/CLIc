#include "cleCustomKernel.hpp"

#include <fstream>
#include <string>

namespace cle
{

CustomKernel::CustomKernel(const ProcessorPointer & device,
                           const std::string &      filename,
                           const std::string &      name,
                           const size_t &           nb_parameters)
  : Operation(device, nb_parameters)
{
  std::ifstream ifs(filename);
  std::string   ocl_source((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
  this->SetSource(name, ocl_source);
}

auto
CustomKernel::AddImages(const std::map<std::string, Image> & images) -> void
{
  for (auto ite = images.begin(); ite != images.end(); ite++)
  {
    kernel.AddParameter(ite->first, ite->second);
  }
}

auto
CustomKernel::AddScalars(const std::map<std::string, float> & scalars) -> void
{
  for (auto ite = scalars.begin(); ite != scalars.end(); ite++)
  {
    kernel.AddParameter(ite->first, ite->second);
  }
}

} // namespace cle
