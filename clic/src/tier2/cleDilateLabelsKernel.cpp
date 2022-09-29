
#include "cleDilateLabelsKernel.hpp"

#include "cleCopyKernel.hpp"
#include "cleMemory.hpp"
#include "cleOnlyzeroOverwriteMaximumBoxKernel.hpp"
#include "cleOnlyzeroOverwriteMaximumDiamondKernel.hpp"
#include "cleSetKernel.hpp"
#include "cleTypes.hpp"

namespace cle
{

DilateLabelsKernel::DilateLabelsKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{}

auto
DilateLabelsKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
DilateLabelsKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
DilateLabelsKernel::SetRadius(const int & radius) -> void
{
  this->radius_ = radius;
}

auto
DilateLabelsKernel::Execute() -> void
{
  // // get I/O pointers
  auto src = this->GetImage("src");
  auto dst = this->GetImage("dst");

  auto flip = Memory::AllocateMemory(this->GetDevice(), dst->Shape(), dst->GetDataType(), dst->GetMemoryType());
  auto flop = Memory::AllocateMemory(this->GetDevice(), dst->Shape(), dst->GetDataType(), dst->GetMemoryType());
  auto flag = Memory::AllocateMemory(this->GetDevice(), { 1, 1, 1 }, FLOAT, BUFFER);
  flag.Fill(1);

  CopyKernel copy(this->GetDevice());
  copy.SetInput(*src);
  copy.SetOutput(flip);
  copy.Execute();

  float flag_value = 1;
  int   iteration_count = 0;
  while (flag_value > 0 && iteration_count < this->radius_)
  {
    if ((iteration_count % 2) == 0)
    {
      OnlyzeroOverwriteMaximumBoxKernel boxMaximum(this->GetDevice());
      boxMaximum.SetInput(flip);
      boxMaximum.SetOutput1(flag);
      boxMaximum.SetOutput2(flop);
      boxMaximum.Execute();
    }
    else
    {
      OnlyzeroOverwriteMaximumBoxKernel diamondMaximum(this->GetDevice());
      diamondMaximum.SetInput(flop);
      diamondMaximum.SetOutput1(flag);
      diamondMaximum.SetOutput2(flip);
      diamondMaximum.Execute();
    }
    flag_value = Memory::ReadObject<float>(flag).front();
    flag.Fill(0);
    iteration_count++;
  }
  if ((iteration_count % 2) == 0)
  {
    flip.CopyDataTo(*dst);
  }
  else
  {
    flop.CopyDataTo(*dst);
  }
}

} // namespace cle
