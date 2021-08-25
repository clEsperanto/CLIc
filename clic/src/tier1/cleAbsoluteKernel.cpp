

#include "cleAbsoluteKernel.hpp"

namespace cle
{

AbsoluteKernel::AbsoluteKernel (std::shared_ptr<GPU> gpu) :    
    Kernel( gpu, 
            "absolute",     // kernel name
            {"src" , "dst"} // parameter tags
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void AbsoluteKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void AbsoluteKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void AbsoluteKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
