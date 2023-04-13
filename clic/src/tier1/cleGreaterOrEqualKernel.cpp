

#include "cleGreaterOrEqualKernel.hpp"
#include "cle_greater_or_equal.h"

namespace cle
{

GreaterOrEqualKernel::GreaterOrEqualKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("greater_or_equal", oclKernel::greater_or_equal);
}

auto
GreaterOrEqualKernel::SetInput1(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
GreaterOrEqualKernel::SetInput2(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
GreaterOrEqualKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
