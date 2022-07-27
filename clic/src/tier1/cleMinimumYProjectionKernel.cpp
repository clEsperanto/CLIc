

#include "cleMinimumYProjectionKernel.hpp"

namespace cle
{

MinimumYProjectionKernel::MinimumYProjectionKernel (const ProcessorPointer &device) : Operation (device, 2)
{
    std::string cl_header = {
#include "cle_minimum_y_projection.h"
    };
    this->SetSource ("minimum_y_projection", cl_header);
}

void
MinimumYProjectionKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
MinimumYProjectionKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
