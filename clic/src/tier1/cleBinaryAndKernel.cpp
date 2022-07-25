

#include "cleBinaryAndKernel.hpp"

namespace cle
{

BinaryAndKernel::BinaryAndKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_binary_and.h"
    };
    this->SetSource ("binary_and", cl_header);
}

void
BinaryAndKernel::SetInput1 (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
BinaryAndKernel::SetInput2 (const Image &object)
{
    this->AddParameter ("src1", object);
}

void
BinaryAndKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
