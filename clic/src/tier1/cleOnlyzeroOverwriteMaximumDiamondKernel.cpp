

#include "cleOnlyzeroOverwriteMaximumDiamondKernel.hpp"

namespace cle
{

OnlyzeroOverwriteMaximumDiamondKernel::OnlyzeroOverwriteMaximumDiamondKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_onlyzero_overwrite_maximum_diamond.h"
  };
  this->SetSource("onlyzero_overwrite_maximum_diamond", cl_header);
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
  this->SetRange("dst1");
}

} // namespace cle
