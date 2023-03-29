

#include "cleGreaterKernel.hpp"
#include "cle_greater.h"

namespace cle
{

GreaterKernel::GreaterKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("greater", oclKernel::greater);
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
