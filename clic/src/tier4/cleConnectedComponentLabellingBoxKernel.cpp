
#include "cleCloseIndexGapsInLabelMapKernel.hpp"

#include "cleConnectedComponentLabellingBoxKernel.hpp"
#include "cleSetNonzeroPixelsToPixelindexKernel.hpp"
#include "cleNonzeroMinimumBoxKernel.hpp"
#include "cleSetKernel.hpp"
#include "cleCopyKernel.hpp"

namespace cle
{

void ConnectedComponentLabellingBoxKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void ConnectedComponentLabellingBoxKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void ConnectedComponentLabellingBoxKernel::Execute()
{
    std::shared_ptr<Buffer> src = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("src"));
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("dst"));
        
    cl::Buffer temp1_obj = CreateBuffer<float>(src->GetSize(), this->m_gpu);
    Buffer temp1 = Buffer(temp1_obj, src->GetDimensions(), LightObject::DataType::Float);
    cl::Buffer temp2_obj = CreateBuffer<float>(src->GetSize(), this->m_gpu);
    Buffer temp2 = Buffer(temp2_obj, src->GetDimensions(), LightObject::DataType::Float);
    cl::Buffer temp3_obj = CreateBuffer<float>(src->GetSize(), this->m_gpu);
    Buffer temp3 = Buffer(temp3_obj, src->GetDimensions(), LightObject::DataType::Float);

    SetNonzeroPixelsToPixelindexKernel setNonzeroPixelToIndex(this->m_gpu);
    setNonzeroPixelToIndex.SetInput(*src);
    setNonzeroPixelToIndex.SetOutput(temp1);
    setNonzeroPixelToIndex.SetOffset(1);
    setNonzeroPixelToIndex.Execute();

    SetKernel setInit(this->m_gpu);
    setInit.SetInput(temp2);
    setInit.SetValue(0);
    setInit.Execute();

    unsigned int flag_dim[3] = {1,1,2};
    float arr[2] = {0,0};
    cl::Buffer flag_obj = CreateBuffer<float>(2, this->m_gpu);
    Buffer flag = Buffer(flag_obj, flag_dim, LightObject::DataType::Float);

    float flag_value[2] = {1, 1};
    int iteration_count = 0;
    NonzeroMinimumBoxKernel nonzeroMinBox(this->m_gpu);
    SetKernel setFlag(this->m_gpu);
    while (flag_value[0] > 0)
    {
        nonzeroMinBox.SetOutputFlag(flag);
        if (iteration_count % 2 == 0)
        {
            nonzeroMinBox.SetInput(temp1);
            nonzeroMinBox.SetOutput(temp2);
        }
        else
        {
            nonzeroMinBox.SetInput(temp2);
            nonzeroMinBox.SetOutput(temp1);
        }
        nonzeroMinBox.Execute();
        ReadBuffer<float>(flag.GetObject(), flag_value, 2, this->m_gpu);
        setFlag.SetInput(flag);
        setFlag.SetValue(0);
        setFlag.Execute();

        iteration_count++;
    }

    CopyKernel copy(this->m_gpu);
    if (iteration_count % 2 == 0)
    {    
        copy.SetInput(temp1);
    }
    else
    {
        copy.SetInput(temp2);
    }
    copy.SetOutput(temp3);
    copy.Execute();

    CloseIndexGapsInLabelMapKernel closeGaps(this->m_gpu);
    closeGaps.SetInput(temp3);
    closeGaps.SetOutput(*dst);
    closeGaps.SetBlockSize(4096);
    closeGaps.Execute();
}

} // namespace cle
