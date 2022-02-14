

#include "cleDetectMaximaKernel.hpp"

namespace cle
{

DetectMaximaKernel::DetectMaximaKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu, 
            "detect_maxima",
            {"src" , "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
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
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
