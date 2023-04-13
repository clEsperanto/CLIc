

#include "cleSobelKernel.hpp"
#include "cle_sobel.h"

namespace cle
{

SobelKernel::SobelKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("sobel", oclKernel::sobel);
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
