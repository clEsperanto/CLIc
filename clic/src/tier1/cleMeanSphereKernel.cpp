
#include "cleMeanSphereKernel.hpp"

namespace cle
{

MeanSphereKernel::MeanSphereKernel(const ProcessorPointer & device)
  : Operation(device, 5)
{
  std::string cl_header = {
#include "cle_mean_sphere.h"
  };
  this->SetSource("mean_sphere", cl_header);
}

auto
MeanSphereKernel::Radius2KernelSize(const float & radius) const -> int
{
  return static_cast<int>(radius) * 2 + 1;
}

auto
MeanSphereKernel::SetRadius(const float & radius_x, const float & radius_y, const float & radius_z) -> void
{
  this->AddParameter("scalar0", Radius2KernelSize(radius_x));
  this->AddParameter("scalar1", Radius2KernelSize(radius_y));
  this->AddParameter("scalar2", Radius2KernelSize(radius_z));
}

auto
MeanSphereKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
MeanSphereKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
