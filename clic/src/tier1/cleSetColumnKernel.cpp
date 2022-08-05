

#include "cleSetColumnKernel.hpp"

namespace cle
{

SetColumnKernel::SetColumnKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{
  std::string cl_header = {
#include "cle_set_column.h"
  };
  this->SetSource("set_column", cl_header);
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
