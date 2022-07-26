

#include "cleFlagExistingLabelsKernel.hpp"
#include "cleSetKernel.hpp"

namespace cle
{

FlagExistingLabelsKernel::FlagExistingLabelsKernel (const ProcessorPointer &device) : Operation (device, 2)
{
    std::string cl_header = {
#include "cle_flag_existing_labels.h"
    };
    this->SetSource ("cle_flag_existing_labels", cl_header);
}

void
FlagExistingLabelsKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
FlagExistingLabelsKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
FlagExistingLabelsKernel::Execute ()
{
    this->GetImage ("dst")->Fill (0);
    this->Operation::Execute ();
}

} // namespace cle
