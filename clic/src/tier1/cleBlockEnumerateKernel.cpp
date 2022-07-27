

#include "cleBlockEnumerateKernel.hpp"

namespace cle
{

BlockEnumerateKernel::BlockEnumerateKernel (const ProcessorPointer &device) : Operation (device, 4)
{
    std::string cl_header = {
#include "cle_block_enumerate.h"
    };
    this->SetSource ("block_enumerate", cl_header);
}

void
BlockEnumerateKernel::SetInput (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
BlockEnumerateKernel::SetInputSums (const Image &object)
{
    this->AddParameter ("src1", object);
    this->SetRange ("src1");
}

void
BlockEnumerateKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
BlockEnumerateKernel::SetBlocksize (const int &value)
{
    this->AddParameter ("index", value);
}

} // namespace cle
