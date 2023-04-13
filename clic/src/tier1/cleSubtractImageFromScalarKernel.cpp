
#include "cleSubtractImageFromScalarKernel.hpp"
#include "cle_subtract_image_from_scalar.h"

namespace cle
{

SubtractImageFromScalarKernel::SubtractImageFromScalarKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("subtract_image_from_scalar", oclKernel::subtract_image_from_scalar);
}

auto
SubtractImageFromScalarKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
SubtractImageFromScalarKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
SubtractImageFromScalarKernel::SetScalar(const float & value) -> void
{
  this->AddParameter("scalar", value);
}

} // namespace cle
