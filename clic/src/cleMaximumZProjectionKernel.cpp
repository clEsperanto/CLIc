
#include "cleMaximumZProjectionKernel.h"

namespace cle
{
    
void MaximumZProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MaximumZProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_max");
}

void MaximumZProjectionKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
