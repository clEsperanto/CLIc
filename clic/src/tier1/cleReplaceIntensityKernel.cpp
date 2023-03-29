

#include "cleReplaceIntensityKernel.hpp"
#include "cle_replace_intensity.h"

namespace cle
{

ReplaceIntensityKernel::ReplaceIntensityKernel(const ProcessorPointer & device)
  : Operation(device, 4)
{
  this->SetSource("replace_intensity", oclKernel::replace_intensity);
}

auto
ReplaceIntensityKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
ReplaceIntensityKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
ReplaceIntensityKernel::SetInValue(const float & value) -> void
{
  this->AddParameter("scalar0", value);
}

auto
ReplaceIntensityKernel::SetOutValue(const float & value) -> void
{
  this->AddParameter("scalar1", value);
}

} // namespace cle
