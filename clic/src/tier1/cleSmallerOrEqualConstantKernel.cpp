

#include "cleSmallerOrEqualConstantKernel.hpp"

namespace cle
{

SmallerOrEqualConstantKernel::SmallerOrEqualConstantKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_smaller_or_equal_constant.h"
    };
    this->SetSource ("cle_smaller_or_equal_constant", cl_header);
}

void
SmallerOrEqualConstantKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
SmallerOrEqualConstantKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
SmallerOrEqualConstantKernel::SetConstant (const float &value)
{
    this->AddParameter ("scalar", value);
}

} // namespace cle
