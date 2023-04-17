

#include "cleLaplaceBoxKernel.hpp"
#include "cle_laplace_box.h"

namespace cle
{

LaplaceBoxKernel::LaplaceBoxKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("laplace_box", oclKernel::laplace_box);
}

auto
LaplaceBoxKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
LaplaceBoxKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle