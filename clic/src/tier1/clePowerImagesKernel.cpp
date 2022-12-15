
#include "clePowerImagesKernel.hpp"

namespace cle
{

PowerImagesKernel::PowerImagesKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_power_images.h"
  };
  this->SetSource("power_images", cl_header);
}

auto
PowerImagesKernel::SetInput1(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
PowerImagesKernel::SetInput2(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
PowerImagesKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle
