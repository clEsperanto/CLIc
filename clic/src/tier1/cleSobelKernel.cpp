

#include "cleSobelKernel.hpp"

namespace cle
{

SobelKernel::SobelKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_sobel.h"
  };
  this->SetSource("sobel", cl_header);
}

auto
SobelKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
SobelKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
