

#include "cleSetNonzeroPixelsToPixelindexKernel.hpp"

namespace cle
{

SetNonzeroPixelsToPixelindexKernel::SetNonzeroPixelsToPixelindexKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_set_nonzero_pixels_to_pixelindex.h"
    };
    this->SetSource ("cle_set_nonzero_pixels_to_pixelindex", cl_header);
}

void
SetNonzeroPixelsToPixelindexKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
SetNonzeroPixelsToPixelindexKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
SetNonzeroPixelsToPixelindexKernel::SetOffset (const int &value)
{
    this->AddParameter ("offset", value);
}

} // namespace cle
