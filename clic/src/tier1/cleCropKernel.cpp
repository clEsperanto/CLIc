
#include "cleCropKernel.hpp"

namespace cle
{
CropKernel::CropKernel(const ProcessorPointer & device)
  : Operation(device, 5)
{
  std::string cl_header = {
#include "cle_crop.h"
  };
  this->SetSource("crop", cl_header);
}

auto
CropKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
CropKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
CropKernel::SetIndex(const int & index0, const int & index1, const int & index2) -> void
{
  this->AddParameter("index0", index0);
  this->AddParameter("index1", index1);
  this->AddParameter("index2", index2);
}

} // namespace cle
