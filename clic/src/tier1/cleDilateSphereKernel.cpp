

#include "cleDilateSphereKernel.hpp"
#include "cle_dilate_sphere.h"

namespace cle
{

DilateSphereKernel::DilateSphereKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("dilate_sphere", oclKernel::dilate_sphere);
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
