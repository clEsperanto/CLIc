#include "cleDivideImageAndScalarKernel.hpp"
#include "cle_divide_image_and_scalar.h"

namespace cle
{

DivideImageAndScalarKernel::DivideImageAndScalarKernel(const ProcessorPointer & device)
  : Operation(device, 5)
{
  this->SetSource("divide_image_and_scalar", oclKernel::divide_image_and_scalar);
}

void
DivideImageAndScalarKernel::SetInput(const Image & object)
{
  this->AddParameter("src", object);
}

void
DivideImageAndScalarKernel::SetOutput(const Image & object)
{
  this->AddParameter("dst", object);
}

void
DivideImageAndScalarKernel::SetScalar(const float & scalar)
{
  this->AddParameter("scalar", scalar);
}

} // namespace cle