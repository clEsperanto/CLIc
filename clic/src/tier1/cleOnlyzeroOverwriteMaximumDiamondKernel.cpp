

#include "cleOnlyzeroOverwriteMaximumDiamondKernel.hpp"

namespace cle
{


OnlyzeroOverwriteMaximumDiamondKernel::OnlyzeroOverwriteMaximumDiamondKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "onlyzero_overwrite_maximum_diamond",
            {"src", "dst0", "dst1"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}    

void OnlyzeroOverwriteMaximumDiamondKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void OnlyzeroOverwriteMaximumDiamondKernel::SetOutput1(Object& t_x)
{
    this->AddObject(t_x, "dst0");
}

void OnlyzeroOverwriteMaximumDiamondKernel::SetOutput2(Object& t_x)
{
    this->AddObject(t_x, "dst1");
}

void OnlyzeroOverwriteMaximumDiamondKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->SetGlobalNDRange("dst1");
    this->EnqueueKernel();
}
} // namespace cle
