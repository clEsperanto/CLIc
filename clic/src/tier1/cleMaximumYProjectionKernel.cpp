
#include "cleMaximumYProjectionKernel.hpp"

namespace cle
{

MaximumYProjectionKernel::MaximumYProjectionKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "maximum_y_projection",
            {"dst_max", "src"}
    )
{
    m_Sources.insert({this->m_KernelName + "", source});
}

void MaximumYProjectionKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MaximumYProjectionKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_max");
}

void MaximumYProjectionKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
