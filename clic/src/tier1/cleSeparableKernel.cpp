#include "cleSeparableKernel.hpp"

namespace cle
{

SeparableKernel::SeparableKernel(const ProcessorPointer & device)
  : Operation(device, 5)
{}

auto
SeparableKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
SeparableKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
SeparableKernel::SetSigma(const float & sigma) -> void
{
  this->AddParameter("s", sigma);
}

auto
SeparableKernel::SetSize(const int & radius) -> void
{
  this->AddParameter("N", radius);
}

auto
SeparableKernel::SetDimension(const int & dimension) -> void
{
  this->AddParameter("dim", dimension);
}

} // namespace cle
