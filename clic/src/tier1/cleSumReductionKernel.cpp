

#include "cleSumReductionXKernel.hpp"
#include "cle_sum_reduction_x.h"

namespace cle
{

SumReductionXKernel::SumReductionXKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("sum_reduction_x", oclKernel::sum_reduction_x);
}

auto
SumReductionXKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
SumReductionXKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
SumReductionXKernel::SetBlocksize(const int & size) -> void
{
  this->AddParameter("index", size);
}

} // namespace cle
