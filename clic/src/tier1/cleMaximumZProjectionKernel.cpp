#include "cleMaximumZProjectionKernel.hpp"
#include "cle_maximum_z_projection.h"

namespace cle
{

MaximumZProjectionKernel::MaximumZProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("maximum_z_projection", oclKernel::maximum_z_projection);
}

auto
MaximumZProjectionKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
MaximumZProjectionKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
