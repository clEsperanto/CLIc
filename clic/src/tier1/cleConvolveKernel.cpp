

#include "cleConvolveKernel.hpp"
#include "cle_convolve.h"

namespace cle
{

ConvolveKernel::ConvolveKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("convolve", oclKernel::convolve);
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
