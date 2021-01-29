
#include "cleMaximumYProjectionKernel.h"

namespace cle
{
    
void MaximumYProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MaximumYProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_max");
}

void MaximumYProjectionKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
