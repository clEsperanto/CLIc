

#include "cleReplaceIntensityKernel.hpp"

namespace cle
{

ReplaceIntensityKernel::ReplaceIntensityKernel(const ProcessorPointer & device)
  : Operation(device, 4)
{
  std::string cl_header = {
#include "cle_replace_intensity.h"
  };
  this->SetSource("replace_intensity", cl_header);
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
