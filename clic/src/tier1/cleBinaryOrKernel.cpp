

#include "cleBinaryOrKernel.hpp"

namespace cle
{

BinaryOrKernel::BinaryOrKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_binary_or.h"
    };
    this->SetSource ("binary_or", cl_header);
}

void
BinaryOrKernel::SetInput1 (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
BinaryOrKernel::SetInput2 (const Image &object)
{
    this->AddParameter ("src1", object);
}

void
BinaryOrKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
