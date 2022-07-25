
#include "cleCopyKernel.hpp"

namespace cle
{

CopyKernel::CopyKernel (const ProcessorPointer &device) : Operation (device, 2)
{
    std::string cl_header = {
#include "cle_copy.h"
    };
    this->SetSource ("cle_copy", cl_header);
}

void
CopyKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
CopyKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
