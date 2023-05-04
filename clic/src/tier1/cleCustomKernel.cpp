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
  if (ifs.fail())
  {
    std::cout << "Error in loading custom kernel, could not find file " << filename << std::endl;
  }
  std::string ocl_source((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
  this->SetSource(name, ocl_source);
}

auto
CustomKernel::AddImage(const std::string & tag, const cle::Image & object) -> void
{
  this->AddParameter(tag, object);
}

auto
CustomKernel::AddScalar(const std::string & tag, const float & value) -> void
{
  this->AddParameter(tag, value);
}

auto
CustomKernel::AddScalar(const std::string & tag, const int & value) -> void
{
  this->AddParameter(tag, value);
}


} // namespace cle
