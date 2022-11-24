#include "cleGradientZKernel.hpp"

namespace cle
{

GradientZKernel::GradientZKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header_ = {
#include "cle_gradient_z.h"
  };
  this->SetSource("gradient_z", cl_header_);
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
