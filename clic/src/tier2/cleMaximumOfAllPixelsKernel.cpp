
#include "cleMaximumOfAllPixelsKernel.hpp"
#include "cleMaximumZProjectionKernel.hpp"
#include "cleMaximumYProjectionKernel.hpp"
#include "cleMaximumXProjectionKernel.hpp"

namespace cle
{
    
MaximumOfAllPixelsKernel::MaximumOfAllPixelsKernel (std::shared_ptr<GPU> gpu) : 
    Kernel( gpu, 
            "maximum_of_all_pixels",
            {"dst_max", "src"}
    )
{}

void MaximumOfAllPixelsKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void MaximumOfAllPixelsKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_max");
}

void MaximumOfAllPixelsKernel::Execute()
{
    std::shared_ptr<Buffer> src = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("src"));
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("dst_max"));
    std::array<int,3> dim = src->GetShape();

    if (dim[2] > 1)
    {
        dim[2] = 1;
        size_t size = dim[0] * dim[1] * dim[2];
        cl::Buffer tmp1_obj = CreateBuffer<float>(size, this->m_gpu);
        Buffer temp1 (tmp1_obj, dim, Buffer::FLOAT);

        MaximumZProjectionKernel kernelZ(this->m_gpu);
        kernelZ.SetInput(*src);
        kernelZ.SetOutput(temp1);
        kernelZ.Execute();

        *src = temp1;
    }
    if (dim[1] > 1)
    {
        dim[1] = 1;
        size_t size = dim[0] * dim[1] * dim[2];
        cl::Buffer tmp2_obj = CreateBuffer<float>(size, this->m_gpu);
        Buffer temp2 (tmp2_obj, dim, Buffer::FLOAT);

        MaximumYProjectionKernel kernelY(this->m_gpu);
        kernelY.SetInput(*src);
        kernelY.SetOutput(temp2);
        kernelY.Execute();

        *src = temp2;
    }
    MaximumXProjectionKernel kernelX(this->m_gpu);
    kernelX.SetInput(*src);
    kernelX.SetOutput(*dst);
    kernelX.Execute();
}

} // namespace cle
