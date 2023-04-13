

#include "cleSetNonzeroPixelsToPixelindexKernel.hpp"
#include "cle_set_nonzero_pixels_to_pixelindex.h"

namespace cle
{

SetNonzeroPixelsToPixelindexKernel::SetNonzeroPixelsToPixelindexKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("set_nonzero_pixels_to_pixelindex", oclKernel::set_nonzero_pixels_to_pixelindex);
}

auto
SetNonzeroPixelsToPixelindexKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
SetNonzeroPixelsToPixelindexKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
SetNonzeroPixelsToPixelindexKernel::SetOffset(const int & value) -> void
{
  this->AddParameter("offset", value);
}

} // namespace cle
