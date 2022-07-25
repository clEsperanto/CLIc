
#include "cleMaximumOfAllPixelsKernel.hpp"
#include "cleCopyKernel.hpp"
#include "cleMaximumXProjectionKernel.hpp"
#include "cleMaximumYProjectionKernel.hpp"
#include "cleMaximumZProjectionKernel.hpp"

namespace cle
{

MaximumOfAllPixelsKernel::MaximumOfAllPixelsKernel (const ProcessorPointer &device) : Operation (device, 2)
{
}

void
MaximumOfAllPixelsKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
MaximumOfAllPixelsKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
MaximumOfAllPixelsKernel::Execute ()
{
    // auto src = this->GetParameter<Object> ("src");
    // auto dst = this->GetParameter<Object> ("dst");
    // std::array<size_t, 3> dim = src->Shape ();

    // if (dim[2] > 1)
    //     {
    //         dim[2] = 1;
    //         MaximumZProjectionKernel kernel (this->m_gpu);
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
    //         MaximumYProjectionKernel kernel (this->m_gpu);
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
    // MaximumXProjectionKernel kernel (this->m_gpu);
    // kernel.SetInput (*src);
    // kernel.SetOutput (*dst);
    // kernel.Execute ();
}

} // namespace cle
