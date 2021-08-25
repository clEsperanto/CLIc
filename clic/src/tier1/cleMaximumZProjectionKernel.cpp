
#include "cleMaximumZProjectionKernel.hpp"

namespace cle
{
    
MaximumZProjectionKernel::MaximumZProjectionKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "maximum_z_projection",
            {"dst_max", "src"}
    )
{
    m_Sources.insert({this->m_KernelName + "", source});
}

void MaximumZProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MaximumZProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_max");
}

void MaximumZProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
