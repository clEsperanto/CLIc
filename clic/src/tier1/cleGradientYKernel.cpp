#include "cleGradientYKernel.hpp"
#include "cle_gradient_y.h"

namespace cle
{

GradientYKernel::GradientYKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("gradient_y", oclKernel::gradient_y);
}

auto
GradientYKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
GradientYKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
