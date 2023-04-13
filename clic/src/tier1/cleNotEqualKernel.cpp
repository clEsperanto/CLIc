

#include "cleNotEqualKernel.hpp"
#include "cle_not_equal.h"

namespace cle
{

NotEqualKernel::NotEqualKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("not_equal", oclKernel::not_equal);
}

auto
NotEqualKernel::SetInput1(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
NotEqualKernel::SetInput2(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
NotEqualKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
