

#include "cleMinimumOfAllPixelsKernel.hpp"
#include "cleMinimumXProjectionKernel.hpp"
#include "cleMinimumYProjectionKernel.hpp"
#include "cleMinimumZProjectionKernel.hpp"

namespace cle
{

MinimumOfAllPixelsKernel::MinimumOfAllPixelsKernel (const ProcessorPointer &device) : Operation (device, 2)
{
}

void
MinimumOfAllPixelsKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
MinimumOfAllPixelsKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
MinimumOfAllPixelsKernel::Execute ()
{
    // auto src = this->GetParameter<Object> ("src");
    // auto dst = this->GetParameter<Object> ("dst");

    // std::array<size_t, 3> dim = src->Shape ();
    // if (dim[2] > 1)
    //     {
    //         dim[2] = 1;
    //         MinimumZProjectionKernel kernel (this->m_gpu);
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
    //         MinimumYProjectionKernel kernel (this->m_gpu);
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
    // MinimumXProjectionKernel kernel (this->m_gpu);
    // kernel.SetInput (*src);
    // kernel.SetOutput (*dst);
    // kernel.Execute ();
}

} // namespace cle
