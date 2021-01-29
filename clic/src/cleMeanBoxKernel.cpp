

#include "cleMeanBoxKernel.h"
#include "cleExecuteSeparableKernel.h"

namespace cle
{

int MeanBoxKernel::Radius2KernelSize(float r)
{
    return int(r) * 2 + 1;
}

void MeanBoxKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MeanBoxKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void MeanBoxKernel::SetRadius(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void MeanBoxKernel::Execute()
{
    std::shared_ptr<cle::Buffer> src = std::dynamic_pointer_cast<cle::Buffer>(m_ParameterList.at("src"));
    std::shared_ptr<cle::Buffer> dst = std::dynamic_pointer_cast<cle::Buffer>(m_ParameterList.at("dst"));

    int nx = Radius2KernelSize(this->x);
    int ny = Radius2KernelSize(this->y);
    int nz = Radius2KernelSize(this->z);

    ExecuteSeparableKernel kernel(this->m_gpu);
    kernel.SetKernelName(this->m_KernelName);
    kernel.SetInput( *src );
    kernel.SetOutput( *dst );
    kernel.SetSigma(this->x, this->y, this->z);
    kernel.SetKernelSize(nx, ny, nz);
    kernel.Execute();
}

} // namespace cle
