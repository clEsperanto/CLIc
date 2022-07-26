
#include "cleMaximumOfAllPixelsKernel.hpp"
#include "cleMaximumXProjectionKernel.hpp"
#include "cleMaximumYProjectionKernel.hpp"
#include "cleMaximumZProjectionKernel.hpp"
#include "cleMemory.hpp"

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
    auto src = this->GetImage ("src");
    auto dst = this->GetImage ("dst");
    std::array<size_t, 3> dim = src->Shape ();

    if (dim[2] > 1)
        {
            dim[2] = 1;
            auto temp = Memory::AllocateObject (this->Device (), dim, dst->BitType ().Get (), dst->MemType ().Get ());

            MaximumZProjectionKernel kernel (this->Device ());
            kernel.SetInput (*src);
            kernel.SetOutput (temp);
            kernel.Execute ();
            src = std::make_shared<Image> (temp);
        }

    if (dim[1] > 1)
        {
            dim[1] = 1;
            auto temp = Memory::AllocateObject (this->Device (), dim, dst->BitType ().Get (), dst->MemType ().Get ());

            MaximumYProjectionKernel kernel (this->Device ());
            kernel.SetInput (*src);
            kernel.SetOutput (temp);
            kernel.Execute ();
            src = std::make_shared<Image> (temp);
        }

    MaximumXProjectionKernel kernel (this->Device ());
    kernel.SetInput (*src);
    kernel.SetOutput (*dst);
    kernel.Execute ();
}

} // namespace cle
