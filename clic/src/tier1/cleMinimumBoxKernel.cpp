

#include "cleMinimumBoxKernel.hpp"
#include "cleExecuteSeparableKernel.hpp"

namespace cle
{

MinimumBoxKernel::MinimumBoxKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu,
            "minimum_separable",
            {"dst", "src"}
    )
{
    m_Sources.insert({this->m_KernelName + "_2d", source_2d});
    m_Sources.insert({this->m_KernelName + "_3d", source_3d}); 
}    

int MinimumBoxKernel::Radius2KernelSize(float r)
{
    return int(r) * 2 + 1;
}

void MinimumBoxKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MinimumBoxKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void MinimumBoxKernel::SetRadius(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void MinimumBoxKernel::Execute()
{
    std::shared_ptr<Buffer> src = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("src"));
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("dst"));
    
    int nx = Radius2KernelSize(this->x);
    int ny = Radius2KernelSize(this->y);
    int nz = Radius2KernelSize(this->z);

    ExecuteSeparableKernel kernel(this->m_gpu);
    kernel.SetKernelName(this->m_KernelName);
    kernel.SetSources(this->m_Sources);
    kernel.SetInput(*src);
    kernel.SetOutput(*dst);
    kernel.SetSigma(this->x, this->y, this->z);
    kernel.SetKernelSize(nx, ny, nz);
    kernel.Execute();
}

} // namespace cle
