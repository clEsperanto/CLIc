#include "cleMinimumSphereKernel.hpp"
#include "cle_minimum_sphere.h"

namespace cle
{

MinimumSphereKernel::MinimumSphereKernel(const ProcessorPointer & device)
  : Operation(device, 5)
{
  this->SetSource("minimum_sphere", oclKernel::minimum_sphere);
}

auto
MinimumSphereKernel::Radius2KernelSize(const float & radius) const -> int
{
  return static_cast<int>(radius) * 2 + 1;
}

void
MinimumSphereKernel::SetInput(const Image & object)
{
  this->AddParameter("src", object);
}

void
MinimumSphereKernel::SetOutput(const Image & object)
{
  this->AddParameter("dst", object);
}

void
MinimumSphereKernel::SetRadius(const float & radius_x, const float & radius_y, const float & radius_z)
{
  this->AddParameter("scalar0", Radius2KernelSize(radius_x));
  this->AddParameter("scalar1", Radius2KernelSize(radius_y));
  this->AddParameter("scalar2", Radius2KernelSize(radius_z));
}

} // namespace cle