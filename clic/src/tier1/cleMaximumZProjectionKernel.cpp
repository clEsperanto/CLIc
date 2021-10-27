
#include "cleMaximumZProjectionKernel.hpp"

namespace cle
{
    
MaximumZProjectionKernel::MaximumZProjectionKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "maximum_z_projection",
            {"dst_max", "src"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "", this->m_OclHeader});
}

void MaximumZProjectionKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void MaximumZProjectionKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst_max");
}

void MaximumZProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
