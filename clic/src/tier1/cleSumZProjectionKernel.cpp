

#include "cleSumZProjectionKernel.hpp"
#include "cle_sum_z_projection.h"

namespace cle
{

SumZProjectionKernel::SumZProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("sum_z_projection", oclKernel::sum_z_projection);
}

auto
SumZProjectionKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
SumZProjectionKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
