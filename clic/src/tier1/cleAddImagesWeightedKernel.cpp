#include "cleAddImagesWeightedKernel.hpp"

namespace cle
{

AddImagesWeightedKernel::AddImagesWeightedKernel(const ProcessorPointer & device)
  : Operation(device, 5)
{
  std::string cl_header = {
#include "cle_add_images_weighted.h"
  };
  this->SetSource("add_images_weighted", cl_header);
}

auto
AddImagesWeightedKernel::SetInput1(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
AddImagesWeightedKernel::SetInput2(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
AddImagesWeightedKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
AddImagesWeightedKernel::SetFactor1(const float & value) -> void
{
  this->AddParameter("scalar0", value);
}

auto
AddImagesWeightedKernel::SetFactor2(const float & value) -> void
{
  this->AddParameter("scalar1", value);
}

} // namespace cle
