

#include "cleSumXProjectionKernel.hpp"

namespace cle
{

SumXProjectionKernel::SumXProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_sum_x_projection.h"
  };
  this->SetSource("sum_x_projection", cl_header);
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
