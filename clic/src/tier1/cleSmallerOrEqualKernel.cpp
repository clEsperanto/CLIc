

#include "cleSmallerOrEqualKernel.hpp"

namespace cle
{

SmallerOrEqualKernel::SmallerOrEqualKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_smaller_or_equal.h"
    };
    this->SetSource ("smaller_or_equal", cl_header);
}

void
SmallerOrEqualKernel::SetInput1 (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
SmallerOrEqualKernel::SetInput2 (const Image &object)
{
    this->AddParameter ("src1", object);
}

void
SmallerOrEqualKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
