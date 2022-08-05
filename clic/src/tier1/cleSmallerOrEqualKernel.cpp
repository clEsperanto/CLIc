

#include "cleSmallerOrEqualKernel.hpp"

namespace cle
{

SmallerOrEqualKernel::SmallerOrEqualKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_smaller_or_equal.h"
  };
  this->SetSource("smaller_or_equal", cl_header);
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
