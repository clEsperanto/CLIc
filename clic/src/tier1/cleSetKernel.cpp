

#include "cleSetKernel.hpp"

namespace cle
{

SetKernel::SetKernel (const ProcessorPointer &device) : Operation (device, 2)
{
    std::string cl_header = {
#include "cle_set.h"
    };
    this->SetSource ("cle_set", cl_header);
}

void
SetKernel::SetInput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
SetKernel::SetValue (const float &value)
{
    this->AddParameter ("scalar", value);
}

} // namespace cle
