

#include "cleGreaterOrEqualKernel.hpp"

namespace cle
{

GreaterOrEqualKernel::GreaterOrEqualKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_greater_or_equal.h"
  };
  this->SetSource("greater_or_equal", cl_header);
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
