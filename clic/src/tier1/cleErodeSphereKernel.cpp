

#include "cleErodeSphereKernel.hpp"

namespace cle
{

ErodeSphereKernel::ErodeSphereKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_erode_sphere.h"
  };
  this->SetSource("erode_sphere", cl_header);
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
