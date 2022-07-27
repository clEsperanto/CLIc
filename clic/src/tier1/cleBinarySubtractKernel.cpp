

#include "cleBinarySubtractKernel.hpp"

namespace cle
{

BinarySubtractKernel::BinarySubtractKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_binary_subtract.h"
    };
    this->SetSource ("binary_subtract", cl_header);
}

void
BinarySubtractKernel::SetInput1 (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
BinarySubtractKernel::SetInput2 (const Image &object)
{
    this->AddParameter ("src1", object);
}

void
BinarySubtractKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
