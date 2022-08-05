#include "cleMaximumZProjectionKernel.hpp"

namespace cle
{

MaximumZProjectionKernel::MaximumZProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_maximum_z_projection.h"
  };
  this->SetSource("maximum_z_projection", cl_header);
}

auto
MaximumZProjectionKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
MaximumZProjectionKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
