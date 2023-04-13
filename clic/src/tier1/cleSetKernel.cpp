

#include "cleSetKernel.hpp"
#include "cle_set.h"

namespace cle
{

SetKernel::SetKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("set", oclKernel::set);
}

auto
SetKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
SetKernel::SetValue(const float & value) -> void
{
  this->AddParameter("scalar", value);
}

} // namespace cle
