

#include "cleSetNonzeroPixelsToPixelindexKernel.hpp"

namespace cle
{

SetNonzeroPixelsToPixelindexKernel::SetNonzeroPixelsToPixelindexKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu, 
            "set_nonzero_pixels_to_pixelindex",
            {"src" , "dst", "index"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void SetNonzeroPixelsToPixelindexKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void SetNonzeroPixelsToPixelindexKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void SetNonzeroPixelsToPixelindexKernel::SetOffset(int t_x)
{
    this->AddObject(t_x, "index");
}

void SetNonzeroPixelsToPixelindexKernel::Execute()
{
    this->ManageDimensions();
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
