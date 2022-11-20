

#include "cleNonzeroMinimumBoxKernel.hpp"

namespace cle
{

NonzeroMinimumBoxKernel::NonzeroMinimumBoxKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_nonzero_minimum_box.h"
  };
  this->SetSource("nonzero_minimum_box", cl_header);
}

auto
NonzeroMinimumBoxKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
NonzeroMinimumBoxKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst1", object);
  this->SetRange("dst1");
}

auto
NonzeroMinimumBoxKernel::SetOutputFlag(const Image & object) -> void
{
  this->AddParameter("dst0", object);
}

} // namespace cle
