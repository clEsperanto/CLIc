

#include "cleMaskKernel.hpp"
#include "cle_mask.h"

namespace cle
{

MaskKernel::MaskKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("mask", oclKernel::mask);
}

auto
MaskKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
MaskKernel::SetMask(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
MaskKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
