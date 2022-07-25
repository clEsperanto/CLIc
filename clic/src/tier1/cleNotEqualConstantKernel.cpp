

#include "cleNotEqualConstantKernel.hpp"

namespace cle
{

NotEqualConstantKernel::NotEqualConstantKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_not_equal_constant.h"
    };
    this->SetSource ("cle_not_equal_constant", cl_header);
}

void
NotEqualConstantKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
NotEqualConstantKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
NotEqualConstantKernel::SetScalar (const float &value)
{
    this->AddParameter ("scalar", value);
}

} // namespace cle
