

#include "cleSumReductionXKernel.hpp"

namespace cle
{

SumReductionXKernel::SumReductionXKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_sum_reduction_x.h"
    };
    this->SetSource ("cle_sum_reduction_x", cl_header);
}

void
SumReductionXKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
SumReductionXKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
SumReductionXKernel::SetBlocksize (const int &size)
{
    this->AddParameter ("index", size);
}

} // namespace cle
