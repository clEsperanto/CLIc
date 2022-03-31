

#include "cleOnlyzeroOverwriteMaximumBoxKernel.hpp"

namespace cle
{


OnlyzeroOverwriteMaximumBoxKernel::OnlyzeroOverwriteMaximumBoxKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "onlyzero_overwrite_maximum_box",
            {"src", "dst0", "dst1"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}    

void OnlyzeroOverwriteMaximumBoxKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void OnlyzeroOverwriteMaximumBoxKernel::SetOutput1(Object& t_x)
{
    this->AddObject(t_x, "dst0");
}

void OnlyzeroOverwriteMaximumBoxKernel::SetOutput2(Object& t_x)
{
    this->AddObject(t_x, "dst1");
}

void OnlyzeroOverwriteMaximumBoxKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->SetGlobalNDRange("dst1");
    this->EnqueueKernel();
}
} // namespace cle
