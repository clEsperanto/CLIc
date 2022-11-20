

#include "cleDilateSphereKernel.hpp"

namespace cle
{

DilateSphereKernel::DilateSphereKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_dilate_sphere.h"
  };
  this->SetSource("dilate_sphere", cl_header);
}

auto
DilateSphereKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
DilateSphereKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
