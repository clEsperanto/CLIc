
#include "cleCloseIndexGapsInLabelMapKernel.hpp"

#include "cleConnectedComponentLabelingBoxKernel.hpp"
#include "cleCopyKernel.hpp"
#include "cleMemory.hpp"
#include "cleNonzeroMinimumBoxKernel.hpp"
#include "cleSetKernel.hpp"
#include "cleSetNonzeroPixelsToPixelindexKernel.hpp"

namespace cle
{

ConnectedComponentLabelingBoxKernel::ConnectedComponentLabelingBoxKernel (const ProcessorPointer &device) : Operation (device, 2)
{
}

void
ConnectedComponentLabelingBoxKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
ConnectedComponentLabelingBoxKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
ConnectedComponentLabelingBoxKernel::Execute ()
{
    auto src = this->GetImage ("src");
    auto dst = this->GetImage ("dst");

    auto temp1 = Memory::AllocateObject (this->Device (), dst->Shape (), dst->BitType ().Get (), dst->MemType ().Get ());
    auto temp2 = Memory::AllocateObject (this->Device (), dst->Shape (), dst->BitType ().Get (), dst->MemType ().Get ());
    auto temp3 = Memory::AllocateObject (this->Device (), dst->Shape (), dst->BitType ().Get (), dst->MemType ().Get ());

    SetNonzeroPixelsToPixelindexKernel set_nonzero_to_index_kernel (this->Device ());
    set_nonzero_to_index_kernel.SetInput (*src);
    set_nonzero_to_index_kernel.SetOutput (temp1);
    set_nonzero_to_index_kernel.SetOffset (1);
    set_nonzero_to_index_kernel.Execute ();

    SetKernel set_init_kernel (this->Device ());
    set_init_kernel.SetInput (temp2);
    set_init_kernel.SetValue (0);
    set_init_kernel.Execute ();

    std::array<size_t, 3> flag_dim = { 1, 1, 2 };
    std::vector<float> arr = { 0, 0 };
    auto flag = Memory::AllocateObject (this->Device (), flag_dim);
    Memory::WriteObject (flag, arr);

    std::vector<float> flag_value = { 1, 1 };
    int iteration_count = 0;
    NonzeroMinimumBoxKernel nonzero_minimum_kernel (this->Device ());
    SetKernel set_flag_kernel (this->Device ());
    while (flag_value[0] > 0)
        {
            nonzero_minimum_kernel.SetOutputFlag (flag);
            if (iteration_count % 2 == 0)
                {
                    nonzero_minimum_kernel.SetInput (temp1);
                    nonzero_minimum_kernel.SetOutput (temp2);
                }
            else
                {
                    nonzero_minimum_kernel.SetInput (temp2);
                    nonzero_minimum_kernel.SetOutput (temp1);
                }
            nonzero_minimum_kernel.Execute ();

            flag_value = Memory::ReadObject<float> (flag);
            set_flag_kernel.SetInput (flag);
            set_flag_kernel.SetValue (0);
            set_flag_kernel.Execute ();

            iteration_count++;
        }

    if (iteration_count % 2 == 0)
        {
            temp1.CopyDataTo (temp3);
        }
    else
        {
            temp2.CopyDataTo (temp3);
        }

    CloseIndexGapsInLabelMapKernel close_gaps_kernel (this->Device ());
    close_gaps_kernel.SetInput (temp3);
    close_gaps_kernel.SetOutput (*dst);
    close_gaps_kernel.SetBlockSize (4096);
    close_gaps_kernel.Execute ();
}

} // namespace cle