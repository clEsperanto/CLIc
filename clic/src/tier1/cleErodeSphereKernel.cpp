

#include "cleErodeSphereKernel.hpp"
#include "cle_erode_sphere.h"

namespace cle
{

ErodeSphereKernel::ErodeSphereKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("erode_sphere", oclKernel::erode_sphere);
}

auto
ErodeSphereKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
ErodeSphereKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
