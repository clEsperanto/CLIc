

#include "cleEqualConstantKernel.hpp"

namespace cle
{

EqualConstantKernel::EqualConstantKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_equal_constant.h"
  };
  this->SetSource("equal_constant", cl_header);
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
