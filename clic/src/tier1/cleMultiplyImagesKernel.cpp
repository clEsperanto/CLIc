

#include "cleMultiplyImagesKernel.hpp"

namespace cle
{

MultiplyImagesKernel::MultiplyImagesKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_multiply_images.h"
    };
    this->SetSource ("multiply_images", cl_header);
}

void
MultiplyImagesKernel::SetInput1 (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
MultiplyImagesKernel::SetInput2 (const Image &object)
{
    this->AddParameter ("src1", object);
}

void
MultiplyImagesKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

} // namespace cle
