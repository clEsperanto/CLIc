#include "cleMaximumImagesKernel.hpp"
#include "cle_maximum_images.h"

namespace cle
{

MaximumImagesKernel::MaximumImagesKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("maximum_images", oclKernel::maximum_images);
}

void
MaximumImagesKernel::SetInput1(const Image & object)
{
  this->AddParameter("src0", object);
}

void
MaximumImagesKernel::SetInput2(const Image & object)
{
  this->AddParameter("src1", object);
}

void
MaximumImagesKernel::SetOutput(const Image & object)
{
  this->AddParameter("dst", object);
}

} // namespace cle