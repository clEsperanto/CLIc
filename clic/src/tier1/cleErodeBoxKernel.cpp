#include "cleErodeBoxKernel.hpp"
#include "cle_erode_box.h"

namespace cle
{

ErodeBoxKernel::ErodeBoxKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("erode_box", oclKernel::erode_box);
}

auto
ErodeBoxKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
ErodeBoxKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

} // namespace cle