

#include "cleMinimumXProjectionKernel.hpp"

namespace cle
{

MinimumXProjectionKernel::MinimumXProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_minimum_x_projection.h"
  };
  this->SetSource("minimum_x_projection", cl_header);
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
