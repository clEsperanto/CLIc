
#include "cleAddImageAndScalarKernel.hpp"

namespace cle
{

AddImageAndScalarKernel::AddImageAndScalarKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_add_image_and_scalar.h"
    };
    this->SetSource ("add_image_and_scalar", cl_header);
}

void
AddImageAndScalarKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
AddImageAndScalarKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
AddImageAndScalarKernel::SetScalar (const float &value)
{
    this->AddParameter ("scalar", value);
}

} // namespace cle
