#include "clePowerKernel.hpp"
#include "cle_power.h"

namespace cle
{

PowerKernel::PowerKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("power", oclKernel::power);
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
