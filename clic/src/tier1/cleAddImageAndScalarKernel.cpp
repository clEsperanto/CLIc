
#include "cleAddImageAndScalarKernel.hpp"

namespace cle
{

AddImageAndScalarKernel::AddImageAndScalarKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_add_image_and_scalar.h"
  };
  this->SetSource("add_image_and_scalar", cl_header);
}

auto
AddImageAndScalarKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
AddImageAndScalarKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
AddImageAndScalarKernel::SetScalar(const float & value) -> void
{
  this->AddParameter("scalar", value);
}

} // namespace cle
