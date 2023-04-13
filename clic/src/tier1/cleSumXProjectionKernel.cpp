

#include "cleSumXProjectionKernel.hpp"
#include "cle_sum_x_projection.h"

namespace cle
{

SumXProjectionKernel::SumXProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("sum_x_projection", oclKernel::sum_x_projection);
}

auto
SumXProjectionKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
SumXProjectionKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
