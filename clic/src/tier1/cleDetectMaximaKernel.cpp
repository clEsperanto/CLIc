

#include "cleDetectMaximaKernel.hpp"

namespace cle
{

DetectMaximaKernel::DetectMaximaKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu, 
            "detect_maxima",
            {"src" , "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "_2d", this->m_OclHeader2d});
    this->m_Sources.insert({this->m_KernelName + "_3d", this->m_OclHeader3d});
}

void DetectMaximaKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void DetectMaximaKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void DetectMaximaKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
