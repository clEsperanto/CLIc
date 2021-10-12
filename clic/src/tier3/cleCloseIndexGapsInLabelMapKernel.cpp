

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

CloseIndexGapsInLabelMapKernel::CloseIndexGapsInLabelMapKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "close_index_gaps_in_label_map",
            {"src", "dst", "blocksize"}
    )
{}

void CloseIndexGapsInLabelMapKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void CloseIndexGapsInLabelMapKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void CloseIndexGapsInLabelMapKernel::SetBlockSize(int x)
{
    this->blocksize = x;
}

void CloseIndexGapsInLabelMapKernel::Execute()
{
    std::shared_ptr<Buffer> src = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("src"));
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("dst"));
    
    cl::Buffer max_obj = CreateBuffer<float>(1, this->m_gpu);
    Buffer max = Buffer(max_obj, Buffer::FLOAT);

    MaximumOfAllPixelsKernel maximumOfPixels(this->m_gpu);
    maximumOfPixels.SetInput(*src);
    maximumOfPixels.SetOutput(max);
    maximumOfPixels.Execute();

    float max_value;
    ReadBuffer<float>(max.GetObject(), &max_value, 1, this->m_gpu);
    int nb_indices =  int(max_value) + 1;

    int indices_dim[3] = {nb_indices, 1, 1};
    cl::Buffer flaggedIndices_obj = CreateBuffer<float>(nb_indices, this->m_gpu);
    Buffer flaggedIndices = Buffer(flaggedIndices_obj, indices_dim, Buffer::FLOAT);

    FlagExistingLabelsKernel flagLabels(this->m_gpu);
    flagLabels.SetInput(*src);
    flagLabels.SetOutput(flaggedIndices);
    flagLabels.Execute();

    SetColumnKernel setColumn(this->m_gpu);
    setColumn.SetInput(flaggedIndices);
    setColumn.SetColumn(0);
    setColumn.SetValue(0);
    setColumn.Execute();

    int nb_sums = int(nb_indices / this->blocksize) + 1;
    int sums_dim[3] = {nb_sums, 1, 1};
    cl::Buffer sums_obj = CreateBuffer<float>(nb_sums, this->m_gpu);
    Buffer blockSums = Buffer(sums_obj, sums_dim, Buffer::FLOAT);

    SumReductionXKernel sumReductionX(this->m_gpu);
    sumReductionX.SetInput(flaggedIndices);
    sumReductionX.SetOutput(blockSums);
    sumReductionX.SetBlocksize(this->blocksize);
    sumReductionX.Execute();

    int newIndices_dim[3] = {nb_indices, 1, 1};
    cl::Buffer newIndices_obj = CreateBuffer<float>(nb_indices, this->m_gpu);
    Buffer newIndices = Buffer(newIndices_obj, newIndices_dim, Buffer::FLOAT);

    BlockEnumerateKernel blockEnumerate(this->m_gpu);
    blockEnumerate.SetInput(flaggedIndices);
    blockEnumerate.SetInputSums(blockSums);
    blockEnumerate.SetOutput(newIndices);
    blockEnumerate.SetBlocksize(this->blocksize);
    blockEnumerate.Execute();

    ReplaceIntensitiesKernel replaceIntensities(this->m_gpu);
    replaceIntensities.SetInput(*src);
    replaceIntensities.SetOutput(*dst);
    replaceIntensities.SetMap(newIndices);
    replaceIntensities.Execute();
}

} // namespace cle
