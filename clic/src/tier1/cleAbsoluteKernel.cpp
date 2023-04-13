#include "cleAbsoluteKernel.hpp"
#include "cle_absolute.h"

namespace cle
{

AbsoluteKernel::AbsoluteKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("absolute", oclKernel::absolute);
}

auto
AbsoluteKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
AbsoluteKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
