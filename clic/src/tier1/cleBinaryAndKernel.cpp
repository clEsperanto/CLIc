

#include "cleBinaryAndKernel.hpp"

namespace cle
{

BinaryAndKernel::BinaryAndKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_binary_and.h"
  };
  this->SetSource("binary_and", cl_header);
}

auto
BinaryAndKernel::SetInput1(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
BinaryAndKernel::SetInput2(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
BinaryAndKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
