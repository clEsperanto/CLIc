

#include "cleDivideImagesKernel.hpp"
#include "cle_divide_images.h"

namespace cle
{

DivideImagesKernel::DivideImagesKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("divide_images", oclKernel::divide_images);
}

auto
DivideImagesKernel::SetInput1(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
DivideImagesKernel::SetInput2(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
DivideImagesKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
