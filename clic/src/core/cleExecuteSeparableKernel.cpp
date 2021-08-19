
#include "cleExecuteSeparableKernel.hpp"
#include "cleSeparableKernel.hpp"

namespace cle
{

void ExecuteSeparableKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void ExecuteSeparableKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst");
}

void ExecuteSeparableKernel::SetKernelSize(int x, int y, int z)
{
    this->m_KernelSize = {x, y, z};
}

void ExecuteSeparableKernel::SetSigma(float x, float y, float z)
{
    this->m_Sigma = {x, y, z};
}

void ExecuteSeparableKernel::SetKernelName(std::string name)
{
    m_KernelName = name;
}

void ExecuteSeparableKernel::SetSources(std::map<std::string, std::string> sources)
{
    m_Sources = sources;
}

void ExecuteSeparableKernel::Execute()
{ 
    std::shared_ptr<cle::Buffer> src = std::dynamic_pointer_cast<cle::Buffer>(m_ParameterList.at("src"));
    std::shared_ptr<cle::Buffer> dst = std::dynamic_pointer_cast<cle::Buffer>(m_ParameterList.at("dst"));

    this->m_Dim = src->GetDimension();

    // create temp buffer
    cl::Buffer tmp1_obj = cle::CreateBuffer<float>(src->GetSize(), this->m_gpu);
    cle::Buffer temp1 (tmp1_obj, src->GetShape(), cle::Buffer::FLOAT);
    cl::Buffer tmp2_obj = cle::CreateBuffer<float>(src->GetSize(), this->m_gpu);
    cle::Buffer temp2 (tmp2_obj, src->GetShape(), cle::Buffer::FLOAT);

    SeparableKernel kernel(this->m_gpu);
    kernel.SetKernelName(this->m_KernelName);
    kernel.SetSources(this->m_Sources);
    if (m_Sigma[0] > 0)
    {
        kernel.SetInput( *src );
        if (this->m_Dim == 2)
        {
            kernel.SetOutput(temp1);
        }
        else
        {
            kernel.SetOutput(temp2);
        }
        kernel.SetSigma(m_Sigma[0]);
        kernel.SetSize(m_KernelSize[0]);
        kernel.SetDimension(0);
        kernel.Execute();
    }
    else
    {
        if (this->m_Dim == 2)
        {
            cle::CopyBuffer<float>(src->GetObject(), temp1.GetObject(), src->GetSize(), this->m_gpu);
        }
        else
        {
            cle::CopyBuffer<float>(src->GetObject(), temp2.GetObject(), src->GetSize(), this->m_gpu);
        }
    }

    if (m_Sigma[1] > 0)
    {
        if (this->m_Dim == 2)
        {
            kernel.SetInput(temp1);
            kernel.SetOutput( *dst );
        }
        else
        {
            kernel.SetInput(temp2);
            kernel.SetOutput(temp1);
        }
        kernel.SetSigma(m_Sigma[1]);
        kernel.SetSize(m_KernelSize[1]);
        kernel.SetDimension(1);
        kernel.Execute();
    }
    else
    {
        if (this->m_Dim == 2)
        {
            cle::CopyBuffer<float>(temp1.GetObject(), dst->GetObject(), dst->GetSize(), this->m_gpu);
        }
        else
        {
            cle::CopyBuffer<float>(temp2.GetObject(), temp1.GetObject(), dst->GetSize(), this->m_gpu);
        }
    }
    if (this->m_Dim == 3)
    {
        if (m_Sigma[2] > 0)
        {
            kernel.SetInput(temp1);
            kernel.SetOutput( *dst );
            kernel.SetSigma(m_Sigma[2]);
            kernel.SetSize(m_KernelSize[2]);
            kernel.SetDimension(2);
            kernel.Execute();
        }
        else
        {
            cle::CopyBuffer<float>(temp1.GetObject(), dst->GetObject(), dst->GetSize(), this->m_gpu);
        }

    }
}

} // namespace cle
