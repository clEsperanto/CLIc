

#include "cleBinarySubtractKernel.hpp"
#include "cle_binary_subtract.h"

namespace cle
{

BinarySubtractKernel::BinarySubtractKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("binary_subtract", oclKernel::binary_subtract);
}

auto
BinarySubtractKernel::SetInput1(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
BinarySubtractKernel::SetInput2(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
BinarySubtractKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
