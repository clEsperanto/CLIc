

#include "cleBinaryNotKernel.hpp"

namespace cle
{

BinaryNotKernel::BinaryNotKernel (const ProcessorPointer &device) : Operation (device, 2)
{
    std::string cl_header = {
#include "cle_binary_not.h"
    };
    this->SetSource ("binary_not", cl_header);
}

void
BinaryNotKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
BinaryNotKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
