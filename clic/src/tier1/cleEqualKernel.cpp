
#include "cleEqualKernel.hpp"
#include "cle_equal.h"

namespace cle
{

EqualKernel::EqualKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("equal", oclKernel::equal);
}

auto
EqualKernel::SetInput1(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
EqualKernel::SetInput2(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
EqualKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
