

#include "cleCloseIndexGapsInLabelMapKernel.hpp"

#include "cleBlockEnumerateKernel.hpp"
#include "cleFlagExistingLabelsKernel.hpp"
#include "cleMaximumOfAllPixelsKernel.hpp"
#include "cleMemory.hpp"
#include "cleReplaceIntensitiesKernel.hpp"
#include "cleSetColumnKernel.hpp"
#include "cleSetKernel.hpp"
#include "cleSumReductionXKernel.hpp"


namespace cle
{

CloseIndexGapsInLabelMapKernel::CloseIndexGapsInLabelMapKernel(const ProcessorPointer & device)
  : Operation(device, 3)
{}

auto
CloseIndexGapsInLabelMapKernel::SetInput(const Image & object) -> void
{
  this->AddParameter("src", object);
}

auto
CloseIndexGapsInLabelMapKernel::SetOutput(const Image & object) -> void
{
  this->AddParameter("dst", object);
}

auto
CloseIndexGapsInLabelMapKernel::SetBlockSize(const int & size) -> void
{
  this->block_size_ = size;
}

auto
CloseIndexGapsInLabelMapKernel::Execute() -> void
{
  auto src = this->GetImage("src");
  auto dst = this->GetImage("dst");

  auto max_value_buffer = Memory::AllocateMemory(this->GetDevice());

  MaximumOfAllPixelsKernel max_of_pixel_kernel(this->GetDevice());
  max_of_pixel_kernel.SetInput(*src);
  max_of_pixel_kernel.SetOutput(max_value_buffer);
  max_of_pixel_kernel.Execute();

  float  max_value = Memory::ReadObject<float>(max_value_buffer).front();
  size_t nb_indices = static_cast<size_t>(max_value) + 1;

  std::array<size_t, 3> indices_dim = { nb_indices, 1, 1 };
  auto                  flagged_indices = Memory::AllocateMemory(this->GetDevice(), indices_dim);

  FlagExistingLabelsKernel flag_labels_kernel(this->GetDevice());
  flag_labels_kernel.SetInput(*src);
  flag_labels_kernel.SetOutput(flagged_indices);
  flag_labels_kernel.Execute();

  SetColumnKernel set_column_kernel(this->GetDevice());
  set_column_kernel.SetInput(flagged_indices);
  set_column_kernel.SetColumn(0);
  set_column_kernel.SetValue(0);
  set_column_kernel.Execute();

  size_t                nb_sums = static_cast<size_t>(nb_indices / this->block_size_) + 1;
  std::array<size_t, 3> sums_dim = { nb_sums, 1, 1 };
  auto                  block_sums = Memory::AllocateMemory(this->GetDevice(), sums_dim);

  SumReductionXKernel sum_reduction_x_kernel(this->GetDevice());
  sum_reduction_x_kernel.SetInput(flagged_indices);
  sum_reduction_x_kernel.SetOutput(block_sums);
  sum_reduction_x_kernel.SetBlocksize(this->block_size_);
  sum_reduction_x_kernel.Execute();

  std::array<size_t, 3> new_indices_dim = { nb_indices, 1, 1 };
  auto                  new_indices = Memory::AllocateMemory(this->GetDevice(), new_indices_dim);

  BlockEnumerateKernel block_enumerate_kernel(this->GetDevice());
  block_enumerate_kernel.SetInput(flagged_indices);
  block_enumerate_kernel.SetInputSums(block_sums);
  block_enumerate_kernel.SetOutput(new_indices);
  block_enumerate_kernel.SetBlocksize(this->block_size_);
  block_enumerate_kernel.Execute();

  ReplaceIntensitiesKernel replace_intensities_kernel(this->GetDevice());
  replace_intensities_kernel.SetInput(*src);
  replace_intensities_kernel.SetOutput(*dst);
  replace_intensities_kernel.SetMap(new_indices);
  replace_intensities_kernel.Execute();
}

} // namespace cle
