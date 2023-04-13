

#include "cleOnlyzeroOverwriteMaximumDiamondKernel.hpp"
#include "cle_onlyzero_overwrite_maximum_diamond.h"

namespace cle
{

OnlyzeroOverwriteMaximumDiamondKernel::OnlyzeroOverwriteMaximumDiamondKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("onlyzero_overwrite_maximum_diamond", oclKernel::onlyzero_overwrite_maximum_diamond);
}

auto
OnlyzeroOverwriteMaximumDiamondKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
OnlyzeroOverwriteMaximumDiamondKernel::SetOutput1(const Image & object) -> void
{
  this->AddParameter("dst0", object);
}

auto
OnlyzeroOverwriteMaximumDiamondKernel::SetOutput2(const Image & object) -> void
{
  this->AddParameter("dst1", object);
  this->SetRange(this->GetImage("dst1")->Shape());
}

} // namespace cle
