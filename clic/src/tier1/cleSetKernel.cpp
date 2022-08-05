

#include "cleSetKernel.hpp"

namespace cle
{

SetKernel::SetKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_set.h"
  };
  this->SetSource("set", cl_header);
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
