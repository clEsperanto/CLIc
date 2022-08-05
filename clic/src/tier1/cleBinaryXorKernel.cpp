

#include "cleBinaryXorKernel.hpp"

namespace cle
{

BinaryXorKernel::BinaryXorKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_binary_xor.h"
  };
  this->SetSource("binary_xor", cl_header);
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
