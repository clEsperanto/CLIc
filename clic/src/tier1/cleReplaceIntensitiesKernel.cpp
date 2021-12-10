

#include "cleReplaceIntensitiesKernel.hpp"

namespace cle
{

ReplaceIntensitiesKernel::ReplaceIntensitiesKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "replace_intensities",
            {"src0", "src1", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
} 

void ReplaceIntensitiesKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void ReplaceIntensitiesKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void ReplaceIntensitiesKernel::SetMap(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void ReplaceIntensitiesKernel::Execute()
{
    this->ManageDimensions();
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
