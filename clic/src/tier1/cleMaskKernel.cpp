

#include "cleMaskKernel.hpp"

namespace cle
{

MaskKernel::MaskKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "mask",
            {"src0", "src1", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void MaskKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void MaskKernel::SetMask(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void MaskKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void MaskKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}
} // namespace cle
