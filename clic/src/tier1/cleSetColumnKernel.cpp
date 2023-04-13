

#include "cleSetColumnKernel.hpp"
#include "cle_set_column.h"

namespace cle
{

SetColumnKernel::SetColumnKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  this->SetSource("set_column", oclKernel::set_column);
}

auto
SetColumnKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
SetColumnKernel::SetColumn(const int & index) -> void
{
  this->AddParameter("index", index);
}

auto
SetColumnKernel::SetValue(const float & value) -> void
{
  this->AddParameter("scalar", value);
}

} // namespace cle
