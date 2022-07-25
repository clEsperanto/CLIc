

#include "cleOnlyzeroOverwriteMaximumDiamondKernel.hpp"

namespace cle
{

OnlyzeroOverwriteMaximumDiamondKernel::OnlyzeroOverwriteMaximumDiamondKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_onlyzero_overwrite_maximum_diamond.h"
    };
    this->SetSource ("cle_onlyzero_overwrite_maximum_diamond", cl_header);
}

void
OnlyzeroOverwriteMaximumDiamondKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
OnlyzeroOverwriteMaximumDiamondKernel::SetOutput1 (const Image &object)
{
    this->AddParameter ("dst0", object);
}

void
OnlyzeroOverwriteMaximumDiamondKernel::SetOutput2 (const Image &object)
{
    this->AddParameter ("dst1", object);
    this->SetRange ("dst1");
}

} // namespace cle
