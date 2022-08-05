#include "cleMaximumXProjectionKernel.hpp"

namespace cle
{

MaximumXProjectionKernel::MaximumXProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_maximum_x_projection.h"
  };
  this->SetSource("maximum_x_projection", cl_header);
}

auto
MaximumXProjectionKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
MaximumXProjectionKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
