

#include "cleSumOfAllPixelsKernel.h"
#include "cleSumZProjectionKernel.h"
#include "cleSumYProjectionKernel.h"
#include "cleSumXProjectionKernel.h"

namespace cle
{
    
void SumOfAllPixelsKernel::SetInput(Buffer& x)
{
    this->AddObject(x, "src");
}

void SumOfAllPixelsKernel::SetOutput(Buffer& x)
{
    this->AddObject(x, "dst_sum");
}

void SumOfAllPixelsKernel::Execute()
{
    std::shared_ptr<Buffer> src = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("src"));
    std::shared_ptr<Buffer> dst = std::dynamic_pointer_cast<Buffer>(m_ParameterList.at("dst_sum"));
    unsigned int dim[3] = {src->GetDimensions()[0], src->GetDimensions()[1], src->GetDimensions()[2]};

    if (dim[2] > 1)
    {
        dim[2] = 1;
        size_t size = dim[0] * dim[1] * dim[2];
        cl::Buffer tmp1_obj = CreateBuffer<float>(size, this->m_gpu);
        Buffer temp1 (tmp1_obj, dim, LightObject::Float);

        SumZProjectionKernel kernelZ(this->m_gpu);
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
        Buffer temp2 (tmp2_obj, dim, LightObject::Float);

        SumYProjectionKernel kernelY(this->m_gpu);
        kernelY.SetInput(*src);
        kernelY.SetOutput(temp2);
        kernelY.Execute();

        *src = temp2;
    }
    SumXProjectionKernel kernelX(this->m_gpu);
    kernelX.SetInput(*src);
    kernelX.SetOutput(*dst);
    kernelX.Execute();
}

} // namespace cle
