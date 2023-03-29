#include "cleMaximumYProjectionKernel.hpp"
#include "cle_maximum_y_projection.h"

namespace cle
{

MaximumYProjectionKernel::MaximumYProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("maximum_y_projection", oclKernel::maximum_y_projection);
}

auto
MaximumYProjectionKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
MaximumYProjectionKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
