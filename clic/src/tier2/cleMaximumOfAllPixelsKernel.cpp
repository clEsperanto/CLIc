
#include "cleMaximumOfAllPixelsKernel.hpp"
#include "cleMaximumZProjectionKernel.hpp"
#include "cleMaximumYProjectionKernel.hpp"
#include "cleMaximumXProjectionKernel.hpp"

namespace cle
{
    
MaximumOfAllPixelsKernel::MaximumOfAllPixelsKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu, 
            "maximum_of_all_pixels",
            {"dst_max", "src"}
    )
{}

void MaximumOfAllPixelsKernel::SetInput(Buffer& t_x)
{
    this->AddObject(t_x, "src");
}

void MaximumOfAllPixelsKernel::SetOutput(Buffer& t_x)
{
    this->AddObject(t_x, "dst_max");
}

void MaximumOfAllPixelsKernel::Execute()
{
    std::shared_ptr<Buffer> src = std::dynamic_pointer_cast<Buffer>(this->m_Parameters.at("src"));
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(this->m_Parameters.at("dst_max"));
    std::array<int,3> dim = src->Shape();

    if (dim[2] > 1)
    {
        dim[2] = 1;
        cle::Buffer temp1 = this->m_gpu->CreateBuffer<float>(dim);

        MaximumZProjectionKernel max_z_kernel(this->m_gpu);
        max_z_kernel.SetInput(*src);
        max_z_kernel.SetOutput(temp1);
        max_z_kernel.Execute();

        *src = temp1;
    }
    if (dim[1] > 1)
    {
        dim[1] = 1;
        cle::Buffer temp2 = this->m_gpu->CreateBuffer<float>(dim);

        MaximumYProjectionKernel max_y_kernel(this->m_gpu);
        max_y_kernel.SetInput(*src);
        max_y_kernel.SetOutput(temp2);
        max_y_kernel.Execute();

        *src = temp2;
    }
    MaximumXProjectionKernel max_x_kernel(this->m_gpu);
    max_x_kernel.SetInput(*src);
    max_x_kernel.SetOutput(*dst);
    max_x_kernel.Execute();
}

} // namespace cle
