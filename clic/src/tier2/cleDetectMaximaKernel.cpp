
#include "cleMeanBoxKernel.hpp"
#include "cleCopyKernel.hpp"
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

void DetectMaximaKernel::SetRadius(int t_x, int t_y, int t_z)
{
    this->m_x = t_x;
    this->m_y = t_y;
    this->m_z = t_z;
}

void DetectMaximaKernel::Execute()
{
    auto src = this->GetParameter<Object>("src");
    auto dst = this->GetParameter<Object>("dst");
    if (this->m_x > 2 || this->m_z > 2 || this->m_z > 2)
    {
        MeanBoxKernel mean(this->m_gpu);
        mean.SetInput(*src);
        mean.SetInput(*dst);
        mean.SetRadius(m_x,m_y,m_z);
        mean.Execute();
        CopyKernel copy(this->m_gpu);
        copy.SetInput(*dst);
        copy.SetOutput(*src);
        copy.Execute();
    }
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
