

#include "cleSmallerConstantKernel.hpp"

namespace cle
{

SmallerConstantKernel::SmallerConstantKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_smaller_constant.h"
    };
    this->SetSource ("cle_smaller_constant", cl_header);
}

void
SmallerConstantKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
SmallerConstantKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
SmallerConstantKernel::SetConstant (const float &value)
{
    this->AddParameter ("scalar", value);
}

} // namespace cle
