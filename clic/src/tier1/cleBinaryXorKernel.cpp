

#include "cleBinaryXorKernel.hpp"

namespace cle
{

BinaryXorKernel::BinaryXorKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_binary_xor.h"
    };
    this->SetSource ("cle_binary_xor", cl_header);
}

void
BinaryXorKernel::SetInput1 (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
BinaryXorKernel::SetInput2 (const Image &object)
{
    this->AddParameter ("src1", object);
}

void
BinaryXorKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
