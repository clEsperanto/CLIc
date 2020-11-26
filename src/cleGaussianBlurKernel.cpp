/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleGaussianBlurKernel.h"
#include "cleExecuteSeparableKernel.h"

namespace cle
{

void GaussianBlurKernel::SetInput(Object& x)
{
    this->AddObject(&x, "src");
}

void GaussianBlurKernel::SetOutput(Object& x)
{
    this->AddObject(&x, "dst");
}

void GaussianBlurKernel::SetSigma(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void GaussianBlurKernel::Execute()
{
    Buffer* src = dynamic_cast<Buffer*>(parameterList.at("src"));
    Buffer* dst = dynamic_cast<Buffer*>(parameterList.at("dst"));

    ExecuteSeparableKernel kernel(this->gpu);
    kernel.SetKernelName(this->kernelName);
    kernel.SetInput(*src);
    kernel.SetOutput(*dst);
    kernel.SetSigma(this->x, this->y, this->z);
    kernel.Execute();
}

} // namespace cle
