
#include "cleMaximumOfAllPixelsKernel.hpp"
#include "cleMaximumXProjectionKernel.hpp"
#include "cleMaximumYProjectionKernel.hpp"
#include "cleMaximumZProjectionKernel.hpp"
#include "cleMemory.hpp"

namespace cle
{

MaximumOfAllPixelsKernel::MaximumOfAllPixelsKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{}

auto
MaximumOfAllPixelsKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
MaximumOfAllPixelsKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
MaximumOfAllPixelsKernel::Execute() -> void
{
  auto                  src = this->GetImage("src");
  auto                  dst = this->GetImage("dst");
  std::array<size_t, 3> dim = src->Shape();

  if (dim[2] > 1)
  {
    dim[2] = 1;
    auto temp = Memory::AllocateMemory(this->GetDevice(), dim, dst->GetDataType(), dst->GetMemoryType());

    MaximumZProjectionKernel kernel(this->GetDevice());
    kernel.SetInput(*src);
    kernel.SetOutput(temp);
    kernel.Execute();
    src = std::make_shared<Image>(temp);
  }

  if (dim[1] > 1)
  {
    dim[1] = 1;
    auto temp = Memory::AllocateMemory(this->GetDevice(), dim, dst->GetDataType(), dst->GetMemoryType());

    MaximumYProjectionKernel kernel(this->GetDevice());
    kernel.SetInput(*src);
    kernel.SetOutput(temp);
    kernel.Execute();
    src = std::make_shared<Image>(temp);
  }

  MaximumXProjectionKernel kernel(this->GetDevice());
  kernel.SetInput(*src);
  kernel.SetOutput(*dst);
  kernel.Execute();
}

} // namespace cle
