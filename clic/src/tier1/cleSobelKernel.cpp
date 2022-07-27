

#include "cleSobelKernel.hpp"

namespace cle
{

SobelKernel::SobelKernel (const ProcessorPointer &device) : Operation (device, 2)
{
    std::string cl_header = {
#include "cle_sobel.h"
    };
    this->SetSource ("sobel", cl_header);
}

void
SobelKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
SobelKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
