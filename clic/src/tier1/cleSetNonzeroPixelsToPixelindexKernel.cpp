

#include "cleSetNonzeroPixelsToPixelindexKernel.hpp"

namespace cle
{

SetNonzeroPixelsToPixelindexKernel::SetNonzeroPixelsToPixelindexKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_set_nonzero_pixels_to_pixelindex.h"
  };
  this->SetSource("set_nonzero_pixels_to_pixelindex", cl_header);
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
