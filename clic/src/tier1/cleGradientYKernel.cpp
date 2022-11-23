#include "cleGradientYKernel.hpp"

namespace cle
{

GradientYKernel::GradientYKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header_ = {
#include "cle_gradient_y.h"
  };
  this->SetSource("cle_gradient_y", cl_header_);
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
