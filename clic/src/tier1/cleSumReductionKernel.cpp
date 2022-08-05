

#include "cleSumReductionXKernel.hpp"

namespace cle
{

SumReductionXKernel::SumReductionXKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_sum_reduction_x.h"
  };
  this->SetSource("sum_reduction_x", cl_header);
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
