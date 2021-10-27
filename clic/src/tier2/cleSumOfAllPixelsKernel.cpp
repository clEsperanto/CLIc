

#include "cleSumOfAllPixelsKernel.hpp"
#include "cleSumZProjectionKernel.hpp"
#include "cleSumYProjectionKernel.hpp"
#include "cleSumXProjectionKernel.hpp"

namespace cle
{

SumOfAllPixelsKernel::SumOfAllPixelsKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "sum_of_all_pixels",
            {"dst_sum", "src"}
    )
{}    
    
void SumOfAllPixelsKernel::SetInput(Buffer& t_x)
{
    this->AddObject(t_x, "src");
}

void SumOfAllPixelsKernel::SetOutput(Buffer& t_x)
{
    this->AddObject(t_x, "dst_sum");
}

void SumOfAllPixelsKernel::Execute()
{
    std::shared_ptr<Buffer> src = std::dynamic_pointer_cast<Buffer>(this->m_Parameters.at("src"));
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(this->m_Parameters.at("dst_sum"));
    std::array<int,3> dim = src->Shape();

    if (dim[2] > 1)
    {
        dim[2] = 1;
        cle::Buffer temp1 = this->m_gpu->CreateBuffer<float>(dim);

        SumZProjectionKernel sum_z_kernel(this->m_gpu);
        sum_z_kernel.SetInput(*src);
        sum_z_kernel.SetOutput(temp1);
        sum_z_kernel.Execute();

        *src = temp1;
    }
    if (dim[1] > 1)
    {
        dim[1] = 1;
        cle::Buffer temp2 = this->m_gpu->CreateBuffer<float>(dim);

        SumYProjectionKernel sum_y_kernel(this->m_gpu);
        sum_y_kernel.SetInput(*src);
        sum_y_kernel.SetOutput(temp2);
        sum_y_kernel.Execute();

        *src = temp2;
    }
    SumXProjectionKernel sum_x_kernel(this->m_gpu);
    sum_x_kernel.SetInput(*src);
    sum_x_kernel.SetOutput(*dst);
    sum_x_kernel.Execute();
}

} // namespace cle
