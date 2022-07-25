

#include "cleSumXProjectionKernel.hpp"

namespace cle
{

SumXProjectionKernel::SumXProjectionKernel (const ProcessorPointer &device) : Operation (device, 2)
{
    std::string cl_header = {
#include "cle_sum_x_projection.h"
    };
    this->SetSource ("cle_sum_x_projection", cl_header);
}

void
SumXProjectionKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
SumXProjectionKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
