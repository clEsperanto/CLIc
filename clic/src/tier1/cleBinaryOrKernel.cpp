

#include "cleBinaryOrKernel.hpp"
#include "cle_binary_or.h"

namespace cle
{

BinaryOrKernel::BinaryOrKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("binary_or", oclKernel::binary_or);
}

auto
BinaryOrKernel::SetInput1(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
BinaryOrKernel::SetInput2(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
BinaryOrKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
