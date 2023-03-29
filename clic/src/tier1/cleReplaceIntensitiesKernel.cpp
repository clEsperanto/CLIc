

#include "cleReplaceIntensitiesKernel.hpp"
#include "cle_replace_intensities.h"

namespace cle
{

ReplaceIntensitiesKernel::ReplaceIntensitiesKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("replace_intensities", oclKernel::replace_intensities);
}

auto
ReplaceIntensitiesKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
ReplaceIntensitiesKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
ReplaceIntensitiesKernel::SetMap(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

} // namespace cle
