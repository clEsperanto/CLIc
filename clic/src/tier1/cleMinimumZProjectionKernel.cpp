

#include "cleMinimumZProjectionKernel.hpp"

namespace cle
{

MinimumZProjectionKernel::MinimumZProjectionKernel (const ProcessorPointer &device) : Operation (device, 2)
{
    std::string cl_header = {
#include "cle_minimum_z_projection.h"
    };
    this->SetSource ("minimum_z_projection", cl_header);
}

void
MinimumZProjectionKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
MinimumZProjectionKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
