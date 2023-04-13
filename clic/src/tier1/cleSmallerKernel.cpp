

#include "cleSmallerKernel.hpp"
#include "cle_smaller.h"

namespace cle
{

SmallerKernel::SmallerKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("smaller", oclKernel::smaller);
}

auto
SmallerKernel::SetInput1(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
SmallerKernel::SetInput2(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
SmallerKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
