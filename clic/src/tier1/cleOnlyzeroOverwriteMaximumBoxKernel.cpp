

#include "cleOnlyzeroOverwriteMaximumBoxKernel.hpp"

namespace cle
{

OnlyzeroOverwriteMaximumBoxKernel::OnlyzeroOverwriteMaximumBoxKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_onlyzero_overwrite_maximum_box.h"
    };
    this->SetSource ("cle_onlyzero_overwrite_maximum_box", cl_header);
}

void
OnlyzeroOverwriteMaximumBoxKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
OnlyzeroOverwriteMaximumBoxKernel::SetOutput1 (const Image &object)
{
    this->AddParameter ("dst0", object);
}

void
OnlyzeroOverwriteMaximumBoxKernel::SetOutput2 (const Image &object)
{
    this->AddParameter ("dst1", object);
    this->SetRange ("dst1");
}

} // namespace cle
