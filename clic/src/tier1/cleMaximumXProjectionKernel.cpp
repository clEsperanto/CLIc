
#include "cleMaximumXProjectionKernel.hpp"

namespace cle
{

MaximumXProjectionKernel::MaximumXProjectionKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "maximum_x_projection",
            {"dst_max", "src"}
    )
{
    m_Sources.insert({this->m_KernelName + "", source});
}

void MaximumXProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MaximumXProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_max");
}

void MaximumXProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
