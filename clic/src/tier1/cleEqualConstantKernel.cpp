

#include "cleEqualConstantKernel.hpp"
#include "cle_equal_constant.h"

namespace cle
{

EqualConstantKernel::EqualConstantKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("equal_constant", oclKernel::equal_constant);
}

auto
EqualConstantKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
EqualConstantKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
EqualConstantKernel::SetConstant(const float & value) -> void
{
  this->AddParameter("scalar", value);
}

} // namespace cle
