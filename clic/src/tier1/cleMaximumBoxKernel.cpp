

#include "cleMaximumBoxKernel.h"
#include "cleExecuteSeparableKernel.h"

namespace cle
{

int MaximumBoxKernel::Radius2KernelSize(float r)
{
    return int(r) * 2 + 1;
}

void MaximumBoxKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MaximumBoxKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void MaximumBoxKernel::SetRadius(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void MaximumBoxKernel::Execute()
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
