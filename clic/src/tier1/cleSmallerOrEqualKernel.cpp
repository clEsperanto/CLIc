

#include "cleSmallerOrEqualKernel.hpp"
#include "cle_smaller_or_equal.h"

namespace cle
{

SmallerOrEqualKernel::SmallerOrEqualKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("smaller_or_equal", oclKernel::smaller_or_equal);
}

auto
SmallerOrEqualKernel::SetInput1(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
SmallerOrEqualKernel::SetInput2(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
SmallerOrEqualKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
