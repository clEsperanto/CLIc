

#include "cleOnlyzeroOverwriteMaximumBoxKernel.hpp"
#include "cle_onlyzero_overwrite_maximum_box.h"

namespace cle
{

OnlyzeroOverwriteMaximumBoxKernel::OnlyzeroOverwriteMaximumBoxKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("onlyzero_overwrite_maximum_box", oclKernel::onlyzero_overwrite_maximum_box);
}

auto
OnlyzeroOverwriteMaximumBoxKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
OnlyzeroOverwriteMaximumBoxKernel::SetOutput1(const Image & object) -> void
{
  this->AddParameter("dst0", object);
}

auto
OnlyzeroOverwriteMaximumBoxKernel::SetOutput2(const Image & object) -> void
{
  this->AddParameter("dst1", object);
  this->SetRange(this->GetImage("dst1")->Shape());
}

} // namespace cle
