

#include "cleNotEqualKernel.hpp"

namespace cle
{

NotEqualKernel::NotEqualKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_not_equal.h"
  };
  this->SetSource("not_equal", cl_header);
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
