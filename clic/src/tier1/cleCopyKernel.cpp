
#include "cleCopyKernel.hpp"
#include "cle_copy.h"

namespace cle
{

CopyKernel::CopyKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("copy", oclKernel::copy);
}

auto
CopyKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
CopyKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
