

#include "cleMinimumYProjectionKernel.hpp"
#include "cle_minimum_y_projection.h"

namespace cle
{

MinimumYProjectionKernel::MinimumYProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("minimum_y_projection", oclKernel::minimum_y_projection);
}

auto
MinimumYProjectionKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
MinimumYProjectionKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
