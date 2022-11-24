#include "cleGradientXKernel.hpp"

namespace cle
{

GradientXKernel::GradientXKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header_ = {
#include "cle_gradient_x.h"
  };
  this->SetSource("gradient_x", cl_header_);
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
