

#include "cleSumYProjectionKernel.hpp"

namespace cle
{

SumYProjectionKernel::SumYProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_sum_y_projection.h"
  };
  this->SetSource("sum_y_projection", cl_header);
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
