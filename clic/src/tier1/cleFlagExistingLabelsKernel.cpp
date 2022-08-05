

#include "cleFlagExistingLabelsKernel.hpp"
#include "cleSetKernel.hpp"

namespace cle
{

FlagExistingLabelsKernel::FlagExistingLabelsKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{
  std::string cl_header = {
#include "cle_flag_existing_labels.h"
  };
  this->SetSource("flag_existing_labels", cl_header);
}

auto
FlagExistingLabelsKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
  this->SetRange("src");
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
