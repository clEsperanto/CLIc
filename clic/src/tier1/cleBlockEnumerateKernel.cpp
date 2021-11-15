

#include "cleBlockEnumerateKernel.hpp"

namespace cle
{

BlockEnumerateKernel::BlockEnumerateKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "block_enumerate",
            {"dst", "src", "src_sums", "blocksize"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void BlockEnumerateKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void BlockEnumerateKernel::SetInputSums(Object& t_x)
{
    this->AddObject(t_x, "src_sums");
}

void BlockEnumerateKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void BlockEnumerateKernel::SetBlocksize(int t_x)
{
    this->AddObject(t_x, "blocksize");
}

void BlockEnumerateKernel::Execute()
{
    this->ManageDimensions("dst");
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
