

#include "cleConvolveKernel.hpp"

namespace cle
{

ConvolveKernel::ConvolveKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header_ = {
#include "cle_convolve.h"
  };
  this->SetSource("convolve", cl_header_);
}

auto
ConvolveKernel::SetInput1(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
ConvolveKernel::SetInput2(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
ConvolveKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
