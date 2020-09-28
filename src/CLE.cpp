

#include "CLE.h"
#include "cleKernelList.h"


namespace cle
{

GPU CLE::GetGPU()
{
    return this->gpu;
}

void CLE::AddImageAndScalar(Buffer src, Buffer dst, float scalar)
{
    AddImageAndScalar addImageAndScalarKernel(this->gpu);
    addImageAndScalarKernel.Execute(src, dst, scalar);
}

void CLE::MaximumZProjection(src, dst)
{
    MaximumZProjection maximumProjection(this->gpu);
    maximumProjection.Execute(src, dst); 
}

void CLE::SmallerOrEqualConstant(src, dst, scalar)
{
    SmallerOrEqualConstant addImageAndScalarKernel(this->gpu);
    addImageAndScalarKernel.Execute(src, dst, scalar);  
}


}