
#include "cleMaximumXProjectionKernel.hpp"

namespace cle
{

MaximumXProjectionKernel::MaximumXProjectionKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "maximum_x_projection",
            {"dst_max", "src"}
    )
{
    this->m_Sources.insert({this->m_KernelName + "", this->m_OclHeader});
}

void MaximumXProjectionKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void MaximumXProjectionKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst_max");
}

void MaximumXProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
