/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/

#include "cleCloseIndexGapsInLabelMapKernel.h"

#include "cleConnectedComponentLabellingBoxKernel.h"
#include "cleSetNonzeroPixelsToPixelindexKernel.h"
#include "cleNonzeroMinimumBoxKernel.h"
#include "cleSetKernel.h"
#include "cleCopyKernel.h"

namespace cle
{

void ConnectedComponentLabellingBoxKernel::SetInput(Object& x)
{
    this->AddObject(&x, "src");
}

void ConnectedComponentLabellingBoxKernel::SetOutput(Object& x)
{
    this->AddObject(&x, "dst");
}

void ConnectedComponentLabellingBoxKernel::Execute()
{
    Buffer* src = dynamic_cast<Buffer*>(parameterList.at("src"));
    Buffer* dst = dynamic_cast<Buffer*>(parameterList.at("dst"));

    size_t arr_size = src->GetDimensions()[0] * src->GetDimensions()[1] * src->GetDimensions()[2];
    
    cl_mem temp1_mem = CreateBuffer<float>(sizeof(float) * arr_size, this->gpu);
    Buffer* temp1  = new Buffer(temp1_mem, src->GetDimensions(), src->GetDataType());
    cl_mem temp2_mem = CreateBuffer<float>(sizeof(float) * arr_size, this->gpu);
    Buffer* temp2  = new Buffer(temp2_mem, src->GetDimensions(), src->GetDataType());
    cl_mem temp3_mem = CreateBuffer<float>(sizeof(float) * arr_size, this->gpu);
    Buffer* temp3  = new Buffer(temp3_mem, src->GetDimensions(), src->GetDataType());

    SetNonzeroPixelsToPixelindexKernel setNonzeroPixelToIndex(this->gpu);
    setNonzeroPixelToIndex.SetInput(*src);
    setNonzeroPixelToIndex.SetOutput(*temp1);
    setNonzeroPixelToIndex.SetOffset(1);
    setNonzeroPixelToIndex.Execute();

    SetKernel setInit(this->gpu);
    setInit.SetInput(*temp2);
    setInit.SetValue(0);
    setInit.Execute();

    unsigned int flag_dim[3] = {1,1,2};
    float arr[2] = {0,0};
    cl_mem flag_mem = CreateBuffer<float>(sizeof(float) * 2, this->gpu);
    Buffer* flag = new Buffer (flag_mem, flag_dim, "float");

    float flag_value = 1;
    int iteration_count = 0;

    while (flag_value > 0)
    {
        NonzeroMinimumBoxKernel nonzeroMinBox(this->gpu);
        nonzeroMinBox.SetOutputFlag(*flag);
        if (iteration_count % 2 == 0)
        {
            nonzeroMinBox.SetInput(*temp1);
            nonzeroMinBox.SetOutput(*temp2);
        }
        else
        {
            nonzeroMinBox.SetInput(*temp2);
            nonzeroMinBox.SetOutput(*temp1);
        }
        nonzeroMinBox.Execute();
        
        flag_value = ReadBuffer<float>(flag->GetData(), sizeof(float) * 2, this->gpu)[0];

        SetKernel setFlag(this->gpu);
        setFlag.SetInput(*flag);
        setFlag.SetValue(0);
        setFlag.Execute();

        iteration_count++;
    }

    CopyKernel copy(this->gpu);
    if (iteration_count % 2 == 0)
    {    
        copy.SetInput(*temp1);
    }
    else
    {
        copy.SetInput(*temp2);
    }
    copy.SetOutput(*temp3);
    copy.Execute();

    CloseIndexGapsInLabelMapKernel closeGaps(this->gpu);
    closeGaps.SetInput(*temp3);
    closeGaps.SetOutput(*dst);
    closeGaps.SetBlockSize(4096);
    closeGaps.Execute();
}

} // namespace cle
