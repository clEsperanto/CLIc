

#include "cleFlagExistingLabelsKernel.hpp"
#include "cleSetKernel.hpp"
#include "cle_flag_existing_labels.h"

namespace cle
{

FlagExistingLabelsKernel::FlagExistingLabelsKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  this->SetSource("flag_existing_labels", oclKernel::flag_existing_labels);
}

auto
FlagExistingLabelsKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
  this->SetRange(this->GetImage("src")->Shape());
}

auto
FlagExistingLabelsKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
FlagExistingLabelsKernel::Execute() -> void
{
  this->GetImage("dst")->Fill(0);
  this->Operation::Execute();
}

} // namespace cle
