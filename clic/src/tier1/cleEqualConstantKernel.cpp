

#include "cleEqualConstantKernel.hpp"

namespace cle
{

EqualConstantKernel::EqualConstantKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_equal_constant.h"
    };
    this->SetSource ("equal_constant", cl_header);
}

void
EqualConstantKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
EqualConstantKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
EqualConstantKernel::SetScalar (const float &value)
{
    this->AddParameter ("scalar", value);
}

} // namespace cle
