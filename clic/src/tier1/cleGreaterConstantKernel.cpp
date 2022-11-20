

#include "cleGreaterConstantKernel.hpp"

namespace cle
{

GreaterConstantKernel::GreaterConstantKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_greater_constant.h"
  };
  this->SetSource("greater_constant", cl_header);
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
