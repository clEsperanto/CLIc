#include "cleMaximumSphereKernel.hpp"
#include "cle_maximum_sphere.h"

namespace cle
{

MaximumSphereKernel::MaximumSphereKernel(const ProcessorPointer & device)
  : Operation(device, 5)
{
  this->SetSource("maximum_sphere", oclKernel::maximum_sphere);
}

auto
MaximumSphereKernel::Radius2KernelSize(const float & radius) const -> int
{
  return static_cast<int>(radius) * 2 + 1;
}

void
MaximumSphereKernel::SetInput(const Image & object)
{
  this->AddParameter("src", object);
}

void
MaximumSphereKernel::SetOutput(const Image & object)
{
  this->AddParameter("dst", object);
}

void
MaximumSphereKernel::SetRadius(const float & radius_x, const float & radius_y, const float & radius_z)
{
  this->AddParameter("scalar0", Radius2KernelSize(radius_x));
  this->AddParameter("scalar1", Radius2KernelSize(radius_y));
  this->AddParameter("scalar2", Radius2KernelSize(radius_z));
}

} // namespace cle