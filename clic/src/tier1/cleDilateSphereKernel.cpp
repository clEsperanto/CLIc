

#include "cleDilateSphereKernel.hpp"

namespace cle
{

DilateSphereKernel::DilateSphereKernel (const ProcessorPointer &device) : Operation (device, 2)
{
    std::string cl_header = {
#include "cle_dilate_sphere.h"
    };
    this->SetSource ("cle_dilate_sphere", cl_header);
}

void
DilateSphereKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
DilateSphereKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
