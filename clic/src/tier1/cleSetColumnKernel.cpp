

#include "cleSetColumnKernel.hpp"

namespace cle
{

SetColumnKernel::SetColumnKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "set_column",
            {"dst" , "column", "value"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d});
}

void SetColumnKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void SetColumnKernel::SetColumn(int x)
{
    this->AddObject(x, "column");
}

void SetColumnKernel::SetValue(float x)
{
    this->AddObject(x, "value");
}

void SetColumnKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
