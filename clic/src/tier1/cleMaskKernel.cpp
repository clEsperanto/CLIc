

#include "cleMaskKernel.hpp"

namespace cle
{

MaskKernel::MaskKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_mask.h"
  };
  this->SetSource("mask", cl_header);
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
