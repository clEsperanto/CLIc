

#include "cleMultiplyImagesKernel.hpp"
#include "cle_multiply_images.h"

namespace cle
{

MultiplyImagesKernel::MultiplyImagesKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("multiply_images", oclKernel::multiply_images);
}

auto
MultiplyImagesKernel::SetInput1(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
MultiplyImagesKernel::SetInput2(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
MultiplyImagesKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
