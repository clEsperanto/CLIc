

#include "cleSmallerConstantKernel.hpp"
#include "cle_smaller_constant.h"

namespace cle
{

SmallerConstantKernel::SmallerConstantKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("smaller_constant", oclKernel::smaller_constant);
}

auto
SmallerConstantKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
SmallerConstantKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
SmallerConstantKernel::SetConstant(const float & value) -> void
{
  this->AddParameter("scalar", value);
}

} // namespace cle
