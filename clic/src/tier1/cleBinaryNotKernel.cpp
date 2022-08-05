

#include "cleBinaryNotKernel.hpp"

namespace cle
{

BinaryNotKernel::BinaryNotKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_binary_not.h"
  };
  this->SetSource("binary_not", cl_header);
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
