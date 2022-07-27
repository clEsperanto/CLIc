

#include "cleReplaceIntensitiesKernel.hpp"

namespace cle
{

ReplaceIntensitiesKernel::ReplaceIntensitiesKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_replace_intensities.h"
    };
    this->SetSource ("replace_intensities", cl_header);
}

void
ReplaceIntensitiesKernel::SetInput (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
ReplaceIntensitiesKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
ReplaceIntensitiesKernel::SetMap (const Image &object)
{
    this->AddParameter ("src1", object);
}

} // namespace cle
