

#include "cleMinimumZProjectionKernel.hpp"

namespace cle
{

MinimumZProjectionKernel::MinimumZProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_minimum_z_projection.h"
  };
  this->SetSource("minimum_z_projection", cl_header);
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
