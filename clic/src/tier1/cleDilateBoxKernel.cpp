#include "cleDilateBoxKernel.hpp"
#include "cle_dilate_box.h"

namespace cle
{

DilateBoxKernel::DilateBoxKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("dilate_box", oclKernel::dilate_box);
}

auto
DilateBoxKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
DilateBoxKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle