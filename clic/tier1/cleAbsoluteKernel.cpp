

#include "cleAbsoluteKernel.h"

namespace cle
{

void AbsoluteKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src"); // populate Parameter list with data and tag
}

void AbsoluteKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void AbsoluteKernel::Execute()
{
    this->ManageDimensions("dst");  // deal with 2d, 3d, nd
    this->BuildProgramKernel();     // build program and kernel
    this->SetArguments();           // add arguments to kernel
    this->EnqueueKernel();          // enqueue kernel to GPU
}

} // namespace cle
