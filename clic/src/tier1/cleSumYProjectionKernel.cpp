

#include "cleSumYProjectionKernel.hpp"
#include "cle_sum_y_projection.h"

namespace cle
{

SumYProjectionKernel::SumYProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("sum_y_projection", oclKernel::sum_y_projection);
}

auto
SumYProjectionKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
SumYProjectionKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
