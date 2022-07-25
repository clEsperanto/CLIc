

#include "cleErodeSphereKernel.hpp"

namespace cle
{

ErodeSphereKernel::ErodeSphereKernel (const ProcessorPointer &device) : Operation (device, 2)
{
    std::string cl_header = {
#include "cle_erode_sphere.h"
    };
    this->SetSource ("cle_erode_sphere", cl_header);
}

void
ErodeSphereKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
ErodeSphereKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
