

#include "cleMaximumKernel.h"
#include "cleExecuteSeparableKernel.h"

namespace cle
{

int MaximumKernel::Radius2KernelSize(float r)
{
    return int(r) * 2 + 1;
}

void MaximumKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MaximumKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void MaximumKernel::SetRadius(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void MaximumKernel::Execute()
{
    std::shared_ptr<Buffer> src = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("src"));
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("dst"));

    int nx = Radius2KernelSize(this->x);
    int ny = Radius2KernelSize(this->y);
    int nz = Radius2KernelSize(this->z);

    ExecuteSeparableKernel kernel(this->m_gpu);
    kernel.SetKernelName(this->m_KernelName);
    kernel.SetInput(*src);
    kernel.SetOutput(*dst);
    kernel.SetSigma(this->x, this->y, this->z);
    kernel.SetKernelSize(nx, ny, nz);
    kernel.Execute();
}

} // namespace cle
