

#include "cleGreaterConstantKernel.hpp"
#include "cle_greater_constant.h"

namespace cle
{

GreaterConstantKernel::GreaterConstantKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("greater_constant", oclKernel::greater_constant);
}

auto
GreaterConstantKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
GreaterConstantKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
GreaterConstantKernel::SetConstant(const float & value) -> void
{
  this->AddParameter("scalar", value);
}

} // namespace cle
