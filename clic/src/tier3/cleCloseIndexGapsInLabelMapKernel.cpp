

#include "cleCloseIndexGapsInLabelMapKernel.hpp"

#include "cleMaximumOfAllPixelsKernel.hpp"
#include "cleFlagExistingLabelsKernel.hpp"
#include "cleSetKernel.hpp"
#include "cleSumReductionXKernel.hpp"
#include "cleSetColumnKernel.hpp"
#include "cleBlockEnumerateKernel.hpp"
#include "cleReplaceIntensitiesKernel.hpp"


namespace cle
{

CloseIndexGapsInLabelMapKernel::CloseIndexGapsInLabelMapKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "close_index_gaps_in_label_map",
            {"src", "dst", "blocksize"}
    )
{}

void CloseIndexGapsInLabelMapKernel::SetInput(Buffer& t_x)
{
    this->AddObject(t_x, "src");
}

void CloseIndexGapsInLabelMapKernel::SetOutput(Buffer& t_x)
{
    this->AddObject(t_x, "dst");
}

void CloseIndexGapsInLabelMapKernel::SetBlockSize(int t_x)
{
    this->m_Blocksize = t_x;
}

void CloseIndexGapsInLabelMapKernel::Execute()
{
    std::shared_ptr<Buffer> src = std::dynamic_pointer_cast<Buffer>(this->m_Parameters.at("src"));
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(this->m_Parameters.at("dst"));
    
    // std::array<int,3> one_dim = {1,1,1};
    cle::Buffer max_value_buffer = this->m_gpu->CreateBuffer<float>();

    MaximumOfAllPixelsKernel max_of_pixel_kernel(this->m_gpu);
    max_of_pixel_kernel.SetInput(*src);
    max_of_pixel_kernel.SetOutput(max_value_buffer);
    max_of_pixel_kernel.Execute();

    float max_value = m_gpu->Pull<float>(max_value_buffer).front();
    int nb_indices = static_cast<int>(max_value) + 1;

    std::array<int,3> indices_dim = {nb_indices, 1, 1};
    cle::Buffer flagged_indices = this->m_gpu->CreateBuffer<float>(indices_dim);

    FlagExistingLabelsKernel flag_labels_kernel(this->m_gpu);
    flag_labels_kernel.SetInput(*src);
    flag_labels_kernel.SetOutput(flagged_indices);
    flag_labels_kernel.Execute();

    SetColumnKernel set_column_kernel(this->m_gpu);
    set_column_kernel.SetInput(flagged_indices);
    set_column_kernel.SetColumn(0);
    set_column_kernel.SetValue(0);
    set_column_kernel.Execute();

    int nb_sums = static_cast<int>(nb_indices / this->m_Blocksize) + 1;
    std::array<int,3> sums_dim = {nb_sums, 1, 1};
    cle::Buffer block_sums = this->m_gpu->CreateBuffer<float>(sums_dim);

    SumReductionXKernel sum_reduction_x_kernel(this->m_gpu);
    sum_reduction_x_kernel.SetInput(flagged_indices);
    sum_reduction_x_kernel.SetOutput(block_sums);
    sum_reduction_x_kernel.SetBlocksize(this->m_Blocksize);
    sum_reduction_x_kernel.Execute();

    std::array<int,3> new_indices_dim = {nb_indices, 1, 1};
    cle::Buffer new_indices = this->m_gpu->CreateBuffer<float>(new_indices_dim);

    BlockEnumerateKernel block_enumerate_kernel(this->m_gpu);
    block_enumerate_kernel.SetInput(flagged_indices);
    block_enumerate_kernel.SetInputSums(block_sums);
    block_enumerate_kernel.SetOutput(new_indices);
    block_enumerate_kernel.SetBlocksize(this->m_Blocksize);
    block_enumerate_kernel.Execute();

    ReplaceIntensitiesKernel replace_intensities_kernel(this->m_gpu);
    replace_intensities_kernel.SetInput(*src);
    replace_intensities_kernel.SetOutput(*dst);
    replace_intensities_kernel.SetMap(new_indices);
    replace_intensities_kernel.Execute();
}

} // namespace cle
