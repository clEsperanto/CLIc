#include "clePowerKernel.hpp"

namespace cle
{

PowerKernel::PowerKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header_ = {
#include "cle_power.h"
  };
  this->SetSource("power", cl_header_);
}

auto
PowerKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
PowerKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
PowerKernel::SetExponent(const float & value) -> void
{
  this->AddParameter("scalar", value);
}

} // namespace cle
