

#include "cleSumOfAllPixelsKernel.hpp"
#include "cleMemory.hpp"
#include "cleSumXProjectionKernel.hpp"
#include "cleSumYProjectionKernel.hpp"
#include "cleSumZProjectionKernel.hpp"

namespace cle
{

SumOfAllPixelsKernel::SumOfAllPixelsKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{}

auto
SumOfAllPixelsKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
SumOfAllPixelsKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
SumOfAllPixelsKernel::Execute() -> void
{
  auto                  src = this->GetImage("src");
  auto                  dst = this->GetImage("dst");
  std::array<size_t, 3> dim = src->Shape();

  if (dim[2] > 1)
  {
    dim[2] = 1;
    auto temp = Memory::AllocateObject(this->Device(), dim, dst->BitType().Get(), dst->Memory());

    SumZProjectionKernel kernel(this->Device());
    kernel.SetInput(*src);
    kernel.SetOutput(temp);
    kernel.Execute();
    src = std::make_shared<Image>(temp);
  }

  if (dim[1] > 1)
  {
    dim[1] = 1;
    auto temp = Memory::AllocateObject(this->Device(), dim, dst->BitType().Get(), dst->Memory());

    SumYProjectionKernel kernel(this->Device());
    kernel.SetInput(*src);
    kernel.SetOutput(temp);
    kernel.Execute();
    src = std::make_shared<Image>(temp);
  }

  SumXProjectionKernel kernel(this->Device());
  kernel.SetInput(*src);
  kernel.SetOutput(*dst);
  kernel.Execute();
}

} // namespace cle
