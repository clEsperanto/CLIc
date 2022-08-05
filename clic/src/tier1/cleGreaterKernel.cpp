

#include "cleGreaterKernel.hpp"

namespace cle
{

GreaterKernel::GreaterKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_greater.h"
  };
  this->SetSource("greater", cl_header);
}

auto
GreaterKernel::SetInput1(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
GreaterKernel::SetInput2(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
GreaterKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
