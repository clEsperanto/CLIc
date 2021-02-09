

#include "cleCloseIndexGapsInLabelMapKernel.h"

#include "cleMaximumOfAllPixelsKernel.h"
#include "cleFlagExistingLabelsKernel.h"
#include "cleSetKernel.h"
#include "cleSumReductionXKernel.h"
#include "cleSetColumnKernel.h"
#include "cleBlockEnumerateKernel.h"
#include "cleReplaceIntensitiesKernel.h"


namespace cle
{

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
    
    unsigned int one_dim[3] = {1,1,1};
    cl::Buffer max_obj = CreateBuffer<float>(1, this->m_gpu);
    Buffer max = Buffer(max_obj, LightObject::DataType::Float);

    MaximumOfAllPixelsKernel maximumOfPixels(this->m_gpu);
    maximumOfPixels.SetInput(*src);
    maximumOfPixels.SetOutput(max);
    maximumOfPixels.Execute();

    float max_value;
    ReadBuffer<float>(max.GetObject(), &max_value, 1, this->m_gpu);
    unsigned int nb_indices =  int(max_value) + 1;

    unsigned int indices_dim[3] = {nb_indices, 1, 1};
    cl::Buffer flaggedIndices_obj = CreateBuffer<float>(nb_indices, this->m_gpu);
    Buffer flaggedIndices = Buffer(flaggedIndices_obj, indices_dim, LightObject::DataType::Float);

    FlagExistingLabelsKernel flagLabels(this->m_gpu);
    flagLabels.SetInput(*src);
    flagLabels.SetOutput(flaggedIndices);
    flagLabels.Execute();

    SetColumnKernel setColumn(this->m_gpu);
    setColumn.SetInput(flaggedIndices);
    setColumn.SetColumn(0);
    setColumn.SetValue(0);
    setColumn.Execute();

    unsigned int nb_sums = int(nb_indices / this->blocksize) + 1;
    unsigned int sums_dim[3] = {nb_sums, 1, 1};
    cl::Buffer sums_obj = CreateBuffer<float>(nb_sums, this->m_gpu);
    Buffer blockSums = Buffer(sums_obj, sums_dim, LightObject::DataType::Float);

    SumReductionXKernel sumReductionX(this->m_gpu);
    sumReductionX.SetInput(flaggedIndices);
    sumReductionX.SetOutput(blockSums);
    sumReductionX.SetBlocksize(this->blocksize);
    sumReductionX.Execute();

    unsigned int newIndices_dim[3] = {nb_indices, 1, 1};
    cl::Buffer newIndices_obj = CreateBuffer<float>(nb_indices, this->m_gpu);
    Buffer newIndices = Buffer(newIndices_obj, newIndices_dim, LightObject::DataType::Float);

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
