#include "cleGradientXKernel.hpp"
#include "cle_gradient_x.h"

namespace cle
{

GradientXKernel::GradientXKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("gradient_x", oclKernel::gradient_x);
}

auto
GradientXKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
GradientXKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
