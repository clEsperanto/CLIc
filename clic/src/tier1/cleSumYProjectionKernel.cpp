

#include "cleSumYProjectionKernel.hpp"

namespace cle
{

SumYProjectionKernel::SumYProjectionKernel (const ProcessorPointer &device) : Operation (device, 2)
{
    std::string cl_header = {
#include "cle_sum_y_projection.h"
    };
    this->SetSource ("sum_y_projection", cl_header);
}

void
SumYProjectionKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
SumYProjectionKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
