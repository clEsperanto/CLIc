

#include "cleGreaterOrEqualConstantKernel.hpp"

namespace cle
{

GreaterOrEqualConstantKernel::GreaterOrEqualConstantKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_greater_or_equal_constant.h"
    };
    this->SetSource ("greater_or_equal_constant", cl_header);
}

void
GreaterOrEqualConstantKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
GreaterOrEqualConstantKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
GreaterOrEqualConstantKernel::SetScalar (const float &value)
{
    this->AddParameter ("scalar", value);
}

} // namespace cle
