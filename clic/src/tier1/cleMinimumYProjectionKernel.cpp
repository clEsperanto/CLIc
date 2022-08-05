

#include "cleMinimumYProjectionKernel.hpp"

namespace cle
{

MinimumYProjectionKernel::MinimumYProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_minimum_y_projection.h"
  };
  this->SetSource("minimum_y_projection", cl_header);
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
