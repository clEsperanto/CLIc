
#include "cleExtendLabelingViaVoronoiKernel.hpp"

#include "cleCopyKernel.hpp"
#include "cleMemory.hpp"
#include "cleOnlyzeroOverwriteMaximumBoxKernel.hpp"
#include "cleOnlyzeroOverwriteMaximumDiamondKernel.hpp"
#include "cleSetKernel.hpp"

namespace cle
{

ExtendLabelingViaVoronoiKernel::ExtendLabelingViaVoronoiKernel (const ProcessorPointer &device) : Operation (device, 2)
{
}

void
ExtendLabelingViaVoronoiKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
ExtendLabelingViaVoronoiKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
ExtendLabelingViaVoronoiKernel::Execute ()
{
    // // get I/O pointers
    auto src = this->GetImage ("src");
    auto dst = this->GetImage ("dst");

    auto flip = Memory::AllocateObject (this->Device (), dst->Shape (), dst->BitType ().Get (), dst->MemType ().Get ());
    auto flop = Memory::AllocateObject (this->Device (), dst->Shape (), dst->BitType ().Get (), dst->MemType ().Get ());
    auto flag = Memory::AllocateObject (this->Device (), { 1, 1, 1 }, CL_FLOAT, CL_MEM_OBJECT_BUFFER);
    flag.Fill (1.0F);

    CopyKernel copy (this->Device ());
    copy.SetInput (*src);
    copy.SetOutput (flip);
    copy.Execute ();

    float flag_value = 1;
    int iteration_count = 0;
    while (flag_value > 0)
        {
            if ((iteration_count % 2) == 0)
                {
                    OnlyzeroOverwriteMaximumBoxKernel boxMaximum (this->Device ());
                    boxMaximum.SetInput (flip);
                    boxMaximum.SetOutput1 (flag);
                    boxMaximum.SetOutput2 (flop);
                    boxMaximum.Execute ();
                }
            else
                {
                    OnlyzeroOverwriteMaximumBoxKernel diamondMaximum (this->Device ());
                    diamondMaximum.SetInput (flop);
                    diamondMaximum.SetOutput1 (flag);
                    diamondMaximum.SetOutput2 (flip);
                    diamondMaximum.Execute ();
                }
            flag_value = Memory::ReadObject<float> (flag).front ();
            SetKernel set (this->Device ());
            set.SetInput (flag);
            set.SetValue (0);
            set.Execute ();
            iteration_count++;
        }

    if ((iteration_count % 2) == 0)
        {
            flip.CopyDataTo (*dst);
        }
    else
        {
            flop.CopyDataTo (*dst);
        }
}

} // namespace cle
