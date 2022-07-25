

#include "cleMinimumXProjectionKernel.hpp"

namespace cle
{

MinimumXProjectionKernel::MinimumXProjectionKernel (const ProcessorPointer &device) : Operation (device, 2)
{
    std::string cl_header = {
#include "cle_minimum_x_projection.h"
    };
    this->SetSource ("cle_minimum_x_projection", cl_header);
}

void
MinimumXProjectionKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
MinimumXProjectionKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
