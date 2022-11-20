#include "cleAbsoluteKernel.hpp"

namespace cle
{

AbsoluteKernel::AbsoluteKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header_ = {
#include "cle_absolute.h"
  };
  this->SetSource("absolute", cl_header_);
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
