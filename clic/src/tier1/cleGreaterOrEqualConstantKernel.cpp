

#include "cleGreaterOrEqualConstantKernel.hpp"

namespace cle
{

GreaterOrEqualConstantKernel::GreaterOrEqualConstantKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_greater_or_equal_constant.h"
  };
  this->SetSource("greater_or_equal_constant", cl_header);
}

auto
GreaterOrEqualConstantKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
GreaterOrEqualConstantKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
GreaterOrEqualConstantKernel::SetConstant(const float & value) -> void
{
  this->AddParameter("scalar", value);
}

} // namespace cle
