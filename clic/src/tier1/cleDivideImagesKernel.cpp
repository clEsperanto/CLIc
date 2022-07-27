

#include "cleDivideImagesKernel.hpp"

namespace cle
{

DivideImagesKernel::DivideImagesKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_divide_images.h"
    };
    this->SetSource ("divide_images", cl_header);
}

void
DivideImagesKernel::SetInput1 (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
DivideImagesKernel::SetInput2 (const Image &object)
{
    this->AddParameter ("src1", object);
}

void
DivideImagesKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
