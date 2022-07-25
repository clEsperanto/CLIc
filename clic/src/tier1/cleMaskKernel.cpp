

#include "cleMaskKernel.hpp"

namespace cle
{

MaskKernel::MaskKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_mask.h"
    };
    this->SetSource ("cle_mask", cl_header);
}

void
MaskKernel::SetInput (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
MaskKernel::SetMask (const Image &object)
{
    this->AddParameter ("src1", object);
}

void
MaskKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
