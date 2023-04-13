

#include "cleBinaryNotKernel.hpp"
#include "cle_binary_not.h"

namespace cle
{

BinaryNotKernel::BinaryNotKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("binary_not", oclKernel::binary_not);
}

auto
BinaryNotKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
BinaryNotKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
