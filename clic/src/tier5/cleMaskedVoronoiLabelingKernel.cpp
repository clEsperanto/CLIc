
#include "cleMaskedVoronoiLabelingKernel.hpp"

#include "cleSetKernel.hpp"
#include "cleOnlyzeroOverwriteMaximumBoxKernel.hpp"
#include "cleOnlyzeroOverwriteMaximumDiamondKernel.hpp"
#include "cleAddImageAndScalarKernel.hpp"
#include "cleAddImagesWeightedKernel.hpp"
#include "cleMaskKernel.hpp"
#include "cleConnectedComponentsLabelingBoxKernel.hpp" 
#include "cleHistogramKernel.hpp"

namespace cle
{

MaskedVoronoiLabelingKernel::MaskedVoronoiLabelingKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "masked_voronoi_labeling",
            {"src0" , "src1",  "dst"}
    )
{}    

void MaskedVoronoiLabelingKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src0");
}

void MaskedVoronoiLabelingKernel::SetMask(Object& t_x)
{
    this->AddObject(t_x, "src1");
}

void MaskedVoronoiLabelingKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void MaskedVoronoiLabelingKernel::Execute()
{
    // get I/O pointers
    auto src = this->GetParameter<Object>("src0");
    auto msk = this->GetParameter<Object>("src1");
    auto dst = this->GetParameter<Object>("dst");

    auto flup = this->m_gpu->Create<float>(dst->Shape());
    auto flip = this->m_gpu->Create<float>(dst->Shape());
    auto flop = this->m_gpu->Create<float>(dst->Shape());
    auto flag = this->m_gpu->Push<float>(std::vector<float>{1.0f}, {1,1,1});

    AddImageAndScalarKernel subtractOne(this->m_gpu);
    subtractOne.SetInput(*msk);
    subtractOne.SetOutput(flup);
    subtractOne.SetScalar(-1);
    subtractOne.Execute();

    ConnectedComponentsLabelingBoxKernel labeling(this->m_gpu);
    labeling.SetInput(*src);
    labeling.SetOutput(flop);
    labeling.Execute();

    AddImagesWeightedKernel add(this->m_gpu);
    add.SetInput1(flup);
    add.SetInput2(flop);
    add.SetOutput(flip);
    add.SetFactor1(1);
    add.SetFactor2(1);
    add.Execute();

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

    MaskKernel mask(this->m_gpu);
    if ((iteration_count%2) == 0)
    {
        mask.SetInput(flip);
    }
    else
    {
        mask.SetInput(flop);
    }
    mask.SetMask(*msk);
    mask.SetOutput(*dst);
    mask.Execute();
}

} // namespace cle
