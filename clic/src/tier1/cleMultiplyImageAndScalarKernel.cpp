
#include "cleMultiplyImageAndScalarKernel.hpp"

namespace cle
{

MultiplyImageAndScalarKernel::MultiplyImageAndScalarKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_multiply_image_and_scalar.h"
  };
  this->SetSource("multiply_image_and_scalar", cl_header);
}

auto
MultiplyImageAndScalarKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
MultiplyImageAndScalarKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
MultiplyImageAndScalarKernel::SetScalar(const float & value) -> void
{
  this->AddParameter("scalar", value);
}

} // namespace cle
