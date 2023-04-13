

#include "cleNotEqualConstantKernel.hpp"
#include "cle_not_equal_constant.h"

namespace cle
{

NotEqualConstantKernel::NotEqualConstantKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("not_equal_constant", oclKernel::not_equal_constant);
}

auto
NotEqualConstantKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
NotEqualConstantKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
NotEqualConstantKernel::SetConstant(const float & value) -> void
{
  this->AddParameter("scalar", value);
}

} // namespace cle
