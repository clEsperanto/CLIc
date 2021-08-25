

#include "cleSetNonzeroPixelsToPixelindexKernel.hpp"

namespace cle
{

SetNonzeroPixelsToPixelindexKernel::SetNonzeroPixelsToPixelindexKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu, 
            "set_nonzero_pixels_to_pixelindex",
            {"dst" , "src", "offset"}
    )
{
    m_Sources.insert({this->m_KernelName + "", source});
}

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
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
