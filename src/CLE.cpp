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
    kernel.Execute(src, dst, scalar);
}

void CLE::MaximumZProjection(Buffer& src, Buffer& dst)
{
    MaximumZProjectionKernel kernel(this->gpu);
    kernel.Execute(src, dst); 
}

void CLE::SmallerOrEqualConstant(Buffer& src, Buffer& dst, float scalar)
{
    SmallerOrEqualConstantKernel kernel(this->gpu);
    kernel.Execute(src, dst, scalar);  
}

void CLE::Absolute(Buffer& src, Buffer& dst)
{
    AbsoluteKernel kernel(this->gpu);
    kernel.Execute(src, dst);  
}

}