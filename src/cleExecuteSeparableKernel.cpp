/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleExecuteSeparableKernel.h"
#include "cleSeparableKernel.h"
#include "cleCopyKernel.h"
#include "utils.h"

namespace cle
{

void ExecuteSeparableKernel::SetInput(Object& x)
{
    this->AddObject(&x, "src");
}

void ExecuteSeparableKernel::SetOutput(Object& x)
{
    this->AddObject(&x, "dst");
}

int ExecuteSeparableKernel::Sigma2KernelSize(float s)
{
    int n = int(s * 8.0);
    if (n % 2 == 0)
    {
        n = n + 1;
    }
    return n;
}

void ExecuteSeparableKernel::SetSigma(float x, float y, float z)
{
    this->sigma = {x, y, z};
    for (size_t i = 0; i < 3; i++)
    {
        this->kernel_size[i] = this->Sigma2KernelSize(this->sigma[i]);
    }
}

void ExecuteSeparableKernel::SetKernelName(std::string name)
{
    kernelName = name + kernelName;
}

void ExecuteSeparableKernel::Execute()
{ 
    Buffer* src = dynamic_cast<Buffer*>(parameterList.at("src"));
    Buffer* dst = dynamic_cast<Buffer*>(parameterList.at("dst"));

    if (src->GetDimensions()[1] > 1)
    {
        this->dimension = 2;
    }
    if (src->GetDimensions()[2] > 1)
    {
        this->dimension = 3;
    }

    // create temp buffer
    size_t size = src->GetDimensions()[0] * src->GetDimensions()[1] * src->GetDimensions()[2];
    cl_mem tmp1_mem = CreateBuffer<float>(size, this->gpu);
    Buffer temp1 (tmp1_mem, src->GetDimensions(), "float");
    cl_mem tmp2_mem = CreateBuffer<float>(size, this->gpu);
    Buffer temp2 (tmp2_mem, src->GetDimensions(), "float");

    if (sigma[0] > 0)
    {
        SeparableKernel kernel(this->gpu);
        kernel.SetKernelName(this->kernelName);
        kernel.SetInput(*src);
        if (this->dimension == 2)
        {
            kernel.SetOutput(temp1);
        }
        else
        {
            kernel.SetOutput(temp2);
        }
        kernel.SetSigma(sigma[0]);
        kernel.SetSize(kernel_size[0]);
        kernel.SetDimension(0);
        kernel.Execute();
    }
    else
    {
        CopyKernel kernel(this->gpu);
        if (this->dimension == 2)
        {
            kernel.SetInput(*src);
            kernel.SetOutput(temp1);
        }
        else
        {
            kernel.SetInput(*src);
            kernel.SetOutput(temp2);
        }
        kernel.Execute();
    }

    if (sigma[1] > 0)
    {
        SeparableKernel kernel(this->gpu);
        kernel.SetKernelName(this->kernelName);
        if (this->dimension == 2)
        {
            kernel.SetInput(temp1);
            kernel.SetOutput(*dst);
        }
        else
        {
            kernel.SetInput(temp2);
            kernel.SetOutput(temp1);
        }
        kernel.SetSigma(sigma[1]);
        kernel.SetSize(kernel_size[1]);
        kernel.SetDimension(1);
        kernel.Execute();
    }
    else
    {
        CopyKernel kernel(this->gpu);
        if (this->dimension == 2)
        {
            kernel.SetInput(temp1);
            kernel.SetOutput(*dst);
        }
        else
        {
            kernel.SetInput(temp2);
            kernel.SetOutput(temp1);
        }
        kernel.Execute();
    }

    if (this->dimension == 3)
    {
        if (sigma[2] > 0)
        {
            SeparableKernel kernel(this->gpu);
            kernel.SetKernelName(this->kernelName);
            kernel.SetInput(temp1);
            kernel.SetOutput(*dst);
            kernel.SetSigma(sigma[2]);
            kernel.SetSize(kernel_size[2]);
            kernel.SetDimension(2);
            kernel.Execute();
        }
        else
        {
            CopyKernel kernel(this->gpu);
            kernel.SetInput(temp1);
            kernel.SetOutput(*dst);
            kernel.Execute();
        }
    }
}

} // namespace cle
