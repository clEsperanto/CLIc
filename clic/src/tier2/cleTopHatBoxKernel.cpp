
#include "cleTopHatBoxKernel.hpp"

#include "cleAddImagesWeightedKernel.hpp"
#include "cleMaximumBoxKernel.hpp"
#include "cleMemory.hpp"
#include "cleMinimumBoxKernel.hpp"

namespace cle
{

TopHatBoxKernel::TopHatBoxKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{}

auto
TopHatBoxKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
TopHatBoxKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
TopHatBoxKernel::SetRadius(const int & radius_x, const int & radius_y, const int & radius_z) -> void
{
  this->radius_ = { radius_x, radius_y, radius_z };
}

auto
TopHatBoxKernel::Execute() -> void
{
  auto src = this->GetImage("src");
  auto dst = this->GetImage("dst");

  auto temp1 = Memory::AllocateObject(this->Device(), src->Shape(), dst->BitType().Get(), dst->Memory());
  auto temp2 = Memory::AllocateObject(this->Device(), src->Shape(), dst->BitType().Get(), dst->Memory());

  MinimumBoxKernel minimum(this->Device());
  minimum.SetInput(*src);
  minimum.SetOutput(temp1);
  minimum.SetRadius(this->radius_[0], this->radius_[1], this->radius_[2]);
  minimum.Execute();

  MaximumBoxKernel maximum(this->Device());
  maximum.SetInput(temp1);
  maximum.SetOutput(temp2);
  maximum.SetRadius(this->radius_[0], this->radius_[1], this->radius_[2]);
  maximum.Execute();

  AddImagesWeightedKernel add(this->Device());
  add.SetInput1(*src);
  add.SetInput2(temp2);
  add.SetOutput(*dst);
  add.SetFactor1(1);
  add.SetFactor2(-1);
  add.Execute();
}

} // namespace cle
