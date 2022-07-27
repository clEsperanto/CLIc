

#include "cleReplaceIntensityKernel.hpp"

namespace cle
{

ReplaceIntensityKernel::ReplaceIntensityKernel (const ProcessorPointer &device) : Operation (device, 4)
{
    std::string cl_header = {
#include "cle_replace_intensity.h"
    };
    this->SetSource ("replace_intensity", cl_header);
}

void
ReplaceIntensityKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
ReplaceIntensityKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
ReplaceIntensityKernel::SetInValue (const float &value)
{
    this->AddParameter ("scalar0", value);
}

void
ReplaceIntensityKernel::SetOutValue (const float &value)
{
    this->AddParameter ("scalar1", value);
}

} // namespace cle
