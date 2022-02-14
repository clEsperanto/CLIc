

#include "cleMinimumOfAllPixelsKernel.hpp"
#include "cleMinimumZProjectionKernel.hpp"
#include "cleMinimumYProjectionKernel.hpp"
#include "cleMinimumXProjectionKernel.hpp"

namespace cle
{

MinimumOfAllPixelsKernel::MinimumOfAllPixelsKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "minimum_of_all_pixels",
            {"src", "dst"}
    )
{}    
    
void MinimumOfAllPixelsKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void MinimumOfAllPixelsKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void MinimumOfAllPixelsKernel::Execute()
{
    auto src = this->GetParameter<Object>("src");
    auto dst = this->GetParameter<Object>("dst");

    std::array<size_t,3> dim = src->Shape();
    if (dim[2] > 1)
    {
        dim[2] = 1;
        MinimumZProjectionKernel kernel(this->m_gpu);
        kernel.SetInput(*src);
        if (src->IsMemoryType(CL_MEM_OBJECT_BUFFER))
        {
            auto tmp = std::make_shared<Object>(this->m_gpu->Create<float>(dim));
            kernel.SetOutput(*tmp);
            kernel.Execute();
            src = tmp;
        }
        else
        {
            auto tmp = std::make_shared<Object>(this->m_gpu->Create<float>(dim, "image"));
            kernel.SetOutput(*tmp);
            kernel.Execute();
            src = tmp;
        }
    }
    if (dim[1] > 1)
    {
        dim[1] = 1;
        MinimumYProjectionKernel kernel(this->m_gpu);
        kernel.SetInput(*src);
        if (src->IsMemoryType(CL_MEM_OBJECT_BUFFER))
        {
            auto tmp = std::make_shared<Object>(this->m_gpu->Create<float>(dim));
            kernel.SetOutput(*tmp);
            kernel.Execute();
            src = tmp;
        }
        else
        { 
            auto tmp = std::make_shared<Object>(this->m_gpu->Create<float>(dim, "image"));
            kernel.SetOutput(*tmp);
            kernel.Execute();
            src = tmp;
        }
    }
    MinimumXProjectionKernel kernel(this->m_gpu);
    kernel.SetInput(*src);
    kernel.SetOutput(*dst);
    kernel.Execute();
}

} // namespace cle
