

#include "cleMinimumXProjectionKernel.hpp"
#include "cle_minimum_x_projection.h"

namespace cle
{

MinimumXProjectionKernel::MinimumXProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("minimum_x_projection", oclKernel::minimum_x_projection);
}

auto
MinimumXProjectionKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
MinimumXProjectionKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
