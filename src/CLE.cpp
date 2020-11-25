/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "CLE.h"
#include "cleKernelList.h"


namespace cle
{

CLE::CLE(GPU& device)
{
    gpu = device;
}

GPU CLE::GetGPU()
{
    return this->gpu;
}

void CLE::AddImageAndScalar(Buffer& src, Buffer& dst, float scalar)
{
    AddImageAndScalarKernel kernel(this->gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetScalar(scalar);
    kernel.Execute();
}

void CLE::AddImagesWeighted(Buffer& src1, Buffer& src2, Buffer& dst, float factor1, float factor2)
{
    AddImagesWeightedKernel kernel(this->gpu);
    kernel.SetInput1(src1);
    kernel.SetInput2(src2);
    kernel.SetOutput(dst);
    kernel.SetFactor1(1);
    kernel.SetFactor2(1);
    kernel.Execute();
}

void CLE::AddImages(Buffer& src1, Buffer& src2, Buffer& dst)
{
    this->AddImagesWeighted(src1, src2, dst, 1, 1);
}

void CLE::GreaterThanConstant(Buffer& src, Buffer& dst, float scalar)
{
    GreaterConstantKernel kernel(this->gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetScalar(scalar);
    kernel.Execute();
}

void CLE::MaximumZProjection(Buffer& src, Buffer& dst)
{
    MaximumZProjectionKernel kernel(this->gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void CLE::MaximumYProjection(Buffer& src, Buffer& dst)
{
    MaximumYProjectionKernel kernel(this->gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void CLE::MaximumXProjection(Buffer& src, Buffer& dst)
{
    MaximumXProjectionKernel kernel(this->gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}

void CLE::MaximumOfAllPixels(Buffer& src, Buffer& dst)
{
    MaximumOfAllPixelsKernel kernel(this->gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute(); 
}  


void CLE::Mean2DSphere(Buffer& src, Buffer& dst, int radius_x, int radius_y)
{
    Mean2DSphereKernel kernel(this->gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetRadiusX(radius_x);
    kernel.SetRadiusY(radius_y);
    kernel.Execute();
}

void CLE::NonzeroMinimumBox(Buffer& src, Buffer& dst, Buffer& flag)
{
    NonzeroMinimumBoxKernel kernel(this->gpu);
    kernel.SetInput(src);
    kernel.SetOutput1(dst);
    kernel.SetOutput2(flag);
    kernel.Execute();  
}

void CLE::SmallerOrEqualConstant(Buffer& src, Buffer& dst, float cst)
{
    SmallerOrEqualConstantKernel kernel(this->gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.SetConstant(cst);
    kernel.Execute();  
}

void CLE::Absolute(Buffer& src, Buffer& dst)
{
    AbsoluteKernel kernel(this->gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute();  
}

void CLE::Sobel(Buffer& src, Buffer& dst)
{
    SobelKernel kernel(this->gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute();  
}

void CLE::Set(Buffer& src, float value)
{
    SetKernel kernel(this->gpu);
    kernel.SetInput(src);
    kernel.SetValue(value);
    kernel.Execute();  
}

void CLE::SetNonzeroPixelsToPixelindex(Buffer& src, Buffer& dst)
{
    SetNonzeroPixelsToPixelindexKernel kernel(this->gpu);
    kernel.SetInput(src);
    kernel.SetOutput(dst);
    kernel.Execute();  
}

}