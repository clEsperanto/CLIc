
#include "cleMaximumOfAllPixelsKernel.hpp"
#include "cleMaximumZProjectionKernel.hpp"
#include "cleMaximumYProjectionKernel.hpp"
#include "cleMaximumXProjectionKernel.hpp"
#include "cleCopyKernel.hpp"


namespace cle
{
    
MaximumOfAllPixelsKernel::MaximumOfAllPixelsKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu, 
            "maximum_of_all_pixels",
            {"src", "dst"}
    )
{}

void MaximumOfAllPixelsKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void MaximumOfAllPixelsKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void MaximumOfAllPixelsKernel::Execute()
{
    auto src = this->GetParameter<Object>("src");
    auto dst = this->GetParameter<Object>("dst");
    std::array<size_t,3> dim = src->Shape();

    if (dim[2] > 1)
    {
        dim[2] = 1;
        MaximumZProjectionKernel kernel(this->m_gpu);
        kernel.SetInput(*src);
        if (src->IsObjectType("buffer"))
        {
            auto tmp = std::make_shared<Buffer>(this->m_gpu->CreateBuffer<float>(dim));
            kernel.SetOutput(*tmp);
            kernel.Execute();
            src = tmp;
        }
        else
        {
            auto tmp = std::make_shared<Image>(this->m_gpu->CreateImage<float>(dim));
            kernel.SetOutput(*tmp);
            kernel.Execute();
            src = tmp;
        }
    }
    if (dim[1] > 1)
    {
        dim[1] = 1;
        MaximumYProjectionKernel kernel(this->m_gpu);
        kernel.SetInput(*src);
        if (src->IsObjectType("buffer"))
        {
            auto tmp = std::make_shared<Buffer>(this->m_gpu->CreateBuffer<float>(dim));
            kernel.SetOutput(*tmp);
            kernel.Execute();
            src = tmp;
        }
        else
        { 
            auto tmp = std::make_shared<Image>(this->m_gpu->CreateImage<float>(dim));
            kernel.SetOutput(*tmp);
            kernel.Execute();
            src = tmp;
        }
    }
    MaximumXProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(*src);
    kernel.SetOutput(*dst);
    kernel.Execute();
}

} // namespace cle
