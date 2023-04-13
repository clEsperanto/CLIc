
#include "clePowerImagesKernel.hpp"
#include "cle_power_images.h"

namespace cle
{

PowerImagesKernel::PowerImagesKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("power_images", oclKernel::power_images);
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
