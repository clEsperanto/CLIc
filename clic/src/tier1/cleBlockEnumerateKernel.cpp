

#include "cleBlockEnumerateKernel.hpp"
#include "cle_block_enumerate.h"

namespace cle
{

BlockEnumerateKernel::BlockEnumerateKernel(const ProcessorPointer & device)
  : Operation(device, 4)
{
  this->SetSource("block_enumerate", oclKernel::block_enumerate);
}

auto
BlockEnumerateKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
BlockEnumerateKernel::SetInputSums(const Image & object) -> void
{
  this->AddParameter("src1", object);
  this->SetRange(this->GetImage("src1")->Shape());
}

auto
BlockEnumerateKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
BlockEnumerateKernel::SetBlocksize(const int & value) -> void
{
  this->AddParameter("index", value);
}

} // namespace cle
