

#include "cleEqualKernel.hpp"

namespace cle
{

EqualKernel::EqualKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_equal.h"
    };
    this->SetSource ("cle_equal", cl_header);
}

void
EqualKernel::SetInput1 (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
EqualKernel::SetInput2 (const Image &object)
{
    this->AddParameter ("src1", object);
}

void
EqualKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
