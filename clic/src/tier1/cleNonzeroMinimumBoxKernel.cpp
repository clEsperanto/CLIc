

#include "cleNonzeroMinimumBoxKernel.hpp"
#include "cle_nonzero_minimum_box.h"

namespace cle
{

NonzeroMinimumBoxKernel::NonzeroMinimumBoxKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("nonzero_minimum_box", oclKernel::nonzero_minimum_box);
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
  this->SetRange(this->GetImage("dst1")->Shape());
}

auto
NonzeroMinimumBoxKernel::SetOutputFlag(const Image & object) -> void
{
  this->AddParameter("dst0", object);
}

} // namespace cle
