

#include "cleBinarySubtractKernel.hpp"

namespace cle
{

BinarySubtractKernel::BinarySubtractKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_binary_subtract.h"
  };
  this->SetSource("binary_subtract", cl_header);
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
