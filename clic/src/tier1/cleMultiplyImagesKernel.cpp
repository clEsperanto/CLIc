

#include "cleMultiplyImagesKernel.hpp"

namespace cle
{

MultiplyImagesKernel::MultiplyImagesKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_multiply_images.h"
  };
  this->SetSource("multiply_images", cl_header);
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
