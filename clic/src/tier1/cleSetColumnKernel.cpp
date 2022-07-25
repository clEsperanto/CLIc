

#include "cleSetColumnKernel.hpp"

namespace cle
{

SetColumnKernel::SetColumnKernel (const ProcessorPointer &device) : Operation (device, 3)
{
    std::string cl_header = {
#include "cle_set_column.h"
    };
    this->SetSource ("cle_set_column", cl_header);
}

void
SetColumnKernel::SetInput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
SetColumnKernel::SetColumn (const int &index)
{
    this->AddParameter ("index", index);
}

void
SetColumnKernel::SetValue (const float &value)
{
    this->AddParameter ("scalar", value);
}

} // namespace cle
