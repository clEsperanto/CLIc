

#include "cleSumOfAllPixelsKernel.hpp"
#include "cleSumXProjectionKernel.hpp"
#include "cleSumYProjectionKernel.hpp"
#include "cleSumZProjectionKernel.hpp"

namespace cle
{

SumOfAllPixelsKernel::SumOfAllPixelsKernel (const ProcessorPointer &device) : Operation (device, 2)
{
}

void
SumOfAllPixelsKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
SumOfAllPixelsKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
SumOfAllPixelsKernel::Execute ()
{
    // auto src = this->GetParameter<Object> ("src");
    // auto dst = this->GetParameter<Object> ("dst");
    // std::array<size_t, 3> dim = src->Shape ();

    // if (dim[2] > 1)
    //     {
    //         dim[2] = 1;
    //         SumZProjectionKernel kernel (this->m_gpu);
    //         kernel.SetInput (*src);
    //         if (src->IsMemoryType (CL_MEM_OBJECT_BUFFER))
    //             {
    //                 auto tmp = std::make_shared<Object> (this->m_gpu->Create<float> (dim));
    //                 kernel.SetOutput (*tmp);
    //                 kernel.Execute ();
    //                 src = tmp;
    //             }
    //         else
    //             {
    //                 auto tmp = std::make_shared<Object> (this->m_gpu->Create<float> (dim, "image"));
    //                 kernel.SetOutput (*tmp);
    //                 kernel.Execute ();
    //                 src = tmp;
    //             }
    //     }
    // if (dim[1] > 1)
    //     {
    //         dim[1] = 1;
    //         SumYProjectionKernel kernel (this->m_gpu);
    //         kernel.SetInput (*src);
    //         if (src->IsMemoryType (CL_MEM_OBJECT_BUFFER))
    //             {
    //                 auto tmp = std::make_shared<Object> (this->m_gpu->Create<float> (dim));
    //                 kernel.SetOutput (*tmp);
    //                 kernel.Execute ();
    //                 src = tmp;
    //             }
    //         else
    //             {
    //                 auto tmp = std::make_shared<Object> (this->m_gpu->Create<float> (dim, "image"));
    //                 kernel.SetOutput (*tmp);
    //                 kernel.Execute ();
    //                 src = tmp;
    //             }
    //     }
    // SumXProjectionKernel sum_x_kernel (this->m_gpu);
    // sum_x_kernel.SetInput (*src);
    // sum_x_kernel.SetOutput (*dst);
    // sum_x_kernel.Execute ();
}

} // namespace cle
