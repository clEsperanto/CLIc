

#include "cleOnlyzeroOverwriteMaximumBoxKernel.hpp"

namespace cle
{

OnlyzeroOverwriteMaximumBoxKernel::OnlyzeroOverwriteMaximumBoxKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_onlyzero_overwrite_maximum_box.h"
  };
  this->SetSource("onlyzero_overwrite_maximum_box", cl_header);
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
  this->SetRange("dst1");
}

} // namespace cle
