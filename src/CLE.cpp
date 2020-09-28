

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


}