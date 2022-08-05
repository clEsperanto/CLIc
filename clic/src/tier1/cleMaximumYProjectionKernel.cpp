#include "cleMaximumYProjectionKernel.hpp"

namespace cle
{

MaximumYProjectionKernel::MaximumYProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_maximum_y_projection.h"
  };
  this->SetSource("maximum_y_projection", cl_header);
}

auto
MaximumYProjectionKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
MaximumYProjectionKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
