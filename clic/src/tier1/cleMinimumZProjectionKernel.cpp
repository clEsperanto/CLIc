

#include "cleMinimumZProjectionKernel.hpp"
#include "cle_minimum_z_projection.h"

namespace cle
{

MinimumZProjectionKernel::MinimumZProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("minimum_z_projection", oclKernel::minimum_z_projection);
}

auto
MinimumZProjectionKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
MinimumZProjectionKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
