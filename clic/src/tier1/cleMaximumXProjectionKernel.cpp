#include "cleMaximumXProjectionKernel.hpp"
#include "cle_maximum_x_projection.h"

namespace cle
{

MaximumXProjectionKernel::MaximumXProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("maximum_x_projection", oclKernel::maximum_x_projection);
}

auto
MaximumXProjectionKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
MaximumXProjectionKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
