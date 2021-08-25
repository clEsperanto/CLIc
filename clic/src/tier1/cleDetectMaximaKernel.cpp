

#include "cleDetectMaximaKernel.hpp"

namespace cle
{

DetectMaximaKernel::DetectMaximaKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu, 
            "detect_maxima",
            {"src" , "dst"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void DetectMaximaKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void DetectMaximaKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void DetectMaximaKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
