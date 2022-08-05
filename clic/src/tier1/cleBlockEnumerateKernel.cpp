

#include "cleBlockEnumerateKernel.hpp"

namespace cle
{

BlockEnumerateKernel::BlockEnumerateKernel(const ProcessorPointer & device)
  : Operation(device, 4)
{
  std::string cl_header = {
#include "cle_block_enumerate.h"
  };
  this->SetSource("block_enumerate", cl_header);
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
  this->SetRange("src1");
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
