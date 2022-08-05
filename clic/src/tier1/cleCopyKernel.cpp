
#include "cleCopyKernel.hpp"

namespace cle
{

CopyKernel::CopyKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_copy.h"
  };
  this->SetSource("copy", cl_header);
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
