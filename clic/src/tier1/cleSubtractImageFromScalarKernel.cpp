
#include "cleSubtractImageFromScalarKernel.hpp"

namespace cle
{

SubtractImageFromScalarKernel::SubtractImageFromScalarKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_subtract_image_from_scalar.h"
    };
    this->SetSource ("cle_subtract_image_from_scalar", cl_header);
}

void
SubtractImageFromScalarKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
SubtractImageFromScalarKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
SubtractImageFromScalarKernel::SetScalar (const float &value)
{
    this->AddParameter ("scalar", value);
}

} // namespace cle
