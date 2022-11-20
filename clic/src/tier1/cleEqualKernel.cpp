

#include "cleEqualKernel.hpp"

namespace cle
{

EqualKernel::EqualKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_equal.h"
  };
  this->SetSource("equal", cl_header);
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
