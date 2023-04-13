

#include "cleBinaryXorKernel.hpp"
#include "cle_binary_xor.h"

namespace cle
{

BinaryXorKernel::BinaryXorKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("binary_xor", oclKernel::binary_xor);
}

auto
BinaryXorKernel::SetInput1(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
BinaryXorKernel::SetInput2(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
BinaryXorKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
