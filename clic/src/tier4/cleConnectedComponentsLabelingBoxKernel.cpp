
#include "cleCloseIndexGapsInLabelMapKernel.hpp"

#include "cleConnectedComponentsLabelingBoxKernel.hpp"
#include "cleCopyKernel.hpp"
#include "cleNonzeroMinimumBoxKernel.hpp"
#include "cleSetKernel.hpp"
#include "cleSetNonzeroPixelsToPixelindexKernel.hpp"

namespace cle
{

ConnectedComponentsLabelingBoxKernel::ConnectedComponentsLabelingBoxKernel (const ProcessorPointer &device) : Operation (device, 2)
{
}

void
ConnectedComponentsLabelingBoxKernel::SetInput (const Image &object)
{
    this->AddParameter ("src", object);
}

void
ConnectedComponentsLabelingBoxKernel::SetOutput (const Image &object)
{
    this->AddParameter ("dst", object);
}

void
ConnectedComponentsLabelingBoxKernel::Execute ()
{
    // auto src = this->GetParameter<Object> ("src");
    // auto dst = this->GetParameter<Object> ("dst");

    // auto temp1 = this->m_gpu->Create<float> (src->Shape ());
    // auto temp2 = this->m_gpu->Create<float> (src->Shape ());
    // auto temp3 = this->m_gpu->Create<float> (src->Shape ());

    // SetNonzeroPixelsToPixelindexKernel set_nonzero_to_index_kernel (this->m_gpu);
    // set_nonzero_to_index_kernel.SetInput (*src);
    // set_nonzero_to_index_kernel.SetOutput (temp1);
    // set_nonzero_to_index_kernel.SetOffset (1);
    // set_nonzero_to_index_kernel.Execute ();

    // SetKernel set_init_kernel (this->m_gpu);
    // set_init_kernel.SetInput (temp2);
    // set_init_kernel.SetValue (0);
    // set_init_kernel.Execute ();

    // std::array<size_t, 3> flag_dim = { 1, 1, 2 };
    // std::vector<float> arr = { 0, 0 };
    // auto flag = this->m_gpu->Push<float> (arr, flag_dim);

    // std::vector<float> flag_value = { 1, 1 };
    // int iteration_count = 0;
    // NonzeroMinimumBoxKernel nonzero_minimum_kernel (this->m_gpu);
    // SetKernel set_flag_kernel (this->m_gpu);
    // while (flag_value[0] > 0)
    //     {
    //         nonzero_minimum_kernel.SetOutputFlag (flag);
    //         if (iteration_count % 2 == 0)
    //             {
    //                 nonzero_minimum_kernel.SetInput (temp1);
    //                 nonzero_minimum_kernel.SetOutput (temp2);
    //             }
    //         else
    //             {
    //                 nonzero_minimum_kernel.SetInput (temp2);
    //                 nonzero_minimum_kernel.SetOutput (temp1);
    //             }
    //         nonzero_minimum_kernel.Execute ();

    //         flag_value = this->m_gpu->Pull<float> (flag);
    //         set_flag_kernel.SetInput (flag);
    //         set_flag_kernel.SetValue (0);
    //         set_flag_kernel.Execute ();

    //         iteration_count++;
    //     }

    // CopyKernel copy (this->m_gpu);
    // if (iteration_count % 2 == 0)
    //     {
    //         copy.SetInput (temp1);
    //     }
    // else
    //     {
    //         copy.SetInput (temp2);
    //     }
    // copy.SetOutput (temp3);
    // copy.Execute ();

    // CloseIndexGapsInLabelMapKernel close_gaps_kernel (this->m_gpu);
    // close_gaps_kernel.SetInput (temp3);
    // close_gaps_kernel.SetOutput (*dst);
    // close_gaps_kernel.SetBlockSize (4096);
    // close_gaps_kernel.Execute ();
}

} // namespace cle
