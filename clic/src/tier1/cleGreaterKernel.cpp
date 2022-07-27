

#include "cleGreaterKernel.hpp"

namespace cle
{

GreaterKernel::GreaterKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_greater.h"
    };
    this->SetSource ("greater", cl_header);
}

void
GreaterKernel::SetInput1 (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
GreaterKernel::SetInput2 (const Image &object)
{
    this->AddParameter ("src1", object);
}

void
GreaterKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
