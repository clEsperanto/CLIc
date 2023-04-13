
#include "cleAddImageAndScalarKernel.hpp"
#include "cle_add_image_and_scalar.h"

namespace cle
{

AddImageAndScalarKernel::AddImageAndScalarKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("add_image_and_scalar", oclKernel::add_image_and_scalar);
}

auto
AddImageAndScalarKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
AddImageAndScalarKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
AddImageAndScalarKernel::SetScalar(const float & value) -> void
{
  this->AddParameter("scalar", value);
}

} // namespace cle
