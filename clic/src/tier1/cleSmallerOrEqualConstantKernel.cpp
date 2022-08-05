

#include "cleSmallerOrEqualConstantKernel.hpp"

namespace cle
{

SmallerOrEqualConstantKernel::SmallerOrEqualConstantKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_smaller_or_equal_constant.h"
  };
  this->SetSource("smaller_or_equal_constant", cl_header);
}

auto
SmallerOrEqualConstantKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
SmallerOrEqualConstantKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
SmallerOrEqualConstantKernel::SetConstant(const float & value) -> void
{
  this->AddParameter("scalar", value);
}

} // namespace cle
