#include "cleGradientZKernel.hpp"
#include "cle_gradient_z.h"

namespace cle
{

GradientZKernel::GradientZKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("gradient_z", oclKernel::gradient_z);
}

auto
GradientZKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
GradientZKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
