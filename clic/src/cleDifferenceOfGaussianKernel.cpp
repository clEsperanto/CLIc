/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleDifferenceOfGaussianKernel.h"
#include "cleGaussianBlurKernel.h"
#include "cleAddImagesWeightedKernel.h"

namespace cle
{

void DifferenceOfGaussianKernel::SetInput(Object& x)
{
    this->AddObject(&x, "src");
}

void DifferenceOfGaussianKernel::SetOutput(Object& x)
{
    this->AddObject(&x, "dst");
}

void DifferenceOfGaussianKernel::SetSigma1(float x, float y, float z)
{
    this->sigma1[0] = x;
    this->sigma1[1] = y;
    this->sigma1[2] = z;
}

void DifferenceOfGaussianKernel::SetSigma2(float x, float y, float z)
{
    this->sigma2[0] = x;
    this->sigma2[1] = y;
    this->sigma2[2] = z;
}

void DifferenceOfGaussianKernel::Execute()
{
    Buffer* src = dynamic_cast<Buffer*>(parameterList.at("src"));
    Buffer* dst = dynamic_cast<Buffer*>(parameterList.at("dst"));

    size_t size = src->GetDimensions()[0] * src->GetDimensions()[1] * src->GetDimensions()[2];
    cl_mem tmp1_mem = CreateBuffer<float>(size, this->gpu);
    Buffer temp1 (tmp1_mem, src->GetDimensions(), "float");
    cl_mem tmp2_mem = CreateBuffer<float>(size, this->gpu);
    Buffer temp2 (tmp2_mem, src->GetDimensions(), "float");

    GaussianBlurKernel gaussian1(this->gpu);
    gaussian1.SetInput(*src);
    gaussian1.SetOutput(temp1);
    gaussian1.SetSigma(this->sigma1[0], this->sigma1[1], this->sigma1[2]);
    gaussian1.Execute();

    GaussianBlurKernel gaussian2(this->gpu);
    gaussian2.SetInput(*src);
    gaussian2.SetOutput(temp2);
    gaussian2.SetSigma(this->sigma2[0], this->sigma2[1], this->sigma2[2]);
    gaussian2.Execute();

    AddImagesWeightedKernel difference(this->gpu);
    difference.SetInput1(temp1);
    difference.SetInput2(temp2);
    difference.SetOutput(*dst);
    difference.SetFactor1(1);
    difference.SetFactor2(-1);
    difference.Execute();
}

} // namespace cle
