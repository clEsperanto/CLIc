

#include "cleNotEqualKernel.hpp"

namespace cle
{

NotEqualKernel::NotEqualKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_not_equal.h"
    };
    this->SetSource ("cle_not_equal", cl_header);
}

void
NotEqualKernel::SetInput1 (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
NotEqualKernel::SetInput2 (const Image &object)
{
    this->AddParameter ("src1", object);
}

void
NotEqualKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
