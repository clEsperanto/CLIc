
#include "cleExtendLabelingViaVoronoiKernel.hpp"

#include "cleSetKernel.hpp"
#include "cleOnlyzeroOverwriteMaximumBoxKernel.hpp"
#include "cleOnlyzeroOverwriteMaximumDiamondKernel.hpp"
#include "cleCopyKernel.hpp"

namespace cle
{

ExtendLabelingViaVoronoiKernel::ExtendLabelingViaVoronoiKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "extend_labeling_via_voronoi",
            {"src" , "dst"}
    )
{}    

void ExtendLabelingViaVoronoiKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void ExtendLabelingViaVoronoiKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void ExtendLabelingViaVoronoiKernel::Execute()
{
    // get I/O pointers
    auto src = this->GetParameter<Object>("src");
    auto dst = this->GetParameter<Object>("dst");

    std::vector<float> oneValue = {1.0f};

    auto flip = this->m_gpu->Create<float>(dst->Shape());
    auto flop = this->m_gpu->Create<float>(dst->Shape());
    auto flag = this->m_gpu->Push<float>(oneValue, {1,1,1});

    CopyKernel copy(this->m_gpu);
    copy.SetInput(*src);
    copy.SetOutput(flip);
    copy.Execute();

    float flag_value = 1;
    int iteration_count = 0;
    while (flag_value > 0)
    {
        if ((iteration_count%2) == 0)
        {
            OnlyzeroOverwriteMaximumBoxKernel boxMaximum(this->m_gpu);
            boxMaximum.SetInput(flip);
            boxMaximum.SetOutput1(flag);
            boxMaximum.SetOutput2(flop);
            boxMaximum.Execute();
        }
        else
        {
            OnlyzeroOverwriteMaximumBoxKernel diamondMaximum(this->m_gpu);
            diamondMaximum.SetInput(flop);
            diamondMaximum.SetOutput1(flag);
            diamondMaximum.SetOutput2(flip);
            diamondMaximum.Execute();
        }
        flag_value = this->m_gpu->Pull<float>(flag).front();
        SetKernel set(this->m_gpu);
        set.SetInput(flag);
        set.SetValue(0);
        set.Execute();
        iteration_count++;
    }

    if ((iteration_count%2) == 0)
    {
        copy.SetInput(flip);
    }
    else
    {
        copy.SetInput(flop);
    }
    copy.SetOutput(*dst);
    copy.Execute();
}

} // namespace cle
