/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleMinimumOfAllPixelsKernel.h"
#include "cleMinimumZProjectionKernel.h"
#include "cleMinimumYProjectionKernel.h"
#include "cleMinimumXProjectionKernel.h"
#include "utils.h"


namespace cle
{
    
void MinimumOfAllPixelsKernel::SetInput(Object& x)
{
    this->AddObject(&x, "src");
}

void MinimumOfAllPixelsKernel::SetOutput(Object& x)
{
    this->AddObject(&x, "dst_max");
}

void MinimumOfAllPixelsKernel::Execute()
{
    Buffer* src = dynamic_cast<Buffer*>(parameterList.at("src"));
    Buffer* dst = dynamic_cast<Buffer*>(parameterList.at("dst_max"));
    if (src->GetDimensions()[2] > 1)
    {
        unsigned int tmp_dim[3] = {src->GetDimensions()[0], src->GetDimensions()[1], 1};
        size_t bitsize = tmp_dim[0] * tmp_dim[1];
        cl_mem tmp_mem = CreateBuffer<float>(bitsize, this->gpu);
        Buffer tmp (tmp_mem, tmp_dim, src->GetDataType());

        MinimumZProjectionKernel kernel(this->gpu);
        kernel.SetInput(*src);
        kernel.SetOutput(tmp);
        kernel.Execute();

        src = &tmp;
    }
    if (src->GetDimensions()[1] > 1)
    {
        unsigned int tmp_dim[3] = {src->GetDimensions()[0], 1, 1};
        size_t bitsize = tmp_dim[0];
        cl_mem tmp_mem = CreateBuffer<float>(bitsize, this->gpu);
        Buffer tmp (tmp_mem, tmp_dim, src->GetDataType());

        MinimumYProjectionKernel kernel(this->gpu);
        kernel.SetInput(*src);
        kernel.SetOutput(tmp);
        kernel.Execute();

        src = &tmp;
    }
    MinimumXProjectionKernel kernel(this->gpu);
    kernel.SetInput(*src);
    kernel.SetOutput(*dst);
    kernel.Execute();
}

} // namespace cle
