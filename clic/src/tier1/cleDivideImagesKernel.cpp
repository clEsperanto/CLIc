

#include "cleDivideImagesKernel.hpp"

namespace cle
{

DivideImagesKernel::DivideImagesKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_divide_images.h"
  };
  this->SetSource("divide_images", cl_header);
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
