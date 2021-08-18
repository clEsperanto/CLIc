

#include "cleSetNonzeroPixelsToPixelindexKernel.hpp"

namespace cle
{

void SetNonzeroPixelsToPixelindexKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void SetNonzeroPixelsToPixelindexKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SetNonzeroPixelsToPixelindexKernel::SetOffset(int x)
{
    this->AddObject(x, "offset");
}

void SetNonzeroPixelsToPixelindexKernel::Execute()
{
    if(this->m_Sources.size() > 1)
    {
        this->ManageDimensions("dst");
    }
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
