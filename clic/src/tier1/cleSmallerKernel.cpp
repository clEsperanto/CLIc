

#include "cleSmallerKernel.hpp"

namespace cle
{

SmallerKernel::SmallerKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_smaller.h"
    };
    this->SetSource ("smaller", cl_header);
}

void
SmallerKernel::SetInput1 (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
SmallerKernel::SetInput2 (const Image &object)
{
    this->AddParameter ("src1", object);
}

void
SmallerKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
