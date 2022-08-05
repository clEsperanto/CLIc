

#include "cleSumZProjectionKernel.hpp"

namespace cle
{

SumZProjectionKernel::SumZProjectionKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_sum_z_projection.h"
  };
  this->SetSource("sum_z_projection", cl_header);
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
