#include "cleMinimumImagesKernel.hpp"
#include "cle_minimum_images.h"

namespace cle
{

MinimumImagesKernel::MinimumImagesKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("minimum_images", oclKernel::minimum_images);
}

void
MinimumImagesKernel::SetInput1(const Image & object)
{
  this->AddParameter("src0", object);
}

void
MinimumImagesKernel::SetInput2(const Image & object)
{
  this->AddParameter("src1", object);
}

void
MinimumImagesKernel::SetOutput(const Image & object)
{
  this->AddParameter("dst", object);
}

} // namespace cle