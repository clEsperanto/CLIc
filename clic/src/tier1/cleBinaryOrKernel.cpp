

#include "cleBinaryOrKernel.hpp"

namespace cle
{

BinaryOrKernel::BinaryOrKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_binary_or.h"
  };
  this->SetSource("binary_or", cl_header);
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
