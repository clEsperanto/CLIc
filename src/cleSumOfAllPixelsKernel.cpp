/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleSumOfAllPixelsKernel.h"
#include "cleSumZProjectionKernel.h"
#include "cleSumYProjectionKernel.h"
#include "cleSumXProjectionKernel.h"
#include "utils.h"


namespace cle
{
    
void SumOfAllPixelsKernel::SetInput(Object& x)
{
    this->AddObject(&x, "src");
}

void SumOfAllPixelsKernel::SetOutput(Object& x)
{
    this->AddObject(&x, "dst");
}

void SumOfAllPixelsKernel::Execute()
{
    Buffer* src = dynamic_cast<Buffer*>(parameterList.at("src"));
    Buffer* dst = dynamic_cast<Buffer*>(parameterList.at("dst"));
    unsigned int tmp_dim[3] = {src->GetDimensions()[0], src->GetDimensions()[1], src->GetDimensions()[2]};

    if (tmp_dim[2] > 1)
    {
        tmp_dim[2] = 1;
        size_t bitsize = tmp_dim[0] * tmp_dim[1] * tmp_dim[2];
        cl_mem tmp_mem = CreateBuffer<float>(bitsize, this->gpu);
        Buffer* temp1  = new Buffer(tmp_mem, tmp_dim, src->GetDataType());

        SumZProjectionKernel kernel(this->gpu);
        kernel.SetInput(*src);
        kernel.SetOutput(*temp1);
        kernel.Execute();

        src = temp1;
    }
    if (tmp_dim[1] > 1)
    {
        tmp_dim[1] = 1;
        size_t bitsize = tmp_dim[0] * tmp_dim[1] * tmp_dim[2];
        cl_mem tmp_mem = CreateBuffer<float>(bitsize, this->gpu);
        Buffer* temp2  = new Buffer (tmp_mem, tmp_dim, src->GetDataType());

        SumYProjectionKernel kernel(this->gpu);
        kernel.SetInput(*src);
        kernel.SetOutput(*temp2);
        kernel.Execute();

        src = temp2;
    }
    SumXProjectionKernel kernel(this->gpu);
    kernel.SetInput(*src);
    kernel.SetOutput(*dst);
    kernel.Execute();
}

} // namespace cle
