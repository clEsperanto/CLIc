

#include "cleSmallerConstantKernel.hpp"

namespace cle
{

SmallerConstantKernel::SmallerConstantKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_smaller_constant.h"
  };
  this->SetSource("smaller_constant", cl_header);
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
