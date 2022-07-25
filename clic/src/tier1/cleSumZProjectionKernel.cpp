

#include "cleSumZProjectionKernel.hpp"

namespace cle
{

SumZProjectionKernel::SumZProjectionKernel (const ProcessorPointer &device) : Operation (device, 2)
{
    std::string cl_header = {
#include "cle_sum_z_projection.h"
    };
    this->SetSource ("cle_sum_z_projection", cl_header);
}

void
SumZProjectionKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
SumZProjectionKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
