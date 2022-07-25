#include "cleAddImagesWeightedKernel.hpp"

namespace cle
{

AddImagesWeightedKernel::AddImagesWeightedKernel (const ProcessorPointer &device) : Operation (device, 5)
{
    std::string cl_header = {
#include "cle_add_images_weighted.h"
    };
    this->SetSource ("add_images_weighted", cl_header);
}

void
AddImagesWeightedKernel::SetInput1 (const Image &object)
{
    this->AddParameter ("src0", object);
}

void
AddImagesWeightedKernel::SetInput2 (const Image &object)
{
    this->AddParameter ("src1", object);
}

void
AddImagesWeightedKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
AddImagesWeightedKernel::SetFactor1 (const float &value)
{
    this->AddParameter ("scalar0", value);
}

void
AddImagesWeightedKernel::SetFactor2 (const float &value)
{
    this->AddParameter ("scalar1", value);
}

} // namespace cle
