

#include "cleNonzeroMinimumBoxKernel.hpp"

namespace cle
{

NonzeroMinimumBoxKernel::NonzeroMinimumBoxKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_nonzero_minimum_box.h"
    };
    this->SetSource ("nonzero_minimum_box", cl_header);
}

void
NonzeroMinimumBoxKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
NonzeroMinimumBoxKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst1", object);
    this->SetRange ("dst1");
}

void
NonzeroMinimumBoxKernel::SetOutputFlag (const Image &object)
{
    this->AddParameter ("dst0", object);
}

} // namespace cle
