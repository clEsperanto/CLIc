

#include "cleSumOfAllPixelsKernel.hpp"
#include "cleMemory.hpp"
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
    auto src = this->GetImage ("src");
    auto dst = this->GetImage ("dst");
    std::array<size_t, 3> dim = src->Shape ();

    if (dim[2] > 1)
        {
            dim[2] = 1;
            auto temp = Memory::AllocateObject (this->Device (), dim, dst->BitType ().Get (), dst->MemType ().Get ());

            SumZProjectionKernel kernel (this->Device ());
            kernel.SetInput (*src);
            kernel.SetOutput (temp);
            kernel.Execute ();
            src = std::make_shared<Image> (temp);
        }

    if (dim[1] > 1)
        {
            dim[1] = 1;
            auto temp = Memory::AllocateObject (this->Device (), dim, dst->BitType ().Get (), dst->MemType ().Get ());

            SumYProjectionKernel kernel (this->Device ());
            kernel.SetInput (*src);
            kernel.SetOutput (temp);
            kernel.Execute ();
            src = std::make_shared<Image> (temp);
        }

    SumXProjectionKernel kernel (this->Device ());
    kernel.SetInput (*src);
    kernel.SetOutput (*dst);
    kernel.Execute ();
}

} // namespace cle
