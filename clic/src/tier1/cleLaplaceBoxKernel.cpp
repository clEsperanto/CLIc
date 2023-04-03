
#include "cleLaplaceBoxKernel.hpp"

namespace cle
{

LaplaceBoxKernel::LaplaceBoxKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_laplace_box.h"
  };
  this->SetSource("laplace_box", cl_header);
}

auto
LaplaceBoxKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
LaplaceBoxKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle