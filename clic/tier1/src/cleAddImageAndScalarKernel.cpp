
#include "cleAddImageAndScalarKernel.h"

namespace cle
{

void AddImageAndScalarKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void AddImageAndScalarKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void AddImageAndScalarKernel::SetScalar(float x)
{
    this->AddObject(x, "scalar");
}

void AddImageAndScalarKernel::Execute()
{
    if( this->m_Sources.size() > 1)
    {
        this->ManageDimensions("dst"); // deal with 2d, 3d, nd
    }
    this->BuildProgramKernel();     // build program and kernel
    this->SetArguments();           // add arguments to kernel
    this->EnqueueKernel();          // enqueue kernel to GPU
}

} // namespace cle

