

#include "cleGreaterOrEqualKernel.hpp"

namespace cle
{

GreaterOrEqualKernel::GreaterOrEqualKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_greater_or_equal.h"
    };
    this->SetSource ("greater_or_equal", cl_header);
}

void
GreaterOrEqualKernel::SetInput1 (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
GreaterOrEqualKernel::SetInput2 (const Image &object)
{
    this->AddParameter ("src1", object);
}

void
GreaterOrEqualKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
