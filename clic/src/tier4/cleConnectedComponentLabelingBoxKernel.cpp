
#include "cleCloseIndexGapsInLabelMapKernel.hpp"

#include "cleConnectedComponentLabelingBoxKernel.hpp"
#include "cleCopyKernel.hpp"
#include "cleMemory.hpp"
#include "cleNonzeroMinimumBoxKernel.hpp"
#include "cleSetKernel.hpp"
#include "cleSetNonzeroPixelsToPixelindexKernel.hpp"

namespace cle
{

ConnectedComponentLabelingBoxKernel::ConnectedComponentLabelingBoxKernel(const ProcessorPointer & device)
  : Operation(device, 2)
{}

auto
ConnectedComponentLabelingBoxKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
ConnectedComponentLabelingBoxKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
ConnectedComponentLabelingBoxKernel::Execute() -> void
{
  auto src = this->GetImage("src");
  auto dst = this->GetImage("dst");

  auto temp1 = Memory::AllocateMemory(this->GetDevice(), dst->Shape(), dst->GetDataType(), dst->GetMemoryType());
  auto temp2 = Memory::AllocateMemory(this->GetDevice(), dst->Shape(), dst->GetDataType(), dst->GetMemoryType());
  auto temp3 = Memory::AllocateMemory(this->GetDevice(), dst->Shape(), dst->GetDataType(), dst->GetMemoryType());

  SetNonzeroPixelsToPixelindexKernel set_nonzero_to_index_kernel(this->GetDevice());
  set_nonzero_to_index_kernel.SetInput(*src);
  set_nonzero_to_index_kernel.SetOutput(temp1);
  set_nonzero_to_index_kernel.SetOffset(1);
  set_nonzero_to_index_kernel.Execute();

  SetKernel set_init_kernel(this->GetDevice());
  set_init_kernel.SetInput(temp2);
  set_init_kernel.SetValue(0);
  set_init_kernel.Execute();

  std::array<size_t, 3> flag_dim = { 1, 1, 2 };
  std::vector<float>    arr = { 0, 0 };
  auto                  flag = Memory::AllocateMemory(this->GetDevice(), flag_dim);
  Memory::WriteObject(flag, arr);

  std::vector<float>      flag_value = { 1, 1 };
  int                     iteration_count = 0;
  NonzeroMinimumBoxKernel nonzero_minimum_kernel(this->GetDevice());
  SetKernel               set_flag_kernel(this->GetDevice());
  while (flag_value[0] > 0)
  {
    nonzero_minimum_kernel.SetOutputFlag(flag);
    if (iteration_count % 2 == 0)
    {
      nonzero_minimum_kernel.SetInput(temp1);
      nonzero_minimum_kernel.SetOutput(temp2);
    }
    else
    {
      nonzero_minimum_kernel.SetInput(temp2);
      nonzero_minimum_kernel.SetOutput(temp1);
    }
    nonzero_minimum_kernel.Execute();

    flag_value = Memory::ReadObject<float>(flag);
    set_flag_kernel.SetInput(flag);
    set_flag_kernel.SetValue(0);
    set_flag_kernel.Execute();

    iteration_count++;
  }

  if (iteration_count % 2 == 0)
  {
    temp1.CopyDataTo(temp3);
  }
  else
  {
    temp2.CopyDataTo(temp3);
  }

  CloseIndexGapsInLabelMapKernel close_gaps_kernel(this->GetDevice());
  close_gaps_kernel.SetInput(temp3);
  close_gaps_kernel.SetOutput(*dst);
  close_gaps_kernel.SetBlockSize(4096);
  close_gaps_kernel.Execute();
}

} // namespace cle
