
#include "cleTopHatBoxKernel.hpp"

#include "cleAddImagesWeightedKernel.hpp"
#include "cleMaximumBoxKernel.hpp"
#include "cleMemory.hpp"
#include "cleMinimumBoxKernel.hpp"

namespace cle
{

TopHatBoxKernel::TopHatBoxKernel (const ProcessorPointer &device) : Operation (device, 2)
{
}

void
TopHatBoxKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
TopHatBoxKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
TopHatBoxKernel::SetRadius (const int &radius_x, const int &radius_y, const int &radius_z)
{
    this->radius_ = { radius_x, radius_y, radius_z };
}

void
TopHatBoxKernel::Execute ()
{
    auto src = this->GetImage ("src");
    auto dst = this->GetImage ("dst");

    auto temp1 = Memory::AllocateObject (this->Device (), src->Shape (), dst->BitType ().Get (), dst->MemType ().Get ());
    auto temp2 = Memory::AllocateObject (this->Device (), src->Shape (), dst->BitType ().Get (), dst->MemType ().Get ());

    MinimumBoxKernel minimum (this->Device ());
    minimum.SetInput (*src);
    minimum.SetOutput (temp1);
    minimum.SetRadius (this->radius_[0], this->radius_[1], this->radius_[2]);
    minimum.Execute ();

    MaximumBoxKernel maximum (this->Device ());
    maximum.SetInput (temp1);
    maximum.SetOutput (temp2);
    maximum.SetRadius (this->radius_[0], this->radius_[1], this->radius_[2]);
    maximum.Execute ();

    AddImagesWeightedKernel add (this->Device ());
    add.SetInput1 (*src);
    add.SetInput2 (temp2);
    add.SetOutput (*dst);
    add.SetFactor1 (1);
    add.SetFactor2 (-1);
    add.Execute ();
}

} // namespace cle
