
#include "cleMaskedVoronoiLabelingKernel.hpp"

#include "cleAddImageAndScalarKernel.hpp"
#include "cleAddImagesWeightedKernel.hpp"
#include "cleConnectedComponentLabelingBoxKernel.hpp"
#include "cleHistogramKernel.hpp"
#include "cleMaskKernel.hpp"
#include "cleMemory.hpp"
#include "cleOnlyzeroOverwriteMaximumBoxKernel.hpp"
#include "cleOnlyzeroOverwriteMaximumDiamondKernel.hpp"
#include "cleSetKernel.hpp"

namespace cle
{

MaskedVoronoiLabelingKernel::MaskedVoronoiLabelingKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{}

auto
MaskedVoronoiLabelingKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src0", object);
}

auto
MaskedVoronoiLabelingKernel::SetMask(const Image & object) -> void
{
  this->AddParameter("src1", object);
}

auto
MaskedVoronoiLabelingKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
MaskedVoronoiLabelingKernel::Execute() -> void
{
  auto src = this->GetImage("src0");
  auto msk = this->GetImage("src1");
  auto dst = this->GetImage("dst");

  auto flup = Memory::AllocateMemory(this->Device(), src->Shape(), FLOAT, src->Object());
  auto flip = Memory::AllocateMemory(this->Device(), src->Shape(), FLOAT, src->Object());
  auto flop = Memory::AllocateMemory(this->Device(), src->Shape(), FLOAT, src->Object());
  auto flag = Memory::AllocateMemory(this->Device(), { 1, 1, 1 });
  flag.Fill(1);

  AddImageAndScalarKernel subtractOne(this->Device());
  subtractOne.SetInput(*msk);
  subtractOne.SetOutput(flup);
  subtractOne.SetScalar(-1);
  subtractOne.Execute();

  ConnectedComponentLabelingBoxKernel labeling(this->Device());
  labeling.SetInput(*src);
  labeling.SetOutput(flop);
  labeling.Execute();

  AddImagesWeightedKernel add(this->Device());
  add.SetInput1(flup);
  add.SetInput2(flop);
  add.SetOutput(flip);
  add.SetFactor1(1);
  add.SetFactor2(1);
  add.Execute();

  int   iteration_count = 0;
  float flag_value = 1;

  OnlyzeroOverwriteMaximumBoxKernel boxMaximum(this->Device());
  OnlyzeroOverwriteMaximumBoxKernel diamondMaximum(this->Device());
  while (flag_value > 0)
  {
    if ((iteration_count % 2) == 0)
    {
      boxMaximum.SetInput(flip);
      boxMaximum.SetOutput1(flag);
      boxMaximum.SetOutput2(flop);
      boxMaximum.Execute();
    }
    else
    {
      diamondMaximum.SetInput(flop);
      diamondMaximum.SetOutput1(flag);
      diamondMaximum.SetOutput2(flip);
      diamondMaximum.Execute();
    }
    flag_value = Memory::ReadObject<float>(flag).front();
    flag.Fill(0);
    iteration_count++;
  }

  MaskKernel mask(this->Device());
  if ((iteration_count % 2) == 0)
  {
    mask.SetInput(flip);
  }
  else
  {
    mask.SetInput(flop);
  }
  mask.SetMask(*msk);
  mask.SetOutput(*dst);
  mask.Execute();
}

} // namespace cle
