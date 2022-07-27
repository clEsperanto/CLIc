

#include "cleGreaterConstantKernel.hpp"

namespace cle
{

GreaterConstantKernel::GreaterConstantKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_greater_constant.h"
    };
    this->SetSource ("greater_constant", cl_header);
}

void
GreaterConstantKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
GreaterConstantKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
GreaterConstantKernel::SetScalar (const float &value)
{
    this->AddParameter ("scalar", value);
}

} // namespace cle
