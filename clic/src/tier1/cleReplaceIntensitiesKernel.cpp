

#include "cleReplaceIntensitiesKernel.hpp"

namespace cle
{

ReplaceIntensitiesKernel::ReplaceIntensitiesKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "replace_intensities",
            {"dst", "src", "map"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "", this->m_OclHeader});
} 

void ReplaceIntensitiesKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void ReplaceIntensitiesKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void ReplaceIntensitiesKernel::SetMap(Object& t_x)
{
    this->AddObject(t_x, "map");
}

void ReplaceIntensitiesKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
