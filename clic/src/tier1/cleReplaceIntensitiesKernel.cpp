

#include "cleReplaceIntensitiesKernel.hpp"

namespace cle
{

ReplaceIntensitiesKernel::ReplaceIntensitiesKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_replace_intensities.h"
  };
  this->SetSource("replace_intensities", cl_header);
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
