

#include "cleBlockEnumerateKernel.hpp"

namespace cle
{

BlockEnumerateKernel::BlockEnumerateKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "block_enumerate",
            {"dst", "src", "src_sums", "blocksize"}
    )
{
    m_Sources.insert({this->m_KernelName, source});
}

void BlockEnumerateKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void BlockEnumerateKernel::SetInputSums(Buffer& x)
{
    this->AddObject(x, "src_sums");
}

void BlockEnumerateKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void BlockEnumerateKernel::SetBlocksize(int x)
{
    this->AddObject(x, "blocksize");
}

void BlockEnumerateKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
