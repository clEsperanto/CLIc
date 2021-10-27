
#include "cleExecuteSeparableKernel.hpp"
#include "cleSeparableKernel.hpp"

namespace cle
{

ExecuteSeparableKernel::ExecuteSeparableKernel (std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu, 
            "",
            {"dst" , "src", "dim", "N", "s"}
    )
{}    

void ExecuteSeparableKernel::SetInput(Buffer& t_x)
{
    this->AddObject(t_x, "src");
}

void ExecuteSeparableKernel::SetOutput(Buffer& t_x)
{
    this->AddObject(t_x, "dst");
}

void ExecuteSeparableKernel::SetKernelSize(int t_x, int t_y, int t_z)
{
    this->m_KernelSize = {t_x, t_y, t_z};
}

void ExecuteSeparableKernel::SetSigma(float t_x, float t_y, float t_z)
{
    this->m_Sigma = {t_x, t_y, t_z};
}

void ExecuteSeparableKernel::SetKernelName(const std::string& t_name)
{
    this->m_KernelName = t_name;
}

void ExecuteSeparableKernel::SetSources(const std::map<std::string, std::string>& t_sources)
{
    this->m_Sources = t_sources;
}

void ExecuteSeparableKernel::Execute()
{ 
    std::shared_ptr<cle::Buffer> src = std::dynamic_pointer_cast<cle::Buffer>(this->m_Parameters.at("src"));
    std::shared_ptr<cle::Buffer> dst = std::dynamic_pointer_cast<cle::Buffer>(this->m_Parameters.at("dst"));

    this->m_nDim = src->nDim();

    // create temp buffer
    cle::Buffer temp1 = this->m_gpu->CreateBuffer<float>(src->Shape());
    cle::Buffer temp2 = this->m_gpu->CreateBuffer<float>(src->Shape());

    // cl::Buffer tmp1_obj = cle::CreateBuffer<float>(src->GetSize(), this->m_gpu);
    // cle::Buffer temp1 (tmp1_obj, src->GetShape(), cle::Buffer::FLOAT);
    // cl::Buffer tmp2_obj = cle::CreateBuffer<float>(src->GetSize(), this->m_gpu);
    // cle::Buffer temp2 (tmp2_obj, src->GetShape(), cle::Buffer::FLOAT);

    SeparableKernel kernel(this->m_gpu);
    kernel.SetKernelName(this->m_KernelName);
    kernel.SetSources(this->m_Sources);
    if (this->m_Sigma[0] > 0)
    {
        kernel.SetInput(*src);
        if (this->m_nDim == 2)
        {
            kernel.SetOutput(temp1);
        }
        else
        {
            kernel.SetOutput(temp2);
        }
        kernel.SetSigma(this->m_Sigma[0]);
        kernel.SetSize(this->m_KernelSize[0]);
        kernel.SetDimension(0);
        kernel.Execute();
    }
    else
    {
        if (this->m_nDim == 2)
        {
            this->m_gpu->Copy<float>(*src, temp1);
        }
        else
        {
            this->m_gpu->Copy<float>(*src, temp2);
        }
    }

    if (this->m_Sigma[1] > 0)
    {
        if (this->m_nDim == 2)
        {
            kernel.SetInput(temp1);
            kernel.SetOutput( *dst );
        }
        else
        {
            kernel.SetInput(temp2);
            kernel.SetOutput(temp1);
        }
        kernel.SetSigma(this->m_Sigma[1]);
        kernel.SetSize(this->m_KernelSize[1]);
        kernel.SetDimension(1);
        kernel.Execute();
    }
    else
    {
        if (this->m_nDim == 2)
        {
            this->m_gpu->Copy<float>(temp1, *dst);
        }
        else
        {
            this->m_gpu->Copy<float>(temp2, temp1);
        }
    }
    if (this->m_nDim == 3)
    {
        if (this->m_Sigma[2] > 0)
        {
            kernel.SetInput(temp1);
            kernel.SetOutput(*dst);
            kernel.SetSigma(this->m_Sigma[2]);
            kernel.SetSize(this->m_KernelSize[2]);
            kernel.SetDimension(2);
            kernel.Execute();
        }
        else
        {
            this->m_gpu->Copy<float>(temp1, *dst);
        }
    }
}

} // namespace cle
