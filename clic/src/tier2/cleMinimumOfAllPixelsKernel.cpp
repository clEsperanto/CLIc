

#include "cleMinimumOfAllPixelsKernel.hpp"
#include "cleMinimumZProjectionKernel.hpp"
#include "cleMinimumYProjectionKernel.hpp"
#include "cleMinimumXProjectionKernel.hpp"

namespace cle
{

MinimumOfAllPixelsKernel::MinimumOfAllPixelsKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "minimum_of_all_pixels",
            {"dst_min", "src"}
    )
{}    
    
void MinimumOfAllPixelsKernel::SetInput(Buffer& t_x)
{
    this->AddObject(t_x, "src");
}

void MinimumOfAllPixelsKernel::SetOutput(Buffer& t_x)
{
    this->AddObject(t_x, "dst_min");
}

void MinimumOfAllPixelsKernel::Execute()
{
    std::shared_ptr<Buffer> src = std::dynamic_pointer_cast<Buffer>(this->m_Parameters.at("src"));
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(this->m_Parameters.at("dst_min"));
    std::array<int,3> dim = src->Shape();
    if (dim[2] > 1)
    {
        dim[2] = 1;
        cle::Buffer temp1 = this->m_gpu->CreateBuffer<float>(dim);

        MinimumZProjectionKernel min_z_kernel(this->m_gpu);
        min_z_kernel.SetInput(*src);
        min_z_kernel.SetOutput(temp1);
        min_z_kernel.Execute();

        *src = temp1;
    }
    if (dim[1] > 1)
    {
        dim[1] = 1;
        cle::Buffer temp2 = this->m_gpu->CreateBuffer<float>(dim);

        MinimumYProjectionKernel min_y_kernel(this->m_gpu);
        min_y_kernel.SetInput(*src);
        min_y_kernel.SetOutput(temp2);
        min_y_kernel.Execute();

        *src = temp2;
    }
    MinimumXProjectionKernel min_x_kernel(this->m_gpu);
    min_x_kernel.SetInput(*src);
    min_x_kernel.SetOutput(*dst);
    min_x_kernel.Execute();
}

} // namespace cle
